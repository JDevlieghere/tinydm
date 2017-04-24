#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

namespace tinydm {
class Download;
class DownloadManager {
 public:
  DownloadManager(std::size_t threads);
  ~DownloadManager();

  void download(std::string url);

 private:
  std::queue<std::string> _urls;

  std::vector<std::thread> _workers;
  std::vector<Download*> _downloads;

  std::mutex _mutex;
  std::mutex _io_mutex;
  std::condition_variable _condition;

  bool _stop;
};
}
