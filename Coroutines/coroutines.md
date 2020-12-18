# tencent wechat libco

> https://github.com/Tencent/libco

# libco源码流程分析
## 上下文切换原理
```C++
// 在开辟的堆空间内存上,模拟函数调用栈
// 将IP(Instruction Pointer)寄存器指向 pfn函数入口
// 将该函数的参数，和返回值加入栈中,设置好BP和SP寄存器
coctx_make(coctx_pfn_t pfn);
// 将CPU中的寄存器保存到当前协程的ctx(context)中
// 将coctx_make刚刚设置好的调用堆栈
// 中的寄存器load到CPU上执行
coctx_swap();
```

## hook
## 时间轮
## 共享栈


# boost coroutines2

# C++20 coroutine

# stackless coroutine