#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

// 2.  Synchronization approach
int
main(int argc, char* argv[])
{
        int file_count, cpid;
	int pipefd[2];

        if(argc < 3)
        {
                fprintf(2,"mgrep: bad argument\n");
                exit(1);
        }

        file_count = argc - 2;


	int childproc[file_count][3];

        for(int i=0; i<file_count; i++)
        {

		if(pipe(pipefd) == -1 )
		{
			fprintf(2,"mgrep: pipe failed\n");
			exit(1);
		}
		
		childproc[i][1] = pipefd[0];
		childproc[i][2] = pipefd[1];

                cpid = fork();

                if(cpid == -1)
                {
                        fprintf(2,"mgrep : fork() failed\n");
                        exit(1);
                }

		if(cpid !=0 )
		{
			childproc[i][0] = cpid;
			close(childproc[i][2]);
		}

                if( cpid == 0 )
                {
			close(childproc[i][1]);
			close(1);
			if(dup(childproc[i][2]) == -1)
			{
				fprintf(2, "mgrep: dup() failed\n");
				close(childproc[i][2]);
				exit(1);
			}

                        fprintf(1, "(Worker PID: %d)\n",(int)getpid());

                        char *nargs[3];
                        nargs[0] = "grep";
                        nargs[1] = argv[1];
                        nargs[2] = argv[2+i];

                        exec("grep",nargs);

                        fprintf(2, "mgrep : exec() failed\n");
                        exit(1);
                }

        }
	

	

	char buffer[512];
	int n;
	for(int i=0; i<file_count; i++)
	{
		while((n=read(childproc[i][1],buffer,sizeof(buffer)))>0)
		{
			if(write(1,buffer,n)!=n)
			{
				fprintf(2,"mgrep: write error\n");
				exit(1);
			}
		}

		if(n < 0)
		{
			fprintf(2, "mgrep : read() error\n");
			exit(1);
		}
		close(childproc[i][1]);
	}

        exit(0);
}
// 1.  Sequential approach

/*

int
main(int argc, char* argv[])
{
	int file_count, cpid, status;
	
	if(argc < 3)
	{
		fprintf(2,"mgrep: bad argument\n");
		exit(1);
	}

	file_count = argc - 2;

	for(int i=0; i<file_count; i++)
	{
		cpid = fork();

		if(cpid == -1)
		{
			fprintf(2,"mgrep : fork() failed\n");
			exit(1);
		}
		 
		if( cpid == 0 )
		{
			fprintf(1, "(Worker PID: %d)\n",(int)getpid());

			char *nargs[3];
			nargs[0] = "grep";
			nargs[1] = argv[1];
			nargs[2] = argv[2+i];

			exec("grep",nargs);
			
			fprintf(2, "mgrep : exec() failed\n");
			exit(1);
		}

		wait(&status);
		
		if(status==0)
			fprintf(1, "Worker PID: %d terminated successfully...\n\n",cpid);
		else	
			fprintf(1, "Woker PID: %d failed, exit CODE: %d\n\n",cpid,status);
	}

	exit(0);
}


*/	
