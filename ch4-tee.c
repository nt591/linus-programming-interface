#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>

#define BUF_SIZE 1024
// reads stdin, writes to stdout and an output file
int main(int argc, char* argv[]) {
  int fd, n, out_b, fd_b, append;
  int opt;
  char buf[BUF_SIZE + 1];

  if (argc < 2) {
    printf("incorrect number of arguments\n");
    fflush(stdout);
    return 1;
  }

  while ((opt = getopt(argc, argv, "a")) != -1) {
    switch(opt) {
    case 'a':
      append = 1;
      break;
    default:
      printf("Incorrect usage of arguments");
      return 1;
    }
  }

  if (optind >= argc) {
    // should have an argument after the option flag
    printf("Expected argument after CLI flag");
    return 1;
  }

  int flags = O_WRONLY | O_CREAT;
  flags |= (append ? O_APPEND : O_TRUNC);
  fd = open(argv[optind], flags,
                S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH | S_IWOTH
                );
  if (fd < 0) {
    printf("failed to open file");
    fflush(stdout);
    return 1;
  }

  while ((n = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
    buf[n] = '\0';
    out_b = write(STDOUT_FILENO, buf, n);
    if (out_b != n) {
      printf("stdout write fail\n");
      fflush(stdout);
      return 1;
    }

    fd_b = write(fd, buf, n);
    if (fd_b != n) {
      printf("file write fail\n");
      fflush(stdout);
      return 1;
    }
  }

  if (n < 0) {
    printf("stdin read fail\n");
    fflush(stdout);
    return 1;
  }

  return 0;
}
