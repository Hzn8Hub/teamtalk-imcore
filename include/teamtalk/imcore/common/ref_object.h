/**
 * @author: luochenhao
 * @email: lch2022fox@163.com
 * @time: Fri 01 May 2026 21:08:22 CST
 * @brief: 引用计数对象类
*/

#ifndef TEAMTALK_IMCORE_COMMON_REF_OBJECT_H_
#define TEAMTALK_IMCORE_COMMON_REF_OBJECT_H_

#include <atomic>

namespace teamtalk::imcore::common {

// 引用计数对象类
class CRefObject {
 public:
 CRefObject();
  virtual ~CRefObject();

  void AddRef();
  void ReleaseRef();

 private:
  std::atomic<int> ref_count_;
};

}  // namespace teamtalk::imcore::common

#endif  // TEAMTALK_IMCORE_COMMON_REF_OBJECT_H_
