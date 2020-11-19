#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <zmq.h>

/*
compile command
gcc -o server rep_server.c -lzmq
*/

/*
	Hello World 服务端
绑定⼀个REP套接字⾄tcp://*:5555
从客户端接收Hello，并应答World
*/

int main(void)
{
	// Socket to talk to clients
	void *context = zmq_ctx_new();
	// 与客户端通信的套接字
	void *responder = zmq_socket(context, ZMQ_REP);
	int res = zmq_bind(responder, "tcp://*:8000");
	assert(res == 0);

	while (1)
	{
		// 等待客户端请求
		char buffer[10];
		zmq_recv(responder, buffer, 10, 0);
		printf("recv Hello\n");
		// 返回应答
		zmq_send(responder, "World", 5, 0);
		printf("===================\n");
	}

	//clean up
	zmq_close(responder);
	zmq_ctx_destroy(context);
	return 0;
}
