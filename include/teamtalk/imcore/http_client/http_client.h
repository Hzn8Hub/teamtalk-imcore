/**
 * @author: luochenhao
 * @email: lch2022fox@163.com
 * @time: Sun 03 May 2026 23:16:33 CST
 * @brief: HTTP客户端类
 **/

#ifndef TEAMTALK_IMCORE_HTTP_CLIENT_HTTP_CLIENT_H_
#define TEAMTALK_IMCORE_HTTP_CLIENT_HTTP_CLIENT_H_

#include <string>
#include <curl/curl.h>

namespace teamtalk::imcore::http_client {

class CHttpClient {
 public:
  CHttpClient(void);
  ~CHttpClient(void);

 public:
  CURLcode Post(const std::string& strUrl, const std::string& strPost, std::string& strResponse);
  CURLcode Get(const std::string& strUrl, std::string& strResponse);
  // 语音数据上传下载
  std::string UploadByteFile(const std::string& url, void* data, int data_len);
  // bool DownloadByteFile(const std::string& url, AudioMsgInfo* pAudioMsg);
};

}  // namespace teamtalk::imcore::http_client

#endif  // TEAMTALK_IMCORE_HTTP_CLIENT_HTTP_CLIENT_H_
