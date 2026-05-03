
/*
  Reviser: Polaris_hzn8
  Email: lch2022fox@163.com
  Github: https://github.com/Polaris-hzn8
  brief: 异步操作执行单位
*/

#ifndef TEAMTALK_IMCORE_NETLIB_OPERATION_IOPERATION_H_
#define TEAMTALK_IMCORE_NETLIB_OPERATION_IOPERATION_H_

#include <memory>
#include <teamtalk/imcore/netlib/ostype.h>

namespace teamtalk::imcore::netlib {

struct IOperation {
 public:
  virtual void process() = 0;
  virtual void release() = 0;
};

}  // namespace teamtalk::imcore::netlib

#endif  // TEAMTALK_IMCORE_NETLIB_OPERATION_IOPERATION_H_
