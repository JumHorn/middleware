#include <zmq.h>

/*
compile command
gcc -o client req_client.c -lzmq
*/

#define BUFFER_SIZE 100

int main()
{
	printf("Connecting to hello world server...\n");
	void *context = zmq_ctx_new();
	// 连接⾄服务端的套接字
	void *requester = zmq_socket(context, ZMQ_REQ);
	zmq_connect(requester, "tcp://localhost:8000");

	for (int i = 0; i < 10; ++i)
	{
		char buffer[BUFFER_SIZE];
		printf("send Hello %d...\n", i);
		zmq_send(requester, "Hello", 5, 0);
		zmq_recv(requester, buffer, BUFFER_SIZE, 0);
		printf("recv World %d\n", i);
		printf("===================\n");
	}
	//clean up
	zmq_close(requester);
	zmq_ctx_destroy(context);
	return 0;
}