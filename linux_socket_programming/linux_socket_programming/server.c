#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1460

void error_handling(char* message);

int main(int argc, char* argv[]) {

	int serv_sock; // Server 소켓
	int clnt_sock; // Client 소켓
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	FILE* fp;
	char buf[BUF_SIZE];
	int read_cnt;
	char message_from_client[30]; // 클라이언트->서버
	char message[] = "error"; // 에러 발생시
	
	int str_len;
	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0
		]);
		exit(1);
	}

	/* fopen 함수 - 전송할 파일 열기 */
	fp = fopen("./Server/movie.mp4", "rb");
	// 옵션 rb = 바이너리 형식으로 파일 읽음

	/* socket 함수 - 소켓 생성 */
	serv_sock = socket(PF_INET, SOCK_STREAM
		, 0);
	memset(&serv_addr, 0, sizeof(serv_addr)
	);
	// serv_addr 크기만큼 0 으로 채움 (초기화)

	serv_addr.sin_family = AF_INET; // 주소 체계
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // IPv4 주소 저장
	serv_addr.sin_port = htons(atoi(argv[1]
	)); // Port 저장

	/* bind 함수 - IP 주소, Port 할당 */
	//bind(serv_sock, (struct sockaddr*) &	serv_addr, sizeof(serv_addr));
	if (bind(serv_sock, (struct sockaddr*)
		& serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");
	// bind 함수 - 1 이나 0 반환. -	1 반환시 실패(에러 핸들링)
	
	/* listen 함수 - 소켓 연결 요청 받아들일 수 있도록 함 */
		listen(serv_sock, 5);
	if (listen(serv_sock, 5) == -1) {
		error_handling("listen() error");
	}
	// listen 함수 -1 이나 0 반환. -1 반환시 실패(에러 핸들링)

	/* accpet 함수 - 연결 요청 수락 */
	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*) & clnt_addr, &clnt_addr_size);

	if (clnt_sock == -1) {
		error_handling("accept() error");
	}
	// accept 함수 -1 이나 0 반환. -1 반환시 실패(에러 핸들링)

	/* read 함수 - Client 로부터 온 메시지 확인 */
	str_len = read(clnt_sock, message_from_client, sizeof(message_from_client) -1);
	if (str_len == -1)
		error_handling("read() error!");
	// read 함수 -1 이나 0 반환. -1 반환시 실패(에러 핸들링)
	printf("Message from client: %s \n", message_from_client);

	/* 본격적으로 영화 파일 전송 */
	while (1) {
		read_cnt = fread((void*)buf, 1, BUF
			_SIZE, fp);
		if (read_cnt < BUF_SIZE) {
			write(clnt_sock, buf, read_cnt)
				;
			break;		}
		write(clnt_sock, buf, BUF_SIZE);
	}

	/* shutdown 함수 - 전송 퍼버 닫음 */
	shutdown(clnt_sock, SHUT_WR);

	/* read 함수 - Client 로부터 온 감사 메시지 확인 */
	read(clnt_sock, buf, BUF_SIZE);
	printf("Message from client: %s \n", buf);

	/* fclose 함수 - 파일 닫기 */
	fclose(fp);

	/* close 함수 - 소켓 해제 */
	close(clnt_sock);
	close(serv_sock);

	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}