/*
 Reviser: Polaris_hzn8
 Email: 3453851623@qq.com
 filename: imconn.cpp
 Update Time: Mon 12 Jun 2023 23:48:11 CST
 brief:
*/

#include <teamtalk/imcore/slog/slog.h>
#include <teamtalk/imcore/common/tools.h>
#include <teamtalk/imcore/netlib/imconn/conn.h>

namespace teamtalk::imcore::netlib {

namespace ttcommon = teamtalk::imcore::common;

// static uint64_t g_send_pkt_cnt = 0;		// 发送数据包总数
// static uint64_t g_recv_pkt_cnt = 0;		// 接收数据包总数

static CImConn* FindImConn(ConnMap_t* imconn_map, net_handle_t handle) {
  CImConn* pConn = NULL;
  ConnMap_t::iterator iter = imconn_map->find(handle);
  if (iter != imconn_map->end()) {
    pConn = iter->second;
    pConn->AddRef();
  }
  return pConn;
}

CImConn::CImConn() {
  log_debug("CImConn::CImConn ");
  m_busy = false;
  m_handle = NETLIB_INVALID_HANDLE;
  m_recv_bytes = 0;
  m_last_send_tick = ttcommon::get_tick_count();
  m_last_recv_tick = ttcommon::get_tick_count();
}

CImConn::~CImConn() {
  log_debug("CImConn::~CImConn, handle=%d ", m_handle);
}

// 向TCP连接中发送数据
int CImConn::Send(void* data, int len) {
  m_last_send_tick = ttcommon::get_tick_count();

  if (m_busy) {
    m_out_buf.Write(data, len);
    return len;
  }

  /* 将待发送数据分割为小的块，使用netlib_send分块发送 */
  int offset = 0;
  int remain = len;
  while (remain > 0) {
    int send_size = remain;
    if (send_size > NETLIB_MAX_SOCKET_BUF_SIZE)
      send_size = NETLIB_MAX_SOCKET_BUF_SIZE;
    int ret = netlib_send(m_handle, (char*)data + offset, send_size);
    if (ret <= 0) {
      ret = 0;
      break;
    }
    offset += ret;
    remain -= ret;
  }

  /* 剩余未发送数据写入输出缓冲区 */
  if (remain > 0) {
    m_out_buf.Write((char*)data + offset, remain);
    m_busy = true;
    log_debug("send busy, remain=%d ", m_out_buf.GetWriteOffset());
  } else {
    OnWriteCompelete();
  }

  return len;
}

int CImConn::SendPdu(CImPdu* pPdu) {
  if (!pPdu) {
    log_error("SendPdu pPdu is nullptr");
    return 0;
  }
  int len = pPdu->GetLength();
  if (len <= 0) {
    log_error("SendPdu pPdu length is invalid");
    return 0;
  }
  return Send(pPdu->GetBuffer(), len);
}

void CImConn::OnConnect(net_handle_t handle) {
  log_info("connect done handle:%d", m_handle);
  m_handle = handle;
}

void CImConn::OnClose() {
  log_info("connect close handle:%d", m_handle);
}

// 从TCP连接中读取数据 解析CImPdu对象
void CImConn::OnRead() {
  // 循环接收数据
  for (;;) {
    // 缓冲区扩容
    uint32_t free_buf_len = m_in_buf.GetAllocSize() - m_in_buf.GetWriteOffset();
    if (free_buf_len < READ_BUF_SIZE) {
      m_in_buf.Extend(READ_BUF_SIZE);
    }

    log_debug("handle = %u, netlib_recv into, time = %u\n", m_handle, ttcommon::get_tick_count());

    // 从连接中接收数据
    int ret = netlib_recv(m_handle, m_in_buf.GetBuffer() + m_in_buf.GetWriteOffset(), READ_BUF_SIZE);
    if (ret <= 0) {
      break;  //接收失败或连接关闭
    }

    m_recv_bytes += ret;
    m_in_buf.IncWriteOffset(ret);
    m_last_recv_tick = ttcommon::get_tick_count();
  }

  // 从输入缓冲区中解析出CImPdu对象，并处理每个CImPdu对象
  CImPdu* pPdu = nullptr;
  try {
    while ((pPdu = CImPdu::ReadPdu(m_in_buf.GetBuffer(), m_in_buf.GetWriteOffset()))) {
      uint32_t pdu_len = pPdu->GetLength();
      log_debug("handle = %u, pdu_len into = %u\n", m_handle, pdu_len);
      // 处理CImPdu对象
      HandlePdu(pPdu);
      // 移除已处理的数据
      m_in_buf.Read(nullptr, pdu_len);
      // 释放CImPdu对象
      delete pPdu;
      pPdu = nullptr;
    }
  } catch (CPduException& ex) {
    log_error("catch exception, sid=%u, cid=%u, err_code=%u, err_msg=%s, close the connection ",
              ex.GetServiceId(),
              ex.GetCommandId(),
              ex.GetErrorCode(),
              ex.GetErrorMsg());
    if (pPdu) {
      delete pPdu;
      pPdu = nullptr;
    }
    OnClose();  // 关闭连接回调
  }
}

// 将待发送的数据从输出缓冲区发送出去 直到输出缓冲区中的数据全部发送完成
void CImConn::OnWrite() {
  if (!m_busy)
    return;

  // 连接处于忙碌状态，即存在待发送的数据 进入循环进行数据发送
  while (m_out_buf.GetWriteOffset() > 0) {
    /* 分块将数据发送出去 */
    int send_size = m_out_buf.GetWriteOffset();
    if (send_size > NETLIB_MAX_SOCKET_BUF_SIZE) {
      send_size = NETLIB_MAX_SOCKET_BUF_SIZE;
    }

    int ret = netlib_send(m_handle, m_out_buf.GetBuffer(), send_size);
    if (ret <= 0) {
      ret = 0;
      break;
    }
    m_out_buf.Read(NULL, ret);  //从缓冲区中丢弃已发送数据
  }

  // 数据全部发送完毕
  if (m_out_buf.GetWriteOffset() == 0) {
    m_busy = false;
  }

  log_debug("onWrite, remain=%d ", m_out_buf.GetWriteOffset());
}

// 连接到服务器
// bool CImConn::Connect(const char* server_ip, uint16_t server_port) {
//   m_handle = netlib_connect(server_ip, server_port);
//   if (m_handle == NETLIB_INVALID_HANDLE) {
//     log_error("connect to server:%s:%d failed", server_ip, server_port);
//     return false;
//   }
//   log_info("connect to server:%s:%d success", server_ip, server_port);
//   return true;
// }

// 关闭连接
// bool CImConn::Close() {
//   if (m_handle == NETLIB_INVALID_HANDLE) {
//     log_error("connect is not established");
//     return false;
//   }
//   netlib_close(m_handle);
//   ReleaseRef();
//   log_info("connect close handle:%d", m_handle);
//   return true;
// }

// 关闭连接
// bool CImConn::Shutdown() {
//   if (m_handle == NETLIB_INVALID_HANDLE) {
//     log_error("connect is not established");
//     return false;
//   }
//   netlib_shutdown(m_handle);
//   ReleaseRef();
//   log_info("connect shutdown handle:%d", m_handle);
//   return true;
// }

// 处理与网络连接相关的消息(事件分发)
void imconn_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam) {
  NOTUSED_ARG(handle);
  NOTUSED_ARG(pParam);

  if (!callback_data) {
    log_error("imconn_callback callback_data is nullptr");
    return;
  }

  ConnMap_t* conn_map = (ConnMap_t*)callback_data;
  CImConn* pConn = FindImConn(conn_map, handle);
  if (!pConn) {
    log_error("connection is invalied:%d", handle);
    return;
  }

  switch (msg) {
    case NETLIB_MSG_CONFIRM:
      pConn->OnConfirm();
      break;
    case NETLIB_MSG_READ:
      pConn->OnRead();
      break;
    case NETLIB_MSG_WRITE:
      pConn->OnWrite();
      break;
    case NETLIB_MSG_CLOSE:
      pConn->OnClose();
      break;
    default:
      log_error("!!!imconn_callback error msg: %d ", msg);
      break;
  }

  pConn->ReleaseRef();
}

}  // namespace teamtalk::imcore::netlib
