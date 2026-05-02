
/*
 Reviser: Polaris_hzn8
 Email: lch2022fox@163.com
 Github: https://github.com/Polaris-hzn8
 brief: 逻辑引擎异常
*/

#ifndef EXCEPTION_7889B422_4F2F_495F_AFAC_180F657486D2_H__
#define EXCEPTION_7889B422_4F2F_495F_AFAC_180F657486D2_H__

#include <string>
#include <ostype.h>

namespace imcore {

#pragma warning(disable : 4251)
struct NETWORK_DLL Exception : public std::exception {
 public:
  Exception(int errorCode, const std::string& msg) : m_ErrorCode(errorCode), m_msg(msg) {}

 public:
  int m_ErrorCode;
  std::string m_msg;
};

}  // namespace imcore

#endif  // EXCEPTION_7889B422_4F2F_495F_AFAC_180F657486D2_H__
