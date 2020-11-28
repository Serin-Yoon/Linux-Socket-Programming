#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1460

void error_handling(char* message);

int main(int argc, char* argv[]) {

	int serv_sock; // Server ����
	int clnt_sock; // Client ����
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	FILE* fp;
	char buf[BUF_SIZE];
	int read_cnt;
	char message_from_client[30]; // Ŭ���̾�Ʈ->����
	char message[] = "error"; // ���� �߻���
	
	int str_len;
	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0
		]);
		exit(1);
	}

	/* fopen �Լ� - ������ ���� ���� */
	fp = fopen("./Server/movie.mp4", "rb");
	// �ɼ� rb = ���̳ʸ� �������� ���� ����

	/* socket �Լ� - ���� ���� */
	serv_sock = socket(PF_INET, SOCK_STREAM
		, 0);
	memset(&serv_addr, 0, sizeof(serv_addr)
	);
	// serv_addr ũ�⸸ŭ 0 ���� ä�� (�ʱ�ȭ)

	serv_addr.sin_family = AF_INET; // �ּ� ü��
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // IPv4 �ּ� ����
	serv_addr.sin_port = htons(atoi(argv[1]
	)); // Port ����

	/* bind �Լ� - IP �ּ�, Port �Ҵ� */
	//bind(serv_sock, (struct sockaddr*) &	serv_addr, sizeof(serv_addr));
	if (bind(serv_sock, (struct sockaddr*)
		& serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");
	// bind �Լ� - 1 �̳� 0 ��ȯ. -	1 ��ȯ�� ����(���� �ڵ鸵)
	
	/* listen �Լ� - ���� ���� ��û �޾Ƶ��� �� �ֵ��� �� */
		listen(serv_sock, 5);
	if (listen(serv_sock, 5) == -1) {
		error_handling("listen() error");
	}
	// listen �Լ� -1 �̳� 0 ��ȯ. -1 ��ȯ�� ����(���� �ڵ鸵)

	/* accpet �Լ� - ���� ��û ���� */
	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*) & clnt_addr, &clnt_addr_size);

	if (clnt_sock == -1) {
		error_handling("accept() error");
	}
	// accept �Լ� -1 �̳� 0 ��ȯ. -1 ��ȯ�� ����(���� �ڵ鸵)

	/* read �Լ� - Client �κ��� �� �޽��� Ȯ�� */
	str_len = read(clnt_sock, message_from_client, sizeof(message_from_client) -1);
	if (str_len == -1)
		error_handling("read() error!");
	// read �Լ� -1 �̳� 0 ��ȯ. -1 ��ȯ�� ����(���� �ڵ鸵)
	printf("Message from client: %s \n", message_from_client);

	/* ���������� ��ȭ ���� ���� */
	while (1) {
		read_cnt = fread((void*)buf, 1, BUF
			_SIZE, fp);
		if (read_cnt < BUF_SIZE) {
			write(clnt_sock, buf, read_cnt)
				;
			break;		}
		write(clnt_sock, buf, BUF_SIZE);
	}

	/* shutdown �Լ� - ���� �۹� ���� */
	shutdown(clnt_sock, SHUT_WR);

	/* read �Լ� - Client �κ��� �� ���� �޽��� Ȯ�� */
	read(clnt_sock, buf, BUF_SIZE);
	printf("Message from client: %s \n", buf);

	/* fclose �Լ� - ���� �ݱ� */
	fclose(fp);

	/* close �Լ� - ���� ���� */
	close(clnt_sock);
	close(serv_sock);

	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}