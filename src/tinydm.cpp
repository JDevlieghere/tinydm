#include "DownloadManager.h"

#include <cstdio>
#include <iostream>
#include <string>

using namespace tinydm;

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Usage: tinydm [threads]" << std::endl;
    return EXIT_FAILURE;
  }

  int threads = std::atoi(argv[1]);
  if (threads <= 0) {
    std::cerr << "Invalid number of threads specified" << std::endl;
    return EXIT_FAILURE;
  }

  DownloadManager dm(static_cast<std::size_t>(threads));

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) break;
    dm.download(line);
  }

  return EXIT_SUCCESS;
}
