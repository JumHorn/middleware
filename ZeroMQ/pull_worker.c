/**
 * @file pull_worker.c
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
gcc -o pull pull_worker.c -lzmq
*/

#include <assert.h>
#include <stdlib.h>
#include <zmq.h>

#define BUFFER_SIZE 100

int main()
{
	void *context = zmq_ctx_new();
	// 连接⾄服务端的套接字
	void *receiver = zmq_socket(context, ZMQ_PULL);
	zmq_bind(receiver, "tcp://*:8000");

	for (int i = 0; i < 10; ++i)
	{
		char buffer[BUFFER_SIZE];
		int nbytes = zmq_recv(receiver, buffer, BUFFER_SIZE, 0);
		buffer[nbytes] = '\0';
		printf("recv: %s\n", buffer);
		printf("===================\n");
		fflush(stdout);
	}
	//clean up
	zmq_close(receiver);
	zmq_ctx_destroy(context);
	return 0;
}