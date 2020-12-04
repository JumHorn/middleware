/**
 * @file pub_server.c
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
gcc -o server pub_server.c -lzmq
*/

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <zmq.h>

int main(void)
{
	// Socket to talk to clients
	void *context = zmq_ctx_new();
	// 与客户端通信的套接字
	void *publisher = zmq_socket(context, ZMQ_PUB);
	int res = zmq_bind(publisher, "tcp://*:8000");
	assert(res == 0);
	while (1)
	{
		// 发送消息
		const char *msg = "hello world";
		zmq_send(publisher, msg, strlen(msg), 0);
		printf("published: %s\n", msg);
		fflush(0); //多线程下不flush，无法看到打印结果
		zmq_sleep(1);
	}

	//clean up
	zmq_close(publisher);
	zmq_ctx_destroy(context);
	return 0;
}