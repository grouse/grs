#ifndef THREAD_GENERATED_H
#define THREAD_GENERATED_H

extern Mutex *create_mutex();
extern void lock_mutex(Mutex *);
extern void unlock_mutex(Mutex *);
extern Thread *create_thread(ThreadProc proc, void *user_data);
extern i32 thread_id();

#endif // THREAD_GENERATED_H

#ifdef THREAD_GENERATED_IMPL
#define THREAD_INTERNAL
#endif
