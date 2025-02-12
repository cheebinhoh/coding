/**
 * This is a test file for teepipe that two threads are reading data and feed
 * into teepipe that will sort the data and make sure that it is processed
 * in order, if the data already sorted in the source data, the teepipe will
 * coordinate high level sorting between two stream of data.
 */

#include "hal-proc.hpp"
#include "hal-teepipe.hpp"

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  std::vector<std::string> files{"./teepipe-test-data-1.txt",
                                 "./teepipe-test-data-2.txt"};
  Hal_TeePipe<long> tpipe{
      "teepipe", [](long val) { std::cout << val << "\n"; },
      [](std::vector<long> list) { std::sort(list.begin(), list.end()); }};
  std::vector<std::unique_ptr<Hal_Proc>> proclist{};

  for (auto &filename : files) {
    auto tpipeSource = tpipe.addHal_TeePipeSource();
    auto proc = std::make_unique<Hal_Proc>(
        filename, [&tpipe, tpipeSource, filename, prog = argv[0]]() {
          int fd{};
          FILE *file{};
          char buf[BUFSIZ]{};

          fd = open(filename.c_str(), O_RDONLY);
          if (-1 == fd) {
            perror(prog);
            exit(1);
          }

          file = fdopen(fd, "r");
          if (NULL == file) {
            perror(prog);
            exit(1);
          }

          while (fgets(buf, sizeof(buf), file) != NULL) {
            if ('\n' == buf[strlen(buf) - 1]) {
              buf[strlen(buf) - 1] = '\0';
            }

            long val{};
            val = strtol(buf, NULL, 10);

            tpipeSource->write(val);

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
          }

          if (ferror(file)) {
            perror(prog);
            exit(1);
          }

          fclose(file);
        });

    proclist.push_back(std::move(proc));
  }

  for (auto &proc : proclist) {
    proc->exec();
    Hal_Proc::yield();
  }

  Hal_Proc::yield();

  for (auto &proc : proclist) {
    proc->wait();
  }

  tpipe.waitForEmpty();

  return 0;
}
