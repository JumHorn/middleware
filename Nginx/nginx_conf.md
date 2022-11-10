# nginx常用配置

	配置简单的http服务器

## 负载均衡配置

	常见方式有:轮询（默认),weight(权重),ip_hash(解决session的问题)

1. 轮询
```conf
upstream webname {
server 192.168.0.1:8080;
server 192.168.0.2:8080;
}
```

2. weight
```conf
upstream webname {
server 192.168.0.1:8080 weight 2;
server 192.168.0.2:8080 weight 1;
}
```

3. ip_hash
```conf
upstream webname {
ip_hash;
server 192.168.0.1:8080;
server 192.168.0.2:8080;
}
```

## 主备
```conf
upstream webname {
server 192.168.0.1:8080;
server 192.168.0.2:8080 backup;
}
```

## 不参与负载均衡
```conf
upstream webname {
server 192.168.0.1:8080;
server 192.168.0.2:8080 down;
}
```

## fastcgi
```conf
location = /cmd {
	fastcgi_pass 127.0.0.1:8088;
	fastcgi_index index.cgi;
	include fastcgi.conf;
}
```

## https & ssl
```conf
server{
	listen  443 ssl; #监听端口
    server_name  a.domain.com; #请求域名
	ssl on; #开启ssl
	ssl_certificate /home/cert/a.domain.com.pem; #pem证书路径
	ssl_certificate_key     /home/cert/a.domain.com.key; #pem证书key路径
	ssl_session_timeout     5m; #会话超时时间
	ssl_ciphers     ECDHE-RSA-AES128-GCM-SHA256:ECDHE:ECDH:AES:HIGH:!NULL:!aNULL:!MD5:!ADH:!RC4; #加密算法
	ssl_protocols   TLSv1 TLSv1.1 TLSv1.2; #SSL协议
}
```

## 二级域名(subdomain)

	nginx配置中server下的server_name可以匹配二级域名
	让不同域名,访问不同服务
```config
server{
	listen 80;
	server_name a.domain.com;
}

server{
	listen 80;
	server_name b.domain.com;
}
```

## location root alias

1. location配合root

	表示访问路径为http://domain.com/images/a.jpg 将转化为访问root即本地
	/home/www/images/a.jpg
```config
server{
	listen 80;
	root /home/www;
	location /images {
	}
}
```

2. location配合alias

	表示访问路径为http://domain.com/images/a.jpg 将转化为访问本地
	/home/www/a.jpg,并不加上url，同时alias要以/结尾
```config
server{
	listen 80;
	alias /home/www/;
	location /images {
	}
}
```

## video stream

	配置nginx播放mp4视频
```config
location ~/.mp4 {
	root /media;
	mp4;
	mp4_buffer_size     1M;
	mp4_max_buffer_size 3M;

	# enable thread bool
	aio threads=default;

	# enable caching for mp4 videos
	proxy_cache mycache;
	proxy_cache_valid 200 300s;
	proxy_cache_lock on;

	# enable nginx slicing
	slice              1m;
	proxy_cache_key    $host$uri$is_args$args$slice_range;
	proxy_set_header   Range $slice_range;
	proxy_http_version 1.1;

	# Immediately forward requests to the origin if we are filling the cache
	proxy_cache_lock_timeout 0s;

	# Set the 'age' to a value larger than the expected fill time
	proxy_cache_lock_age 200s;

	proxy_cache_use_stale updating;

}
```

## 反向代理(proxy_pass)

## FAQ
1. permition denied

文件部署在/root目录下(权限不对)

2. 测试配置文件
```shell
nginx -t
```