# nginx

### 命令
1. -p

指定日志路径的前缀

### 配置
1. 非daemon方式运行
daemon off;

### log

1. 查看nginx.conf

/var/log/nginx/error.log(centos)

### permition denied

文件部署在/root目录下(权限不对)

### fastcgi

### 负载均衡配置



# nginx运行原理

nginx源码分析执行过程

### 配置

1. nginx的配置对应于

nginx.c文件中的
```C
ngx_core_commands
```

2. argc/argv参数处理
```C
ngx_get_options()
```

3. 配置文件读取
```C
ngx_conf_parse() //processed token by token
```

### event事件

1. 在初始化全局配置后的最后完成监听socket的创建
```C
ngx_init_cycle();
ngx_open_listening_sockets();
```

2. 进入事件主循环
```C
ngx_master_process_cycle();//进入master 循环
ngx_worker_process_cycle();//进入work process循环
ngx_process_events_and_timers();//子进程处理所有事件
```

3. 如何加载event module
```C
ngx_event_core_init_conf();
```

### http请求处理流程

1. 初始化添加handler
```C
ngx_http_optimize_servers
ngx_http_init_listening
// ngx_http_add_listening
// ngx_http_init_connection()//这个handler在有事件时调用
```

2. epoll_wait接收事件

1. 多进程调试

由于nginx是多进程模式,所以接收数据的代码和master process不再一个线程里
这里用vscode调试或者其他工具调试需要attach to process id
vscode配置如下
```json
{
	"name": "debug running process",
	"type": "cppdbg",
	"request": "attach",
	"program": "${workspaceFolder}/objs/nginx",
	"processId": "${command:pickProcess}",
	"MIMode": "lldb",
}
```
**attach到进程时,进程阻塞,需要F5继续执行**

2. 进入http事件过程
```C
ngx_event_accept()//接收http请求
ngx_http_init_connection();//初始化http连接
ngx_http_wait_request_handler();//处理request
ngx_http_process_request();
ngx_http_handler();
ngx_http_core_run_phases();
```

3. http状态机

11种状态
```C
typedef enum {
    NGX_HTTP_POST_READ_PHASE = 0,

    NGX_HTTP_SERVER_REWRITE_PHASE,

    NGX_HTTP_FIND_CONFIG_PHASE,
    NGX_HTTP_REWRITE_PHASE,
    NGX_HTTP_POST_REWRITE_PHASE,

    NGX_HTTP_PREACCESS_PHASE,

    NGX_HTTP_ACCESS_PHASE,
    NGX_HTTP_POST_ACCESS_PHASE,

    NGX_HTTP_PRECONTENT_PHASE,

    NGX_HTTP_CONTENT_PHASE,

    NGX_HTTP_LOG_PHASE
} ngx_http_phases;

ngx_http_init_phase_handlers
```

### module加载机制

1. configure时将module添加到ngx_modules数组中

```shell
./configure --add-module
```
```C
//ngx_modules.c
//将module的实例化对象加入到改数组中
ngx_module_t *ngx_modules[]
```

### 重要module解析

1. upstream

转发到后端服务器

2. handler

3. filter

后端服务器返回数据转发给客户端