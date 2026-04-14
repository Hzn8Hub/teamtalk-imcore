
/*
 Reviser: Polaris_hzn8
 Email: lch2022fox@163.com
 Github: https://github.com/Polaris-hzn8
 brief: 封装的操作对象operation
*/

#include <exception>
#include <imcore/ostype.h>
#include <imcore/operation/operation.h>
#include <imcore/operation/tt_exception.h>

namespace imcore {

const std::string OPERATION_NAME_COMMON = "operation_name_common";

Operation::Operation() : m_optState(OPERATION_IDLE), m_optName(OPERATION_NAME_COMMON) {}

Operation::Operation(const std::string& name) : m_optState(OPERATION_IDLE), m_optName(name) {}

Operation::~Operation() {}

void Operation::process() {
  try {
    m_optState = OPERATION_RUNNING;
    processOpertion();
  } catch (Exception& exc) {
    assert(false);
    LOG__(ERR, _T("process exception,reason:%s"), exc.what());
  } catch (std::exception& exc) {
    assert(false);
    LOG__(ERR, _T("process exception,reason:%s"), exc.what());
  } catch (...) {
    assert(false);
    LOG__(ERR, _T("process unknown exception"));
  }
  m_optState = OPERATION_FINISHED;
}

void Operation::release() {
  delete this;
}

}  // namespace imcore
