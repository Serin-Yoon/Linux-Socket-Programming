#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1460

void error_handling(char* message);

int main(int argc, char* argv[]) {

	int sock; // ����
	struct sockaddr_in serv_addr;
	FILE* fp;
	char buf[BUF_SIZE];
	int read_cnt;
	char message_to_server[] = "Hello Server!";
	char thanks_to_server[30];

	char message[] = "error"; // ���� �߻���
	if (argc != 3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	/* fopen �Լ� - ���۹ް� ������ ���� ���� (����) */
	fp = fopen("./Client/movie", "wb");

	/* socket �Լ� - ���� ���� */
	sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr)); // serv_addr ũ�⸸ŭ 0 ���� ä�� (�ʱ�ȭ)
	serv_addr.sin_family = AF_INET; // �ּ� ü��
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]); // IPv4 �ּ� ����
	serv_addr.sin_port = htons(atoi(argv[2])); // Port ����

	/* connect �Լ� - ���� ��û */
	connect(sock, (struct sockaddr*) & serv_addr, sizeof(serv_addr));

	if (connect(sock, (struct sockaddr*) & serv_addr, sizeof(serv_addr)) == 1)	error_handling("connect() error!");
	// connect �Լ� -1 �̳� 0 ��ȯ. -1 ��ȯ�� ����(���� �ڵ鸵)
		
	/* write �Լ� - Server ���� �޽��� ���� */

	write(sock, message_to_server, sizeof(message_to_server));
	
	/* ���������� ��ȭ ���� ���۹��� */
	while ((read_cnt = read(sock, buf, BUF_SIZE)) != 0) fwrite((void*)buf, 1, read_cnt, fp);
	puts(" Downloaded a movie!");

	/* write �Լ� - Server ���� ���� �޽��� ���� */
	printf(" Send a message to server: ");
	scanf(" %s", thanks_to_server);
	write(sock, thanks_to_server, sizeof(thanks_to_server));
	
	/* fclose �Լ� - ���� �ݱ� */
	fclose(fp);
	
	/* close �Լ� - ���� ���� */
	close(sock);
	
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}