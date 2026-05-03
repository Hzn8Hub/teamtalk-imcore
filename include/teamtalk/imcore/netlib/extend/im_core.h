
/*
 Reviser: Polaris_hzn8
 Email: lch2022fox@163.com
 Github: https://github.com/Polaris-hzn8
 brief: im cross platform library,currently include imcore
*/

#ifndef TEAMTALK_IMCORE_NETLIB_EXTEND_IM_CORE_H_
#define TEAMTALK_IMCORE_NETLIB_EXTEND_IM_CORE_H_

#include <string.h>
#include <iostream>
#include <functional>
#include <teamtalk/imcore/slog/slog.h>
#include <teamtalk/imcore/netlib/ostype.h>

namespace teamtalk::imcore::netlib {

const std::string OPERATION_NAME_MSG_READ_ACK = "operation_name_msg_read_ack";

class Operation;
struct ITcpSocketCallback;

#ifdef _MSC_VER
unsigned int __stdcall event_run(void* threadArgu);
#else
void* event_run(void* arg);
#endif

// start/stop
bool IMLibCoreRunEvent();
bool IMLibCoreStopEvent();
bool IMLibCoreIsRunning();

// imcore
int IMLibCoreConnect(std::string ip, int port);
int IMLibCoreWrite(int key, uchar_t* data, uint32_t size);
void IMLibCoreShutdown(int key);
void IMLibCoreClose(int key);
// void IMLibCoreRegisterCallback(int handle, ITcpSocketCallback* pCB);
void IMLibCoreUnRegisterCallback(int handle);

// operation
// 任务队列插入
void IMLibCoreStartOperation(Operation* pOperation, int32_t delay = 0);

// 任务队列插入Lambda
void IMLibCoreStartOperationWithLambda(std::function<void()> operationRun,
                                       int32_t delay = 0,
                                       std::string oper_name = "_common_operation_name");

// 任务队列删除
void IMLibCoreClearOperationByName(std::string oper_name);

}  // namespace teamtalk::imcore::netlib

#endif  // TEAMTALK_IMCORE_NETLIB_EXTEND_IM_CORE_H_
