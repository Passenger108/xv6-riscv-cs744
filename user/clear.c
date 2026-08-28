#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char *buff = "\033[H\033[J";
  write(1, buff, strlen(buff));
  exit(0);
}
