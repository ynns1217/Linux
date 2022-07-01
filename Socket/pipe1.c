#include <stdio.h>
#include<unistd.h>
#define BUF_SIZE 30

int main(int argc, char *argv[])
{
	int fds[2];
	char str1[]="Who are you?";
	char buf[BUF_SIZE];
	pid_t pid;

	pipe(fds);
	pid = fork();
	if(pid ==0)
	{
		write(fds[1],str1, sizeof(str1));
	}
	else
	{
		read(fds[0],buf,BUF_SIZE);
		puts(buf);
	}
	

	return 0;
}
