#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define BUFF_SIZE 512

int
main(int argc, char *argv[])
{

  char buff[BUFF_SIZE];
  int fds, fdd, n;

  if (argc != 3) {
    fprintf(2, "cp: bad arguments\n");
    exit(1);
  }

  if ((fds = open(argv[1], O_RDONLY)) < 0) {
    fprintf(2, "cp : open() failed on %s\n", argv[1]);
    exit(1);
  }

  if ((fdd = open(argv[2], O_CREATE | O_WRONLY | O_TRUNC)) < 0) {
    fprintf(2, "cp : open() failed on %s\n", argv[2]);
    close(fds);
    exit(1);
  }

  while ((n = read(fds, buff, sizeof(buff))) > 0) {
    if (write(fdd, buff, n) != n) {
      fprintf(2, "write() failed on %s\n", argv[2]);
      close(fds);
      close(fdd);
      exit(1);
    }
  }

  if (n < 0) {
    fprintf(2, "read() failed on %s\n", argv[1]);
    close(fds);
    close(fdd);
    exit(1);
  }

  close(fds);
  close(fdd);

  exit(0);
}
