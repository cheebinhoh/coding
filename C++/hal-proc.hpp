#ifndef HAL_PROC_H_HPP_HAVE_SEEN

#define HAL_PROC_H_HPP_HAVE_SEEN

#include <functional>
#include <string>

#include <pthread.h>

class Hal_Proc
{
  public:
    Hal_Proc(std::string name, std::function<void()> fn = {});
    ~Hal_Proc();
    bool exec(std::function<void()> fn = {});

    static void * thread_fn_helper(void *context);

  protected:
    void run_exec(std::function<void()> fn);

  private:
    std::function<void()> m_fn {};
    std::string           m_name {};
    pthread_t             m_th {};
};

#endif /* HAL_PROC_H_HPP_HAVE_SEEN */