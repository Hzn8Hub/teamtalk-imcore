#ifndef IMCORE_STR_EXPLODE_H_
#define IMCORE_STR_EXPLODE_H_

#include <cstdint>

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

#endif  // IMCORE_STR_EXPLODE_H_
