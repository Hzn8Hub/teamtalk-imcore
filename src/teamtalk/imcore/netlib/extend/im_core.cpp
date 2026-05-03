
/*
 Reviser: Polaris_hzn8
 Email: lch2022fox@163.com
 Github: https://github.com/Polaris-hzn8
 brief:
*/

#include <mutex>
#include <thread>
#include <teamtalk/imcore/netlib/core/netlib.h>
#include <teamtalk/imcore/netlib/extend/im_core.h>
#include <teamtalk/imcore/netlib/extend/tcp_sockets_manager.h>
#include <teamtalk/imcore/netlib/operation/operation_manager.h>

namespace teamtalk::imcore::netlib {

static std::mutex g_lock;

#ifdef _MSC_VER
HANDLE g_hEventThread = NULL;
unsigned int __stdcall event_run(void* threadArgu) {
  log_info("event_run");
  netlib_init();
  netlib_set_running();
  netlib_eventloop();
  return NULL;
}
#else
void* event_run(void* arg) {
  log_info("event_run");
  netlib_init();
  netlib_eventloop();
  return NULL;
}
#endif

// start/stop
bool IMLibCoreRunEvent() {
  log_info("===============================================================================");

  // start thread -> operation tasklist
  getOperationManager()->startup();

  std::lock_guard<std::mutex> lock(g_lock);
  if (netlib_is_running())
    return true;

    // start thread -> netlib_eventloop
#ifdef _MSC_VER
  unsigned int threadId = 0;
  g_hEventThread = (HANDLE)_beginthreadex(nullptr, 0, event_run, nullptr, 0, &threadId);
  if (g_hEventThread == NULL || g_hEventThread == INVALID_HANDLE_VALUE) {
    g_hEventThread = NULL;
    return false;
  }
#else
  pthread_t thread_id;
  if (pthread_create(&thread_id, nullptr, event_run, nullptr) != 0) {
    return false;
  }
#endif

  return true;
}

bool IMLibCoreStopEvent() {
  netlib_stop_event();

#ifdef _MSC_VER
  if (g_hEventThread) {
    if (WaitForSingleObject(g_hEventThread, 3000) == WAIT_TIMEOUT) {
      TerminateThread(g_hEventThread, 0);
      WaitForSingleObject(g_hEventThread, 500);
    }
    CloseHandle(g_hEventThread);
    g_hEventThread = NULL;
  }
#endif

  netlib_destroy();

  getOperationManager()->shutdown();

  return true;
}

bool IMLibCoreIsRunning() {
  return netlib_is_running();
}

// imcore
int IMLibCoreConnect(std::string ip, int port) {
  return TcpSocketsManager::getInstance()->connect(ip.c_str(), port);
}

int IMLibCoreWrite(int key, uchar_t* data, uint32_t size) {
  int nRet = -1;
  int nHandle = key;
  CImConn* pConn = TcpSocketsManager::getInstance()->get_client_conn(nHandle);
  if (pConn) {
    pConn->Send((void*)data, size);
  } else {
    log_error("connection is invalied:%d", key);
  }
  return nRet;
}

void IMLibCoreShutdown(int key) {
  log_info("shutdown key:%d", key);
  int nHandle = key;
  CImConn* pConn = TcpSocketsManager::getInstance()->get_client_conn(nHandle);
  if (pConn) {
    // pConn->Shutdown();
  }
}

void IMLibCoreClose(int key) {
  log_info("close key:%d", key);
  int nHandle = key;
  CImConn* pConn = TcpSocketsManager::getInstance()->get_client_conn(nHandle);
  if (pConn) {
    // pConn->Close();
  }
}

// void IMLibCoreRegisterCallback(int handle, ITcpSocketCallback* pCB) {
//   TcpSocketsManager::getInstance()->registerCallback(handle, pCB);
// }

void IMLibCoreUnRegisterCallback(int handle) {
  TcpSocketsManager::getInstance()->unRegisterCallback(handle);
}

// operation
void IMLibCoreStartOperation(Operation* pOperation, int32_t delay /*= 0*/) {
  if (getOperationManager()->startOperation(pOperation, delay) != IMCORE_OK) {
    log_error("push operation failed");
  }
}

void IMLibCoreStartOperationWithLambda(std::function<void()> operationRun,
                                       int32_t delay /*= 0*/,
                                       std::string oper_name) {
  if (getOperationManager()->startOperationWithLambda(operationRun, delay, oper_name) != IMCORE_OK) {
    log_error("push operation with lambda failed");
  }
}

void IMLibCoreClearOperationByName(std::string oper_name) {
  if (getOperationManager()->clearOperationByName(oper_name) != IMCORE_OK) {
    log_error("clear operation by name failed");
  }
}

}  // namespace teamtalk::imcore::netlib
