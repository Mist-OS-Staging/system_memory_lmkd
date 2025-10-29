#ifndef _SLMK_H
#define _SLMK_H

#include <atomic>
#include <pthread.h>
#include <sys/types.h>
#include <vector>
#include <limits.h>

#define SLMK_MAX_VICTIMS 1024

struct proc_info {
    pid_t pid;
    int adj;
    unsigned long rss; 
    uid_t uid;
    unsigned long anon_pages;
};

class SimpleLmk {
public:
    SimpleLmk()
        : reclaim_pending_(false),
          comm_fd_(-1),
          thread_pool_(nullptr),
          thread_cnt_(0) {}
    ~SimpleLmk() {
        if (thread_pool_) {
            delete[] thread_pool_;
        }
    }

    bool init(int comm_fd);
    void set_reaper(class Reaper* reaper);
    void scan_and_kill();

    std::atomic<bool> reclaim_pending_;
    int comm_fd_;

private:
    pthread_t* thread_pool_;
    int thread_cnt_;
};

void* slmk_main(void* param);

#endif // _SLMK_H
