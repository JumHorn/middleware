#include "coctx.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

using std::string;

const static int STACK_SIZE = 128 * 1024;
const static int BUFF_SIZE = 2 * STACK_SIZE;
extern "C"
{
	extern void coctx_swap(coctx_t *, coctx_t *) asm("coctx_swap");
};

struct stRo_t
{
	coctx_t *ctx;
	string content;
	void (*fn)(stRo_t *);
};

static int RoutineFunc(stRo_t *cur, stRo_t *pre)
{
	if (cur->fn)
	{
		(*cur->fn)(cur);
	}
	coctx_swap(cur->ctx, pre->ctx); //第一次切回去

	if (cur->fn)
	{
		(*cur->fn)(cur);
	}
	coctx_swap(cur->ctx, pre->ctx); //第二次切回去
	printf("invalid here!\n");		//走到这里来，表示错误的切换过来了，后续没有切回去的代码了,走到这里会core
	return 0;
}

static void UserFunc(stRo_t *co)
{
	printf("%s", co->content.c_str());
	return;
}

int main()
{
	char *buffer = new char[2 * STACK_SIZE];
	memset(buffer, 0, sizeof(char) * BUFF_SIZE);
	coctx_t main_ctx;
	main_ctx.ss_size = STACK_SIZE;
	main_ctx.ss_sp = buffer;

	coctx_t pending_ctx;
	pending_ctx.ss_size = STACK_SIZE;
	pending_ctx.ss_sp = buffer + STACK_SIZE;

	stRo_t pending, mainRo;
	pending.ctx = &pending_ctx;
	pending.fn = UserFunc;
	pending.content = "hello ";
	mainRo.ctx = &main_ctx;
	mainRo.fn = NULL;

	//printf("main 0, before switch to hello\n");
	//构建pending协程coctx_t结构
	coctx_make(&pending_ctx, (coctx_pfn_t)RoutineFunc, &pending, &mainRo);

	//第一次切到协程
	coctx_swap(mainRo.ctx, pending.ctx);

	//printf("main 1, switch back to main\n");
	pending.content = "world!\n";

	//第二次切到协程
	coctx_swap(mainRo.ctx, pending.ctx);

	//printf("main 2, switch back to main\n");

	delete[] buffer;
	return 0;
}