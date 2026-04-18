
#include "str_explode.h"
#include <cstring>

CStrExplode::CStrExplode(char* str, char separator) {
  item_cnt_ = 1;
  char* pos = str;
  while (*pos) {
    if (*pos == separator)
      item_cnt_++;
    pos++;
  }

  item_list_ = new char*[item_cnt_];

  int idx = 0;
  char* start = pos = str;
  while (*pos) {
    if (pos != start && *pos == separator) {
      uint32_t len = pos - start;
      item_list_[idx] = new char[len + 1];
      strncpy(item_list_[idx], start, len);
      item_list_[idx][len] = '\0';
      idx++;
      start = pos + 1;
    }
    pos++;
  }

  uint32_t len = pos - start;
  if (len != 0) {
    item_list_[idx] = new char[len + 1];
    strncpy(item_list_[idx], start, len);
    item_list_[idx][len] = '\0';
  }
}

CStrExplode::~CStrExplode() {
  for (uint32_t i = 0; i < item_cnt_; i++) {
    delete[] item_list_[i];
  }
  delete[] item_list_;
}
