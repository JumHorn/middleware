# 设计原理

> http://www.wangafu.net/~nickm/libevent-book/Ref0_meta.html

1. evutil

对系统socket的封装

2. event and event_base

non-blocking IO后端代码

3. bufferevent evbuffer

将IO事件完成，不需要用户调用read/write/send/recv