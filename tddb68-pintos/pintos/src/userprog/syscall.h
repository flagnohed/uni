#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

//#include "filesys/unsigned.h"

void syscall_init (void);

void halt(void);
bool create (const char *file, unsigned initial_size);
int open(const char *file);
void close(int fd);
unsigned read(int fd, void *buffer, unsigned size);
unsigned write(int fd, const void *buffer, unsigned size);
void exit(int status);
tid_t exec(const char* cmdline);
int wait(tid_t pid);
void is_pointer_valid(void* p);
void is_string_valid(char* s);
void is_buffer_valid(void* b,unsigned size);
void seek(int fd, unsigned position);
int tell(int fd);
unsigned filesize(int fd);
bool is_fd_valid(int fd);

#endif /* userprog/syscall.h */
