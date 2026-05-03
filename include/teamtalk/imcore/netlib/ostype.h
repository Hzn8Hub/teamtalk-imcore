/*
 Reviser: Polaris_hzn8
 Email: 3453851623@qq.com
 filename: ostype.h
 Update Time: Mon 12 Jun 2023 08:34:19 CST
 brief: OS dependant type definition
*/

#ifndef TEAMTALK_IMCORE_NETLIB_OSTYPE_H_
#define TEAMTALK_IMCORE_NETLIB_OSTYPE_H_

#include <stdexcept>

#ifdef _WIN32
#include <WinBase.h>
#include <WinSock2.h>
#include <Windows.h>
#include <direct.h>
#include <stdint.h>
#else
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>  // define int8_t ...
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define closesocket close
#define ioctlsocket ioctl

#ifdef __APPLE__
#include <sys/event.h>
#include <sys/syscall.h>  // syscall(SYS_gettid)
#include <sys/time.h>
#else
#include <sys/epoll.h>
#endif
#endif

#define NETLIB_INVALID_HANDLE -1

#ifdef _WIN32
#define SOCKOPT_CAST(x) reinterpret_cast<const char*>(x)
#define SOCKOPT_PTR(x) reinterpret_cast<char*>(x)
#else
#define SOCKOPT_CAST(x) (x)
#define SOCKOPT_PTR(x) (x)
#endif

#define NOTUSED_ARG(v) ((void)v)  // used this to remove warning C4100, unreferenced parameter

typedef void (*callback_t)(void* callback_data, uint8_t msg, uint32_t handle, void* pParam);

typedef unsigned char uchar_t;
typedef int net_handle_t;
typedef int conn_handle_t;

typedef signed char Int8;
typedef unsigned char UInt8;
typedef signed short Int16;
typedef unsigned short UInt16;
typedef signed int Int32;
typedef unsigned int UInt32;
// typedef signed __int64 Int64;
// typedef unsigned __int64 UInt64;

// typedef std::basic_string<char> ByteBuffer;
// typedef std::string utf8String;
// typedef char utf8char;
// typedef unsigned char byte;

#ifdef _WIN32
typedef int socklen_t;
#else
typedef int SOCKET;
typedef int BOOL;

const int SOCKET_ERROR = -1;
const int INVALID_SOCKET = -1;

#ifndef __APPLE__
const int TRUE = 1;
const int FALSE = 0;
#endif
#endif

const uint32_t INVALID_UINT32 = (uint32_t)-1;
const uint32_t INVALID_VALUE = 0;

enum { NETLIB_OK = 0, NETLIB_ERROR = -1 };

enum {
  NETLIB_MSG_CONNECT = 1,
  NETLIB_MSG_CONFIRM,
  NETLIB_MSG_READ,
  NETLIB_MSG_WRITE,
  NETLIB_MSG_CLOSE,
  NETLIB_MSG_TIMER,
  NETLIB_MSG_LOOP
};

#endif // TEAMTALK_IMCORE_NETLIB_OSTYPE_H_
