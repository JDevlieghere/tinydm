#pragma once

#include <curl/curl.h>

#include <cstddef>
#include <string>
#include <utility>

namespace tinydm {
class Download {
 public:
  Download();
  ~Download();

  std::pair<bool, std::string> download(const std::string &url);

 private:
  std::string get_filename(std::string url);

  std::string unescape(const std::string &url);

  static std::size_t write_callback(char *ptr, std::size_t size,
                                    std::size_t nmemb, void *data);

  CURL *_curl;
};
}
