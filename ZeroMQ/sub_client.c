/**
 * @file sub_client.c
 * @author JumHorn (JumHorn@gamil.com)
 * @brief
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */

/*
compile command
gcc -o client sub_client.c -lzmq
*/

#include <assert.h>
#include <stdlib.h>
#include <zmq.h>

#define BUFFER_SIZE 100

int main()
{
	void *context = zmq_ctx_new();
	// 连接⾄服务端的套接字
	void *subscriber = zmq_socket(context, ZMQ_SUB);
	int res = zmq_connect(subscriber, "tcp://localhost:8000");
	// this must be set for recv msg
	zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, NULL, 0);
	assert(res == 0);

	for (int i = 0; i < 10; ++i)
	{
		char buffer[BUFFER_SIZE];
		int nbytes = zmq_recv(subscriber, buffer, BUFFER_SIZE, 0);
		buffer[nbytes] = '\0';
		printf("recv: %s\n", buffer);
		printf("===================\n");
		fflush(0);
	}
	//clean up
	zmq_close(subscriber);
	zmq_ctx_destroy(context);
	return 0;
}