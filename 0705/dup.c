#include <stdio.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
	int cfd1, cfd2;
	char str1[] = "Hi ! \n";
	char str2[] = "It's nice day! \n";

//dup 함수호출을 통해서 파일 디스크립터 1을 복사하고
//dup2 함수호출을 통해서 복사한 파일 디스크립터를 재 복사하고 있다. 정수값도 7로 지정
	cfd1 = dup(1);
	cfd2 = dup2(cfd1,7);

	printf("fd1 = %d, fd2 = %d \n", cfd1, cfd2);
//복사된 파일 디스크립터를 이용해서 출력
//출력 결과를 통해 복사가 이뤄진것을 확인
	write (cfd1, str1, sizeof(str1));
	write (cfd2, str2, sizeof(str2));

//복사된 파이 디스크립터를 모두 종료
//그러나 아직 하나가 남아있는 상태 때문에 출력이 여전히 이뤄
	close (cfd1);
	close(cfd2);
	write(1, str1, sizeof(str1));
//마지막 파이 ㄹ디스크립터 종료  -> 마지막 출력은 이뤄지지않는다.
	close(1);
	write(1,str2,sizeof(str2));

	return 0;
}
