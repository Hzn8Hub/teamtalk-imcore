
/*
 Reviser: Polaris_hzn8
 Email: lch2022fox@163.com
 Github: https://github.com/Polaris-hzn8
 brief: 封装的操作对象operation
*/

#ifndef TEAMTALK_IMCORE_NETLIB_OPERATION_OPERATION_H_
#define TEAMTALK_IMCORE_NETLIB_OPERATION_OPERATION_H_

#include <teamtalk/imcore/netlib/ostype.h>
#include <teamtalk/imcore/netlib/operation/ioperation.h>

namespace teamtalk::imcore::netlib {

class Operation : public IOperation {
  enum OperationState {
    OPERATION_IDLE = 0,
    OPERATION_STARTING,
    OPERATION_RUNNING,
    OPERATION_CANCELLING,
    OPERATION_FINISHED
  };

 public:
  Operation();
  Operation(const std::string& name);
  virtual ~Operation();

 public:
  virtual void processOpertion() = 0;

 public:
  virtual void process();
  virtual void release();

  inline std::string getName() const { return m_optName; }
  inline void setName(const std::string& optName) { m_optName = optName; }

 private:
  std::string m_optName;
  OperationState m_optState;
};

}  // namespace teamtalk::imcore::netlib

#endif  // TEAMTALK_IMCORE_NETLIB_OPERATION_OPERATION_H_
