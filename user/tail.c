#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char c;
  int l_offset, l_count, real_count, rv, n, fd;

  if (argc != 3) {
    fprintf(2, "head: bad arguments list\n");
    exit(1);
  }

  n = atoi(argv[2]);

  if (n == 0) {
    fprintf(2, "tail: bad value of n\n");
    exit(1);
  }

  fd = open(argv[1], O_RDONLY);

  if (fd < 0) {
    fprintf(2, "tail: cannot open %s\n", argv[1]);
    exit(1);
  }

  char **lines = (char **)malloc(sizeof(char *) * n);

  for (int i = 0; i < n; i++)
    lines[i] = 0;

  char *line = (char *)malloc(512);
  l_offset = 0;
  l_count = 0;
  real_count = 0;

  do {
    if ((rv = read(fd, &c, 1)) > 0) {
      line[l_offset++] = c;

      if (c == '\n') {
        if (lines[l_count] != 0)
          free(lines[l_count]);
        lines[l_count] = line;
        l_count = (l_count + 1) % n;
        l_offset = 0;
        line = (char *)malloc(512);
        real_count++;
      }
    }

    if (rv < 0) {
      fprintf(2, "head: read error\n");
      exit(1);
    }

  } while (rv != 0);

  if (l_offset != 0) {
    line[l_offset] = '\n';
    lines[l_count] = line;
    l_count = (1 + l_count) % n;
    real_count++;
  }

  n = (real_count < n) ? real_count : n;

  for (int i = 0; i < n; i++) {
    fprintf(1, "%s", lines[(l_count + i) % n]);
  }

  close(fd);

  exit(0);
}
