#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int sock;
	int  pid;
	sock = socket(AF_INET,SOCK_STREAM,0);//소켓생성
	pid =fork(); 	//자식 프로세스 생성(프로세서복사)-> 부모프로세스의 pid에는 자식프로세스의 ID가 저장, 자식프로세스 pid는 0dlwjwkd

	if(pid == 0)//자식 프로세스면
	{
		printf("parent sock : %d\n",sock);
	}
	else
	{
		printf("chil sock: %d\n",sock);
	}
return 0;	
}
