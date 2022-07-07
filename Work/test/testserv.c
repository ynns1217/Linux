#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define SMALL_BUF 100

void *request_handler(void* arg);
void send_data(FILE* fp,char* ct, char* file_name);
char* content_type(char* file);
void send_error(FILE* fp);
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock,clnt_sock;
	struct sockaddr_in serv_adr,clnt_adr;
	int clnt_adr_size;
	char buf[BUF_SIZE];
	pthread_t t_id;

	//실행파일경로입력으로 받기
	if(argc!=2)
	{
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}

	//Tcp 소켓 생성
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	//서버주소정보 초기화
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	//서버주소정보 기반으로 주소 할당
	if(bind(serv_sock,(struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	//서버소켓(리스닝 소켓) 연결, 대기큐, 생성 클라이언트연결
	if(listen(serv_sock,20) ==-1)
		error_handling("listem() error");

	//요청및 응답
	while(1)
	{
		clnt_adr_size=sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_size);
		printf("Connection Request : %s:%d\n",inet_ntoa(clnt_adr.sin_addr),ntohs(clnt_adr.sin_port));
		pthread_create(&t_id,NULL,request_handler, &clnt_sock); //쓰레드 생성, 실행
		pthread_detach(t_id); //종료된  쓰레드 소소멸
	}

	close(serv_sock);//서버소켓 연결 종료
	return 0;
}

void request_handler(void *arg)
{
int clnt_sock=*((int*)arg);
char req_line[SMALL_BUF];
FILE* clnt_read;
FILE* clnt_write;

char method[10];
char ct[15];
char file_name[30];

//입력분할
clnt_read = fdopen(clnt_sock,"r");
clnt_write = fdopen(dup(clnt_sock),"w");

//클라이언트로부터 data 수신
fgets(req_line, SMALL_BUF,clnt_read);

//http 의한 요청인지 확인
if(strstr(req_line, "HTTP/")==NULL)
{
	send_error(clnt_write);
	fclose(clnt_read);
	fclose(clnt_write);
	return;
}

strcpy(method,strtok(req_line,"/"));
strcpy(file_name,strtok(NULL,"/"));
strcpy(ct,content_type(file_name));

//Get 요청인지 확인
if(strcmp(method,"GET")!=0)
{
	send_error(clnt_write);
	fclose(clnt_read);
	fclose(clnt_write);
	return;
}

//클라이어트에 데이터 파일수신 완료했으므로입력 스트림 종료
fclose(clnt_read);
send_data(clnt_write,ct,file_name); //클라이언트에게 data통신
}

void send_data(FILE* fp,char* ct,char* file_name)
{
	char protocol[] = "HTTP/1.1 200 OK\r\n"
							"SEver:Linux Web Server\r\n"
							"Content-Type: text/html: charset = UTF-8\r\n\r\n"
							"<!DoCTYPE html>\r\n"
							"<html><head><title> MY Web Page </title>\r\n"
							"<style>body {background-color:#FFFF00 }</style><head>\r\n"
							"<body><center><hl>Hello world!!<hl><br>\r\n"
							"img src=\"game.jpg\"></center></body></html>\r\n";
							
	char cnt_type[SMALL_BUF];
	char buf[BUF_SIZE];
	FILE* send_file;
	send_file = fopen(file_name,"r"); //클라이어트가 요청한 파일 연다
	if(send_file ==NULL)
	{
	 send_error(fp);
	 return;
	}

	//헤더 정보 전송
	fputs(protocol,fp);
	
	//요청 데이터 전송
	while(fgets(buf, BUF_SIZE, send_file)!=NULL)
	{
		fputs(buf,fp);
		fflush(fp);
	}
	fflush(fp);
	fclose(fp);//HTTP  응답 후 종료
}

//Content-type
//char* content_type(char* file)
//{
//	char extension[SMALL_BUF];
//	char file_name[SMALL_BUF];
//	strcpy(file_name,file);
//	strtok(file_name,".");
//	strcpy(extension,strtok(NULL,"."));
//	if(!strcmp(extension,"html")||!strcmp(extension,"htm"))
//		return "txt/html";
//	else
//		return "text/plain";
//}

//오류발생시 메시지
void error_handling(char *message)
{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}
