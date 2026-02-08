#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 128

struct header {
  uint64_t size;
  struct header *next;
};

void handle_error(const char *msg) {
  perror(msg);
  exit(1);
}

void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len =
      snprintf(buf, BUF_SIZE, format,
               data_size == sizeof(uint64_t) ? (void *)(*(uint64_t *)data)
                                             : *(void **)data);
  if (len < 0) {
    handle_error("snprintf");
  }
  write(STDOUT_FILENO, buf, len);
}

int main() {
  void *start = sbrk(256);
  if (start == (void *)-1) {
    handle_error("sbrk failed");
  }

  struct header *block1 = (struct header *)start;
  struct header *block2 = (struct header *)((char *)start + 128);
  block1->size = 128;
  block1->next = NULL;
  block2->size = 128;
  block2->next = block1;

  char *data1 = (char *)block1 + sizeof(struct header);
  char *data2 = (char *)block2 + sizeof(struct header);

  int data_size = 128 - sizeof(struct header);
  memset(data1, 0, data_size);
  memset(data2, 1, data_size);

  print_out("first block:         %p\n", &block1, sizeof(void *));
  print_out("second block:        %p\n", &block2, sizeof(void *));

  print_out("first block size:    %lu\n", &block1->size, sizeof(uint64_t));
  print_out("first block next:    %p\n", &block1->next, sizeof(void *));

  print_out("second block size: %lu\n", &block2->size, sizeof(uint64_t));
  print_out("second block next: %p\n", &block2->next, sizeof(void *));
  for (int i = 0; i < data_size; i++) {
    write(STDOUT_FILENO, "0\n", 2);
  }
  for (int i = 0; i < data_size; i++) {
    write(STDOUT_FILENO, "1\n", 2);
  }
  return 0;
}
