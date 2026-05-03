/**
 * @author: luochenhao
 * @email: lch2022fox@163.com
 * @time: Fri 01 May 2026 21:45:03 CST
 * @brief: JSON工具类
*/

#ifndef TEAMTALK_IMCORE_JSONTOOLS_JSON_TOOLS_H_
#define TEAMTALK_IMCORE_JSONTOOLS_JSON_TOOLS_H_

#include <string>

namespace teamtalk::imcore::jsontools {

bool parse_json_string(const std::string& json_str, Json::Value& json_value);

bool parse_json_object(const Json::Value& json_value, std::string& json_str);

bool parse_json_file(const std::string& json_file, Json::Value& json_value);

}

#endif  // TEAMTALK_IMCORE_JSONTOOLS_JSON_TOOLS_H_