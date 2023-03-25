/**
 * Copyright © 2023 Chee Bin HOH. All rights reserved.
 *
 * This program demonstrates some features of std::thread.
 *
 * The WorkerTask is a task wrapped under an object and
 * added to the WorkerExecution' queue, and WorkerExecution
 * will have certain # of threads that runs and executes the
 * task.
 *
 * We will maintain a multiple producer (adding WorkerTask) and
 * multiple consumer (thread) implementation.
 */

#include <chrono>
#include <condition_variable>
#include <ctime>
#include <deque>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class WorkerTask {
public:
  WorkerTask(
      std::string name,
      void (*fn)(void *) =
          [](void *data) { std::cout << "default noop for task\n"; },
      int sleep = 0)
      : m_name{name}, m_fn{fn}, m_sleep{sleep} {}

  WorkerTask() : WorkerTask{"unknown"} {}

  ~WorkerTask() { std::cout << "Delete workertask: " << m_name << "\n"; }

  WorkerTask(const WorkerTask &copy) = delete;
  WorkerTask &operator=(const WorkerTask &copy) = delete;

private:
  std::string m_name{};
  int m_sleep{};
  void (*m_fn)(void *){};
};

class WorkerExecution;

void doWorkerExecution(WorkerExecution *we);

class WorkerExecution {
public:
  enum State { Initialized, Stop };

  friend void doWorkerExecution(WorkerExecution *we);

  WorkerExecution(int num_workers = 1) : m_num_workers{num_workers} {
    int i;

    m_state = State::Initialized;
    for (i = 0; i < m_num_workers; i++) {
      m_workers.emplace_back(doWorkerExecution, this);
    }
  }

  ~WorkerExecution() {
    int i;

    // we do not use lock_guard here as we want to explicitly unlock
    // to let child threads to be able to run prior join the child
    // threads prior exit the block (which will then unlock if we use
    // lock_guard.
    m_mutex.lock();
    m_state = Stop;
    m_condv.notify_all();
    m_mutex.unlock();

    std::cout << "main thread joins the child thread\n";
    for (i = 0; i < m_num_workers; i++) {
      m_workers[i].join();
    }
  }

  bool addWorkerTask(std::unique_ptr<WorkerTask> t) {
    std::unique_lock<std::mutex> lck(m_mutex);
    m_tasks.push_back(std::move(t));
    m_condv.notify_all();

    std::this_thread::yield();

    return true;
  }

private:
  void func() { std::cout << "calling func\n"; }

  std::deque<std::unique_ptr<WorkerTask>> m_tasks{};
  std::vector<std::thread> m_workers{};
  int m_num_workers{};
  State m_state{};
  std::mutex m_mutex{};
  std::condition_variable m_condv{};
};

void doWorkerExecution(WorkerExecution *we) {
  while (true) {
    std::unique_ptr<WorkerTask> tsk{};

    {
      std::unique_lock<std::mutex> lck(we->m_mutex);

      while (we->m_tasks.empty() &&
             WorkerExecution::State::Stop != we->m_state) {
        we->m_condv.wait(lck);
      }

      if (WorkerExecution::State::Stop == we->m_state) {
        break; // break out of while
      }

      std::cout << "before tasks size = " << we->m_tasks.size() << "\n";
      tsk = std::move(we->m_tasks.front());
      we->m_tasks.pop_front();
      std::cout << "after tasks size = " << we->m_tasks.size() << "\n";
    }

    // do work
    int seconds_to_sleep = (static_cast<unsigned int>(std::rand()) >> 16) % 5;

    std::cout << "child thread (" << std::this_thread::get_id()
              << ") will sleep for " << seconds_to_sleep << " seconds\n";
    std::this_thread::sleep_for(std::chrono::seconds(seconds_to_sleep));
    std::cout << "child thread (" << std::this_thread::get_id()
              << ") resume exeuction\n";
  }
}

int main(int argc, char *argv[]) {
  using std::chrono::system_clock;

  std::time_t tt = system_clock::to_time_t(system_clock::now());
  struct std::tm *ptm = std::localtime(&tt);
  std::cout << "Current time is " << std::put_time(ptm, "%X") << "\n";

  std::srand(std::time(nullptr));

  {
    std::unique_ptr<WorkerTask> t1 = std::make_unique<WorkerTask>(
        "first", [](void *data) { std::cout << "first task"; }, 5);
    std::unique_ptr<WorkerTask> t2 = std::make_unique<WorkerTask>(
        "second", [](void *data) { std::cout << "second task"; }, 3);
    std::unique_ptr<WorkerTask> t3 = std::make_unique<WorkerTask>(
        "third", [](void *data) { std::cout << "third task"; }, 4);
    WorkerExecution exec{2};

    std::cout << "main thread sleeps 3 seconds prior adding first task\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "main thread resumes execution to add first task after 3 "
                 "seconds sleep\n";
    exec.addWorkerTask(std::move(t1));

    std::cout
        << "main thread sleeps 5 seconds prior adding second and third tasks\n";
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "main thread resumes execution to add second and third task "
                 "after 5 seconds sleep\n";
    exec.addWorkerTask(std::move(t2));
    exec.addWorkerTask(std::move(t3));

    std::cout << "main thread sleeps 10 seconds prior exit\n";
    std::this_thread::sleep_for(std::chrono::seconds(10));
    std::cout
        << "main thread resumes execution to exit after 10 seconds sleep\n";
  }

  tt = system_clock::to_time_t(system_clock::now());
  ptm = std::localtime(&tt);
  std::cout << "Current time before end is " << std::put_time(ptm, "%X")
            << "\n";

  return 0;
}