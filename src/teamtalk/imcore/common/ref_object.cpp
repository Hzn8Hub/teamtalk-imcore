/**
 * @author: luochenhao
 * @email: lch2022fox@163.com
 * @time: Fri 01 May 2026 21:15:06 CST
 * @brief: 引用计数对象类实现
 */

#include <teamtalk/imcore/common/ref_object.h>

namespace teamtalk::imcore::common {

// CRefObject实现
CRefObject::CRefObject() : ref_count_(1) {}

CRefObject::~CRefObject() {}

void CRefObject::AddRef() {
  ref_count_.fetch_add(1, std::memory_order_relaxed);
}

void CRefObject::ReleaseRef() {
  int oldCount = ref_count_.fetch_sub(1, std::memory_order_acq_rel);
  if (oldCount == 1) {
    delete this;
  }
}

}  // namespace teamtalk::imcore::common
