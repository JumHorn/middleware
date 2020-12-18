/**
 * @file libco_test.cpp
 * @author JumHorn (JumHorn@gamil.com)
 * @brief epoll test for coroutine
 * @version 0.1
 * @date 2020-11-26
 *
 * @copyright Copyright (c) 2020
 *
 */

/*
compile command

g++ -o test libco_test.cpp -I/path/to/libco -L/path/to/lib -lcolib
*/

#include "co_routine.h"
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stack>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/un.h>
#include <unistd.h>
using std::stack;

#define SERVER_PORT 8000
#define BUFFER_SIZE 1024

int co_accept(int fd, struct sockaddr *addr, socklen_t *len); // why not hook this function?

typedef struct task_t
{
	stCoRoutine_t *co;
	int fd;
} task;

static stack<task *> g_tasks;

//set noblock
int setNonBlock(int fd)
{
	return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK | O_NDELAY);
}

struct sockaddr_in createServerInfo(const char *ip, int port)
{
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	// Filling server information
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	//server_addr.sin_addr.s_addr = inet_addr(ip);
	return server_addr;
}

static void *readwrite_routine(void *args)
{
	co_enable_hook_sys();

	char buffer[BUFFER_SIZE];
	task *co_task = (task *)args;

	for (;;)
	{
		if (-1 == co_task->fd)
		{
			g_tasks.push(co_task);
			co_yield_ct();
			continue;
		}

		int fd = co_task->fd;
		co_task->fd = -1;

		for (;;) //handle one client socket
		{
			struct pollfd pf = {0};
			pf.fd = fd;
			pf.events = (POLLIN | POLLERR | POLLHUP);
			co_poll(co_get_epoll_ct(), &pf, 1, 1000);

			int nbytes = read(fd, buffer, sizeof(buffer));
			if (nbytes > 0)
			{
				buffer[nbytes] = '\0';
				nbytes = write(fd, buffer, nbytes);
			}

			if (nbytes <= 0)
			{
				// accept_routine->SetNonBlock(fd) cause EAGAIN, we should continue
				if (errno == EAGAIN)
					continue;
				close(fd);
				break;
			}
		}
	}
	return 0;
}

static void *accept_routine(void *args)
{
	co_enable_hook_sys();
	// printf("accept_routine\n");
	// fflush(stdout);
	int listen_fd = *(int *)args;
	for (;;)
	{
		if (g_tasks.empty())
		{
			//check timeout sets
			struct pollfd pf = {0};
			pf.fd = -1;
			poll(&pf, 1, 1000);

			continue;
		}

		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		socklen_t len = sizeof(addr);
		int fd = co_accept(listen_fd, (struct sockaddr *)&addr, &len);

		if (fd < 0) //add listen_fd back to epool
		{
			struct pollfd pf = {0};
			pf.fd = listen_fd;
			pf.events = (POLLIN | POLLERR | POLLHUP);
			co_poll(co_get_epoll_ct(), &pf, 1, 1000);
			continue;
		}
		if (g_tasks.empty()) //no task to process this client,close it directly
		{
			close(fd);
			continue;
		}
		setNonBlock(fd);
		task_t *co_task = g_tasks.top();
		co_task->fd = fd;
		g_tasks.pop();
		co_resume(co_task->co);
	}
	return 0;
}

int main()
{
	//create socket file descriptor
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0)
	{
		printf("socket creation failed\n");
		return 0;
	}
	//init server address information
	struct sockaddr_in server_addr = createServerInfo("127.0.0.1", SERVER_PORT);
	//bind to address
	int res = bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
	if (res < 0)
	{
		printf("bind failed,may be port %d already in use\n", SERVER_PORT);
		return 0;
	}
	//listen
	res = listen(sock_fd, 1024);
	if (res < 0)
	{
		printf("Error while listening\n");
		return 0;
	}
	//set noblock
	setNonBlock(sock_fd);

	//read_write_routines
	for (int i = 0; i < 1; ++i)
	{
		task *co_task = new task();
		co_task->fd = -1;
		co_create(&co_task->co, NULL, readwrite_routine, co_task);
		co_resume(co_task->co);
	}

	//accept routine
	stCoRoutine_t *accept_co = NULL;
	co_create(&accept_co, NULL, accept_routine, (void *)&sock_fd);
	co_resume(accept_co);

	co_eventloop(co_get_epoll_ct(), NULL, NULL);
	return 0;
}