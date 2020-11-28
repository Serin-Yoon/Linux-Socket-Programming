#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1460

void error_handling(char* message);

int main(int argc, char* argv[]) {

	int sock; // 소켓
	struct sockaddr_in serv_addr;
	FILE* fp;
	char buf[BUF_SIZE];
	int read_cnt;
	char message_to_server[] = "Hello Server!";
	char thanks_to_server[30];

	char message[] = "error"; // 에러 발생시
	if (argc != 3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	/* fopen 함수 - 전송받고 저장할 파일 열기 (생성) */
	fp = fopen("./Client/movie", "wb");

	/* socket 함수 - 소켓 생성 */
	sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr)); // serv_addr 크기만큼 0 으로 채움 (초기화)
	serv_addr.sin_family = AF_INET; // 주소 체계
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]); // IPv4 주소 저장
	serv_addr.sin_port = htons(atoi(argv[2])); // Port 저장

	/* connect 함수 - 연결 요청 */
	connect(sock, (struct sockaddr*) & serv_addr, sizeof(serv_addr));

	if (connect(sock, (struct sockaddr*) & serv_addr, sizeof(serv_addr)) == 1)	error_handling("connect() error!");
	// connect 함수 -1 이나 0 반환. -1 반환시 실패(에러 핸들링)
		
	/* write 함수 - Server 에게 메시지 전송 */

	write(sock, message_to_server, sizeof(message_to_server));
	
	/* 본격적으로 영화 파일 전송받음 */
	while ((read_cnt = read(sock, buf, BUF_SIZE)) != 0) fwrite((void*)buf, 1, read_cnt, fp);
	puts(" Downloaded a movie!");

	/* write 함수 - Server 에게 감사 메시지 전송 */
	printf(" Send a message to server: ");
	scanf(" %s", thanks_to_server);
	write(sock, thanks_to_server, sizeof(thanks_to_server));
	
	/* fclose 함수 - 파일 닫기 */
	fclose(fp);
	
	/* close 함수 - 소켓 해제 */
	close(sock);
	
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}