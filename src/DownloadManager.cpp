#include "DownloadManager.h"
#include "Download.h"

#include <iostream>

namespace tinydm {
DownloadManager::DownloadManager(std::size_t threads) : _stop(false) {
  for (size_t i = 0; i < threads; ++i) {
    _downloads.push_back(nullptr);
    _workers.emplace_back([this, i] {
      Download d;
      for (;;) {
        std::string url;

        {
          std::unique_lock<std::mutex> lock(this->_mutex);
          this->_condition.wait(
              lock, [this] { return this->_stop || !this->_urls.empty(); });
          if (this->_stop && this->_urls.empty()) return;
          url = std::move(this->_urls.front());
          this->_urls.pop();
        }

        bool success;
        std::string result;
        std::tie(success, result) = d.download(url);

        {
          std::unique_lock<std::mutex> lock(this->_io_mutex);
          if (success) {
            std::cout << "Finished downloading " << result << std::endl;
          } else {
            std::cerr << "Failed to download " << result << std::endl;
          }
        }
      }
    });
  }
}

DownloadManager::~DownloadManager() {
  {
    std::unique_lock<std::mutex> lock(_mutex);
    _stop = true;
  }
  _condition.notify_all();
  for (std::thread& worker : _workers) worker.join();
}

void DownloadManager::download(std::string url) {
  _urls.push(std::move(url));
  _condition.notify_one();
}
}
