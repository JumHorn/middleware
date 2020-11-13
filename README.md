# collections of useful middleware for myself

## RingBuffer

## ThreadPool

## MemoryPool

## Connection Pool

## protobuffer

1. protobuffer自动生成类的定义
2. protobuffer序列化原理

## brpc

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