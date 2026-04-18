#include "string.h"

#include <cstring>
#include <stdexcept>

std::string url_encode(const std::string& s) {
  std::string out;
  for (size_t i = 0; i < s.size(); i++) {
    unsigned char buf[4] = {};
    if (isalnum((unsigned char)s[i])) {
      buf[0] = s[i];
    } else {
      buf[0] = '%';
      buf[1] = to_hex((unsigned char)s[i] >> 4);
      buf[2] = to_hex((unsigned char)s[i] % 16);
    }
    out += (char*)buf;
  }
  return out;
}

std::string url_decode(const std::string& s) {
  std::string out;
  for (size_t i = 0; i < s.size(); i++) {
    unsigned char ch = 0;
    if (s[i] == '%') {
      ch = (from_hex(s[i + 1]) << 4);
      ch |= from_hex(s[i + 2]);
      i += 2;
    } else if (s[i] == '+') {
      ch = ' ';
    } else {
      ch = s[i];
    }
    out += (char)ch;
  }
  return out;
}

void str_trim(std::string& s) {
  const size_t first = s.find_first_not_of(" \t\r\n");
  if (first == std::string::npos) {
    s.clear();
    return;
  }
  const size_t last = s.find_last_not_of(" \t\r\n");
  s = s.substr(first, last - first + 1);
}

bool str_to_int(const std::string& s, int& out) {
  try {
    out = std::stoi(s);
    return true;
  } catch (const std::exception&) {
    return false;
  }
}

const char* mem_find(const char* src_str, size_t src_len, const char* sub_str, size_t sub_len, bool forward) {
  if (src_str == nullptr || sub_str == nullptr || src_len == 0) {
    return nullptr;
  }

  // sub_len 补全必须在边界检查之前，否则 sub_len==0 时会跳过后续校验
  if (sub_len == 0) {
    sub_len = strlen(sub_str);
  }
  if (sub_len == 0 || sub_len > src_len) {
    return nullptr;
  }

  const size_t last = src_len - sub_len;
  for (size_t i = 0; i <= last; i++) {
    const size_t pos = forward ? i : last - i;
    if (memcmp(src_str + pos, sub_str, sub_len) == 0) {
      return src_str + pos;
    }
  }
  return nullptr;
}
