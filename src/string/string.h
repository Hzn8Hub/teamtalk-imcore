#ifndef IMCORE_STRING_H_
#define IMCORE_STRING_H_

#include <cctype>
#include <string>

/// @brief 将数字（0-15）转换为对应的大写十六进制字符
inline unsigned char to_hex(unsigned char x) {
  return x > 9 ? x - 10 + 'A' : x + '0';
}

/// @brief 将大写十六进制字符转换为对应的数值（0-15）
inline unsigned char from_hex(unsigned char x) {
  return isdigit(x) ? x - '0' : x - 'A' + 10;
}

/// @brief 对字符串进行 URL 编码
std::string url_encode(const std::string& s);

/// @brief 对 URL 编码的字符串进行解码
std::string url_decode(const std::string& s);

/// @brief 去除字符串首尾的空白字符（空格、制表符、回车、换行），原地修改
void str_trim(std::string& s);

/// @brief 安全地将字符串解析为整数
/// @param s   待解析的字符串
/// @param out 解析成功时写入结果
/// @return    解析成功返回 true，字符串非法或溢出返回 false
bool str_to_int(const std::string& s, int& out);

/// @brief 在内存块中查找子串
/// @param src_str 源内存起始地址
/// @param src_len 源内存长度
/// @param sub_str 子串起始地址
/// @param sub_len 子串长度（传 0 则由 strlen 自动计算）
/// @param forward true 从左向右搜索，false 从右向左搜索
/// @return 找到返回匹配位置指针，未找到返回 nullptr
const char* mem_find(const char* src_str, size_t src_len, const char* sub_str, size_t sub_len, bool forward = true);

#endif  // IMCORE_STRING_H_
