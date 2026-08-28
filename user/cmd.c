#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int cpid, status;

  if (argc == 1) {
    fprintf(2, "cmd: missing arguments\n");
    exit(1);
  }

  cpid = fork();

  if (cpid == -1) {
    fprintf(2, "cmd : fork failed\n");
    exit(1);
  }

  if (cpid == 0) {
    exec(argv[1], argv + 1);
    fprintf(2, "cmd : exec failed on %s\n", argv[1]);
    exit(1);
  }

  wait(&status);

  if (status == 0)
    fprintf(1, "Child process terminated successfully with exit CODE: %d\n",
            status);
  else
    fprintf(1, "Child process failed with exit CODE: %d\n", status);

  exit(0);
}
