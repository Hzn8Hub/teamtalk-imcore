/*
 Reviser: Polaris_hzn8
 Email: 3453851623@qq.com
 filename: EventDispatch.h
 Update Time: Wed 14 Jun 2023 15:25:05 CST
 brief: 事件调度器（CEventDispatch）用于处理事件的调度和触发
        A socket event dispatcher, features include:
        1. portable: worked both on Windows, MAC OS X,  LINUX platform（多平台）
        2. a singleton pattern: only one instance of this class can
 exist（应用单例模式）
*/

#ifndef TEAMTALK_IMCORE_NETLIB_CORE_EVENT_DISPATCH_H
#define TEAMTALK_IMCORE_NETLIB_CORE_EVENT_DISPATCH_H

#include <list>
#include <mutex>
#include <teamtalk/imcore/netlib/ostype.h>
#include <teamtalk/imcore/netlib/utils/basic_tools.h>

namespace teamtalk::imcore::netlib {

enum {
  SOCKET_READ = 0x1,          // 读事件
  SOCKET_WRITE = 0x2,         // 写事件
  SOCKET_EXCEP = 0x4,         // 异常事件
  SOCKET_ALL = 0x7            // 所有事件
};

// 套接字事件处理分发
class CEventDispatch {
 public:
  virtual ~CEventDispatch();

  void AddTimer(callback_t callback, void* user_data, uint64_t interval);
  void RemoveTimer(callback_t callback, void* user_data);
  void AddLoop(callback_t callback, void* user_data);

  bool isRunning() { return running; }

  void AddEvent(SOCKET fd, uint8_t socket_event);
  void RemoveEvent(SOCKET fd, uint8_t socket_event);

  void StartDispatch(uint32_t wait_timeout = 100);
  void StopDispatch();

  static CEventDispatch* Instance();

 protected:
  CEventDispatch();

 private:
  typedef struct {
    callback_t callback;                   // 回调函数
    void* user_data;                       // 用户数据
    uint64_t interval;                     // 间隔时间
    uint64_t next_tick;                    // 下次执行时间
  } TimerItem;

  void _CheckTimer();
  void _CheckLoop();

 std::mutex m_lock;                         // 互斥锁
 std::list<TimerItem*> m_timer_list;        // 定时器列表
 std::list<TimerItem*> m_loop_list;         // 循环列表

#ifdef _WIN32
  fd_set m_read_set;                         // 读事件集合
  fd_set m_write_set;                        // 写事件集合
  fd_set m_excep_set;                        // 异常事件集合
#elif __APPLE__
  int m_kqfd;                                // 事件驱动文件描述符
#else
  int m_epfd;                                // 事件驱动文件描述符
#endif
  bool running;                              // 是否运行中
};

}  // namespace teamtalk::imcore::netlib

#endif // TEAMTALK_IMCORE_NETLIB_CORE_EVENT_DISPATCH_H
