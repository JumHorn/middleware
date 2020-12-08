# 模型

> https://zguide.zeromq.org/docs/chapter1/#Getting-the-Examples

### REQ/REP模型
* rep_server.c
* req_client.c

### PUB/SUB模型
* pub_server.c
* sub_client.c

### PULL/PUSH模型

可以用来做负载均衡

> https://zguide.zeromq.org/docs/chapter1/

### Router/Dealer模型

# 其他用法

# 源码分析

## 数据传输流程分析
1. engine ==> reactor
2. session ==> pipe
3. context类中io线程和mailbox对应

通过object_t的_tid属性,在线程创建时便给值
```C++
// ctx.cpp
bool zmq::ctx_t::start ();
```

## 重要类型分析
1. zmq::ctx_t
```C++
//  Sockets belonging to this context. We need the list so that
//  we can notify the sockets when zmq_ctx_term() is called.
//  The sockets will return ETERM then.
typedef array_t<socket_base_t> sockets_t;
sockets_t _sockets;

//  List of unused thread slots.
typedef std::vector<uint32_t> empty_slots_t;
empty_slots_t _empty_slots;
```
2. io_thread_t
3. mailbox_t/signaler_t
4. stream_engine_base_t

## 重要数据结构
1. ypipe.hpp

无锁队列

# TODO
1. req/rep模型客户端在服务端后起,也能接收到消息

设置定时器，超时重新连接。定时器是multimap实现的
```C++
worker_poller_base_t::worker_routine();
poller_base_t::execute_timers();//返回下一个定时器时间
//定时器返回的时间又作为epoll_wait的time_out参数，十分巧妙
epoll_wait(time_out);
```
