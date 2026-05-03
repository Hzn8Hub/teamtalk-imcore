
#include <fstream>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/forwards.h>
#include <teamtalk/imcore/jsontools/jsontools.h>

namespace teamtalk::imcore::jsontools {

bool parse_json_string(const std::string& json_str, Json::Value& json_value) {
  Json::Reader reader;
  if (!reader.parse(json_str, json_value)) {
    return false;
  }
  return true;
}

bool parse_json_object(const Json::Value& json_value, std::string& json_str) {
  Json::FastWriter writer;
  json_str = writer.write(json_value);
  return true;
}

bool parse_json_file(const std::string& json_file, Json::Value& json_value) {
  std::ifstream file(json_file);
  if (!file.is_open()) {
    return false;
  }
  Json::Reader reader;
  if (!reader.parse(file, json_value)) {
    file.close();
    return false;
  }
  file.close();
  return true;
}

}  // namespace teamtalk::imcore::jsontools
