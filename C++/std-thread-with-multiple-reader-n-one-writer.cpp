/**
 * Copyright © 2023 Chee Bin HOH. All rights reserved.
 */

#include <array>
#include <chrono>
#include <condition_variable>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

using std::chrono::system_clock;

void read_func(int seconds) {
  long count{0};
  int mins{seconds / 60};

  seconds = {seconds % 60};

  std::time_t tt = system_clock::to_time_t(system_clock::now());
  struct std::tm *ptm = std::localtime(&tt);

  ptm->tm_min += mins;
  ptm->tm_sec += seconds;

  std::time_t next_tt = mktime(ptm);
  tt = system_clock::to_time_t(system_clock::now());
  while (tt < next_tt) {
    std::this_thread::yield();
    count++;
    tt = system_clock::to_time_t(system_clock::now());
  }

  // context switching between outputing of different portions of the following
  // statements, we need to lock the output or ww should pipe it to some sort of
  // in memory output string list that is atomic write.
  std::cout << std::this_thread::get_id() << ", read count = " << count << "\n";
}

void write_func(int seconds) {
  long count{0};
  int mins{seconds / 60};

  seconds = {seconds % 60};

  std::time_t tt = system_clock::to_time_t(system_clock::now());
  struct std::tm *ptm = std::localtime(&tt);

  ptm->tm_min += mins;
  ptm->tm_sec += seconds;

  std::time_t next_tt = mktime(ptm);
  tt = system_clock::to_time_t(system_clock::now());
  while (tt < next_tt) {
    std::this_thread::yield();
    count++;
    tt = system_clock::to_time_t(system_clock::now());
  }

  std::cout << std::this_thread::get_id() << ", write count = " << count
            << "\n";
}

int main(int argc, char *argv[]) {
  std::array<std::thread, 5> readers;

  for (int i = 0; i < readers.size(); i++) {
    readers[i] = std::thread(read_func, 5);
  }

  write_func(10);

  for (auto &th : readers) {
    th.join();
  }

  return 0;
}