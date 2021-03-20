# collections of useful middleware

## RingBuffer

## ThreadPool

## MemoryPool

## Connection Pool

## protobuffer

1. protobuffer自动生成类的定义
2. protobuffer序列化原理
> https://developers.google.com/protocol-buffers/docs/encoding

	varint

## brpc

demo的源码在example目录下

### mac下编译BRPC出现openssl问题
```shell
brew info openssl
```
重新生成CMakeLists.txt

## coroutines

### 目的
1. 同步的编程方式，异步的性能
2. 简化网络编程

一请求一线程,百万并发系统线程数目有限
```shell
cat /proc/sys/kernel/threads-max
```
协程数目远远多于线程数

### 实现方法
1. setjump/longjump
2. ucontext(linux)
3. assembly

保存CPU寄存器，实现上下文切换的协程实现原理

```C++
typedef struct cpu_context {
void *esp; //
void *ebp;
void *eip;
void *edi;
void *esi;
void *ebx;
void *r1;
void *r2;
void *r3;
void *r4;
void *r5;
} cpu_context;

//new_ctx --> rdi ,
//cur_ctx --> rsi

int _switch(cpu_context *new_ctx, cpu_context *cur_ctx);

#ifdef __i386__
__asm__ (
"    .text                                  \n"
"    .p2align 2,,3                          \n"
".globl _switch                             \n"
"_switch:                                   \n"
"__switch:                                  \n"
"movl 8(%esp), %edx      # fs->%edx         \n"
"movl %esp, 0(%edx)      # save esp         \n"
"movl %ebp, 4(%edx)      # save ebp         \n"
"movl (%esp), %eax       # save eip         \n"
"movl %eax, 8(%edx)                         \n"
"movl %ebx, 12(%edx)     # save ebx,esi,edi \n"
"movl %esi, 16(%edx)                        \n"
"movl %edi, 20(%edx)                        \n"
"movl 4(%esp), %edx      # ts->%edx         \n"
"movl 20(%edx), %edi     # restore ebx,esi,edi      \n"
"movl 16(%edx), %esi                                \n"
"movl 12(%edx), %ebx                                \n"
"movl 0(%edx), %esp      # restore esp              \n"
"movl 4(%edx), %ebp      # restore ebp              \n"
"movl 8(%edx), %eax      # restore eip              \n"
"movl %eax, (%esp)                                  \n"
"ret                                                \n"
);
#elif defined(__x86_64__)

__asm__ (
"    .text                                  \n"
"       .p2align 4,,15                                   \n"
".globl _switch                                          \n"
".globl __switch                                         \n"
"_switch:                                                \n"
"__switch:                                               \n"
"       movq %rsp, 0(%rsi)      # save stack_pointer     \n"
"       movq %rbp, 8(%rsi)      # save frame_pointer     \n"
"       movq (%rsp), %rax       # save insn_pointer      \n"
"       movq %rax, 16(%rsi)                              \n"
"       movq %rbx, 24(%rsi)     # save rbx,r12-r15       \n"
"       movq %r12, 32(%rsi)                              \n"
"       movq %r13, 40(%rsi)                              \n"
"       movq %r14, 48(%rsi)                              \n"
"       movq %r15, 56(%rsi)                              \n"
"       movq 56(%rdi), %r15                              \n"
"       movq 48(%rdi), %r14                              \n"
"       movq 40(%rdi), %r13     # restore rbx,r12-r15    \n"
"       movq 32(%rdi), %r12                              \n"
"       movq 24(%rdi), %rbx                              \n"
"       movq 8(%rdi), %rbp      # restore frame_pointer  \n"
"       movq 0(%rdi), %rsp      # restore stack_pointer  \n"
"       movq 16(%rdi), %rax     # restore insn_pointer   \n"
"       movq %rax, (%rsp)                                \n"
"       ret                                              \n"
);
#endif
```

### 协程例子
1. coroutines in C++20
2. stackless coroutines
```C++
// stackless coroutines
#define crBegin           \
	static int state = 0; \
	switch (state)        \
	{                     \
	case 0:

#define crReturn(x)       \
	do                    \
	{                     \
		state = __LINE__; \
		return x;         \
	case __LINE__:;       \
	} while (0)

#define crFinish }
```

3. stackfull coroutines

4. coroutines in Boost
```C++
/*
g++ -std=c++11 example_boost.cpp -lboost_context

boost 协程push_type将数据送给pull_type
push_type ==> pull_type
*/

int main()
{
	using coro_t = boost::coroutines2::coroutine<int>;

	auto source = coro_t::pull_type([](coro_t::push_type &yield) {
		for (auto i = 0; i != 3; ++i)
		{
			yield(i);
		}
	});

	for (auto value : source)
	{
		std::cout << value << std::endl;
	}
	return 0;
}
```

### tencent wechart

> https://github.com/Tencent/libco

## Nginx
1. 负载均衡

## Redis

### 连接redis

1. 编译安装hiredis
> https://github.com/redis/hiredis

2. hiredis reply定义

```C++
/* This is the reply object returned by redisCommand() */
typedef struct redisReply {
	/*命令执行结果的返回类型*/
	int type; /* REDIS_REPLY_* */
	/*存储执行结果返回为整数*/
	long long integer; /* The integer when type is REDIS_REPLY_INTEGER */
	/*字符串值的长度*/
	size_t len; /* Length of string */
	/*存储命令执行结果返回是字符串*/
	char *str; /* Used for both REDIS_REPLY_ERROR and REDIS_REPLY_STRING */
	/*返回结果是数组的大小*/
	size_t elements; /* number of elements, for REDIS_REPLY_ARRAY */
	/*存储执行结果返回是数组*/
	struct redisReply **element; /* elements vector for REDIS_REPLY_ARRAY */
} redisReply;
```

## ZeroMQ
### 模型