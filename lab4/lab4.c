#define _DEFAULT_SOURCE

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF_SIZE 128
struct header {
  uint64_t size;
  struct header *next;
};

void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len = snprintf(buf, BUF_SIZE, format,
                         data_size == sizeof(uint64_t) ? *(uint64_t *)data
                                                       : *(void **)data);
  if (len < 0) {
    handle_error("snprintf");
  }
  write(STDOUT_FILENO, buf, len);
}

int main(void) {
  uint64_t sizeAllocated = 256;
  void *block = sbrk(sizeAllocated);

  struct header *header1 = (struct header *)block;
  header1->size = 128;
  header1->next = block + 128;

  char *byte = block + sizeof(struct header);
  uint64_t dataSize = 128 - sizeof(struct header);
  for (int i = 0; i < dataSize; i++) {
    *byte = 0;
    byte++;
  }
  struct header *header2 = header1->next;
  header2->size = 128;
  header2->next = NULL;

  byte = (void *)header2 * sizeof(struct header);
  dataSize = 128 - sizeof(struct header);

  for (int i = 0; i < dataSize; i++) {
    *byte = 1;
    byte++;
  }

  print_out("first block:   %p\n", &header1, sizeof(&header1));
  print_out("second block:   %p\n", &header2, sizeof(&header2));

  print_out("first block size:  %lu\n", &header1->size, sizeof(header1->size));
  print_out("first block next:  %p\n", &header1->next, sizeof(header1->next));

  print_out("second block size: %lu\n", &header2->size, sizeof(header2->size));
  print_out("second block next: %p\n", &header2->next, sizeof(header2->next));

  byte = (void)header1 + sizeof(struct header);
  for (int i = 0; i < dataSize; i++) {
    print_out("%02X\n", byte, sizeof(byte));
    byte++;
  }
}
