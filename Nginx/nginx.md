# nginx

### log

1. 查看nginx.conf

/var/log/nginx/error.log(centos)

### permition denied

文件部署在/root目录下(权限不对)

### fastcgi

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

### module加载机制