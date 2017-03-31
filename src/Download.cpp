#include "Download.h"

#include <curl/curl.h>
#include <curl/curlbuild.h>
#include <curl/easy.h>

#include <sys/stat.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

namespace {
bool exists(const std::string &filename) {
  struct stat buf;
  if (stat(filename.c_str(), &buf) != -1) {
    return true;
  }
  return false;
}
}

namespace tinydm {

Download::Download() : _curl(nullptr) { _curl = curl_easy_init(); }

Download::~Download() { curl_easy_cleanup(_curl); }

std::pair<bool, std::string> Download::download(const std::string &url) {
  const std::string filename = get_filename(url);

  if (exists(filename)) {
    return {false, filename + ": file already exists"};
  }

  std::ofstream out(filename, std::ios::binary);

  curl_easy_setopt(_curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(_curl, CURLOPT_NOSIGNAL, 0);
  curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &out);

  CURLcode res = curl_easy_perform(_curl);

  if (res != CURLE_OK) {
    return {false, filename + ": " + std::string(curl_easy_strerror(res))};
  }

  return {true, filename};
}

std::string Download::get_filename(std::string url) {
  if (url.empty()) {
    return "out";
  }

  // Dont include the trailing '/' in the filename.
  if (url.back() == '/') {
    url = url.substr(0, url.size() - 1);
  }

  auto pos = url.rfind('/');
  if (pos == std::string::npos) {
    return url;
  }

  return unescape(url.substr(pos + 1));
}

std::string Download::unescape(const std::string &url) {
  int length;
  char *unescaped =
      curl_easy_unescape(_curl, url.c_str(), url.length(), &length);
  const std::string str(unescaped, length);
  curl_free(unescaped);
  return str;
}

std::size_t Download::write_callback(char *ptr, std::size_t size,
                                     std::size_t nmemb, void *data) {
  auto out = static_cast<std::ofstream *>(data);
  std::size_t nbytes = size * nmemb;
  out->write(ptr, nbytes);
  return nbytes;
}
}  // namespace tinydm
