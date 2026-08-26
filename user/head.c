#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"



int
main(int argc, char* argv[])
{
	char c;
	int n, fd;

	if(argc != 3)
	{
		fprintf(2,"head: bad arguments list\n");
		exit(1);
	}
	
	n = atoi(argv[2]);

	if(n == 0)
	{
		fprintf(2,"head: bad value of n\n");
		exit(1);
	}

	fd = open(argv[1],O_RDONLY);

	if( fd < 0 )
	{
		fprintf(2, "head: cannot open %s\n", argv[1]);
      		exit(1);
	}

	do
	{
		int rv;
		if((rv=read(fd,&c,1))>0)
		{
			if(write(1,&c,1)!=1)
			{
				fprintf(2,"head: write error\n");
				exit(1);
			}
		}
		
		if(rv == 0)
			break;
		
		if (rv < 0)
		{
			fprintf(2, "head: read error\n");
			exit(1);
		}
		
		if(c == '\n')
			n--;
	}while(n);
	
	close(fd);
	
	exit(0);
  }
