
/*
 Reviser: Polaris_hzn8
 Email: lch2022fox@163.com
 Github: https://github.com/Polaris-hzn8
 brief: 逻辑引擎异常
*/

#ifndef TEAMTALK_IMCORE_NETLIB_OPERATION_TT_EXCEPTION_H_
#define TEAMTALK_IMCORE_NETLIB_OPERATION_TT_EXCEPTION_H_

#include <string>
#include <teamtalk/imcore/netlib/ostype.h>

namespace teamtalk::imcore::netlib {

struct Exception : public std::exception {
 public:
  Exception(int32_t errorCode, const std::string& msg) : m_ErrorCode(errorCode), m_msg(msg) {}
 public:
  int32_t m_ErrorCode;
  std::string m_Message;
};

}  // namespace teamtalk::imcore::netlib

#endif  // TEAMTALK_IMCORE_NETLIB_OPERATION_TT_EXCEPTION_H_
