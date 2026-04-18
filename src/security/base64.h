/*
 Reviser: Polaris_hzn8
 Email: 3453851623@qq.com
 filename: base64.h
 Update Time: Mon 12 Jun 2023 12:48:23 CST
 brief:
*/

#ifndef __BASE64_H__
#define __BASE64_H__

#include <string>

std::string base64_decode(const std::string& ascdata);
std::string base64_encode(const std::string& bindata);

#endif
