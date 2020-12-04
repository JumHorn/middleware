/**
 * @file push_task.c
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
gcc -o push push_task.c -lzmq
*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>

#define BUFFER_SIZE 100

int main()
{
	void *context = zmq_ctx_new();
	// 连接⾄服务端的套接字
	void *sender = zmq_socket(context, ZMQ_PUSH);
	int res = zmq_connect(sender, "tcp://localhost:8000");
	assert(res == 0);

	for (int i = 0; i < 10; ++i)
	{
		// 发送消息
		const char *msg = "hello world";
		zmq_send(sender, msg, strlen(msg), 0);
		printf("published: %s\n", msg);
		fflush(stdout); //多线程下不flush，无法看到打印结果
		zmq_sleep(1);
	}
	//clean up
	zmq_close(sender);
	zmq_ctx_destroy(context);
	return 0;
}