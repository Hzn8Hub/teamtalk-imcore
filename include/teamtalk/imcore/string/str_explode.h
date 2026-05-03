/**
 * @author: luochenhao
 * @email: lch2022fox@163.com
 * @time: Fri 01 May 2026 21:42:29 CST
 * @brief: 字符串拆分类
 */

#ifndef TEAMTALK_IMCORE_STRING_STR_EXPLODE_H_
#define TEAMTALK_IMCORE_STRING_STR_EXPLODE_H_

#include <cstdint>

namespace teamtalk::imcore::string {

/// @brief 按指定分隔符将 C 字符串拆分为子串数组
class CStrExplode {
 public:
  CStrExplode(char* str, char separator);
  virtual ~CStrExplode();

  uint32_t GetItemCnt() { return item_cnt_; }
  char* GetItem(uint32_t idx) { return item_list_[idx]; }

 private:
  uint32_t item_cnt_;
  char** item_list_;
};

}  // namespace teamtalk::imcore::string

#endif  // TEAMTALK_IMCORE_STRING_STR_EXPLODE_H_
