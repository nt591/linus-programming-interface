#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define BUF_SIZE 1024

// takes two arguments, the first being a source file
// and the second a destination file.
// Overwrites the destination file
// Copies all content from source to destination

int main(int argc, char* argv[]) {
  int src_fd, dest_fd, n, write_len;
  char buf[BUF_SIZE];

  if (argc != 3) {
    fprintf(stderr, "Expected 2 arguments to `cp`");
    return 1;
  }

  if ((src_fd = open(argv[1], O_RDONLY)) == -1) {
    fprintf(stderr, "Failed to open source");
    return 1;
  }

  if ((dest_fd = open(argv[2],
      O_WRONLY | O_CREAT | O_TRUNC,
      S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH | S_IWOTH))== -1 ) {
    fprintf(stderr, "Failed to open destination");
    return 1;
  }

  // keep reading from source and write to destination
  while ((n = read(src_fd, buf, BUF_SIZE)) > 0) {
    write_len = write(dest_fd, buf, n);
    if (write_len != n) {
      fprintf(stderr, "Write failure");
      return 1;
    }
  }
  if (n < 0) {
    fprintf(stderr, "Write failure");
    return 1;
  }

  return 0;
}
