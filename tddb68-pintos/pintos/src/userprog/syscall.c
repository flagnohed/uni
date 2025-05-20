
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "threads/init.h"
#include "userprog/syscall.h"
#include "filesys/filesys.h"
#include "devices/input.h"
#include "filesys/file.h"
#include "userprog/process.h"
#include "userprog/pagedir.h"


static void syscall_handler (struct intr_frame *);

void
syscall_init (void)
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}


// Check if it's a valid pointer, exit if not.
void is_pointer_valid(void* p){
  if (p >= PHYS_BASE || p == NULL || pagedir_get_page(thread_current()->pagedir, p) == NULL){
    exit(-1);
  }
}

// Check if its a valid string, i.e. check that the pointer to each character
// is valid.
void is_string_valid(char* s){

  is_pointer_valid(s);
  while (*s != '\0'){
    is_pointer_valid(s);
    s++;
  }
}

// Check every possible pointer to the buffer
void is_buffer_valid(void* b,unsigned size){
  for(unsigned i = 0; i<size; i++){
    is_pointer_valid(b);
    }

}

// Checks if the fd is valid. (Lab6)
bool is_fd_valid(int fd){
  return fd >= 2 && fd < 130;
}

void seek(int fd, unsigned position){

    if (is_fd_valid(fd)) {
      struct file* f = thread_current()->files[fd-2];
      if (f != NULL){
        if (position > file_length(f)) position = file_length(f);
        file_seek(f, position);

      }
    }
}

int tell(int fd){

  struct file* our_file = NULL;
  if (is_fd_valid(fd)){
    struct thread* our_thread = thread_current();
    our_file = our_thread->files[fd-2];
  }
  if (our_file != NULL) {
    return file_tell(our_file);
  }
   else {
     return -1;
   }
}

unsigned filesize(int fd){

  struct file* our_file = NULL;
  if (is_fd_valid(fd)){
    struct thread* our_thread = thread_current();
    our_file = our_thread->files[fd-2];
  }
  if (our_file != NULL) {
    return file_length(our_file);
  } else {
    return -1;
  }

}

bool remove(const char *file_name){
  return filesys_remove(file_name);
}

void halt(void){
  power_off();
}

bool create (const char *file, unsigned initial_size){
  return filesys_create(file, initial_size);
}

int open(const char *file){
  struct file *our_file = filesys_open(file);
  if (our_file == NULL){ return -1; }
  struct thread *our_thread = thread_current();

  for (int i = 0; i < 128; i++){  //starts at 2, can have maximum 128
    if (our_thread->files[i] == NULL){
      our_thread->files[i] = our_file;
      return i+2;
    }
  }
  return -1;
}

void close(int fd){
  struct file* our_file = NULL;
  if (is_fd_valid(fd)){
    struct thread* our_thread = thread_current();
    our_file = our_thread->files[fd-2];
  }
  if (our_file != NULL){
    struct thread* our_thread = thread_current();
    file_close(our_thread->files[fd-2]);
    our_thread->files[fd-2] = NULL;
  }

}

unsigned read(int fd, void *buffer, unsigned size){
  struct file* our_file = NULL;
  if (is_fd_valid(fd)){
    struct thread* our_thread = thread_current();
    our_file = our_thread->files[fd-2];
    if (our_file == NULL ){
      return -1;
    } else {
      return file_read(our_file, buffer, size);
    }
  }

  if (fd == 0){
    for (int i = 0; i < size; i++){
      *((char*)buffer) = input_getc();
    }
    return size;
  }
}

unsigned write(int fd, const void *buffer, unsigned size){

  struct file* our_file = NULL;
  if (is_fd_valid(fd)){
    struct thread* our_thread = thread_current();
    our_file = our_thread->files[fd-2];
  }
  if (fd == 1){
    putbuf(buffer, size);
    return size;
  } else if (our_file != NULL){
    return file_write(our_file, buffer, size);
  }

}

tid_t exec (const char* cmdline){
  return process_execute(cmdline);
}

void exit (int status){
  struct parent_child* pc = thread_current()->pc;
  if (pc != NULL){
    pc->exit_status = status;
  }

  printf("%s: exit(%d)\n", thread_current()->name, status);

  thread_exit();

}
int wait(tid_t pid){

  return process_wait(pid);
}

static void
syscall_handler (struct intr_frame *f UNUSED)
{
  is_pointer_valid(f->esp);


  int sysnum = *((int*)f->esp);
  switch (sysnum) {
    case SYS_HALT:
    {
      halt();
      break;
    }
    case SYS_EXIT:
    {
      is_pointer_valid(f->esp+4);
      int t = *((int*)(f->esp+4));
      exit(t);
      break;
    }
    case SYS_CREATE:
    {
      is_pointer_valid(f->esp+4);
      char *new_file = *((char**) (f->esp+4));

      is_string_valid(new_file);
      is_pointer_valid(f->esp+8);
      unsigned size = *((unsigned*) (f->esp+8));

      f->eax = create(new_file, size);
      break;
    }
    case SYS_OPEN:
    {
      is_pointer_valid(f->esp+4);
      char *file = *((char**) (f->esp+4));

      is_string_valid(file);

      f->eax = open(file);
      break;
    }
    case SYS_READ:
    {
      is_pointer_valid(f->esp+4);
      int fd = *((int*) (f->esp+4));

      is_pointer_valid(f->esp+8);
      void *buffer = *((void**) (f->esp+8));

      is_pointer_valid(f->esp+12);
      unsigned size = *((unsigned*) (f->esp+12));

      is_buffer_valid(buffer, size);
      f->eax = read(fd, buffer, size);
      break;
    }
    case SYS_WRITE:
    {
      is_pointer_valid(f->esp+4);
      int fd = *((int*) (f->esp+4));

      is_pointer_valid(f->esp+8);
      void *buffer = *((void**) (f->esp+8));

      is_pointer_valid(f->esp+12);
      unsigned size = *((unsigned*) (f->esp+12));

      is_buffer_valid(buffer, size);
      f->eax = write(fd, buffer, size);
      break;
    }
    case SYS_CLOSE:
    {
      is_pointer_valid(f->esp+4);
      int fd = *((int*) (f->esp+4));

      close(fd);
      break;
    }
    case SYS_EXEC:
    {

      is_pointer_valid(f->esp+4);
      char* cmd = *(char**)(f->esp+4);

      is_string_valid(cmd);
      f->eax = exec(cmd);

      break;
    }
    case SYS_WAIT:
    {
      is_pointer_valid(f->esp+4);
      tid_t t = *((tid_t*)f->esp+4);

      f->eax = wait(t);
      break;
    }
    case SYS_FILESIZE:
    {
      is_pointer_valid(f->esp+4);
      int fd = *((int*) f->esp+4);

      f->eax = filesize(fd);

      break;
    }
    case SYS_TELL:
    {
      is_pointer_valid(f->esp+4);
      int fd = *((int*) f->esp+4);
      f->eax = tell(fd);
      break;
    }
    case SYS_SEEK:
    {
      is_pointer_valid(f->esp+4);
      int fd = *((int*)(f->esp+4));
      is_pointer_valid(f->esp+8);
      unsigned position = *((unsigned*)(f->esp+8));
      seek(fd, position);
      break;
    }
    case SYS_REMOVE:
    {
      is_pointer_valid(f->esp+4);
      char* file_name = *((char**) f->esp+4);
      is_string_valid(file_name);
      f->eax = remove(file_name);
      break;
    }
  }
}
