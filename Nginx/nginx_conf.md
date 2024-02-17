# nginx常用配置

	nginx常用配置总结

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
	让不同域名,访问不同服务器。配置二级域名需要同时在DNS配置
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

	如果访问http://domain.com/images/a.jpg,
	将转化为访问root即本地/home/www/images/a.jpg
```config
server{
	listen 80;
	root /home/www;
	location /images {
	}
}
```

2. location配合alias

	如果访问http://domain.com/images/a.jpg,
	将转化为访问本地/home/www/a.jpg,并不加上路径images，**同时alias要以/结尾**
```config
server{
	listen 80;
	alias /home/www/;
	location /images {
	}
}
```

## 反向代理(proxy_pass)

## stream
```config
# SNI 分流
stream {
	map $ssl_preread_server_name $name {
		jumhorn.com   127.0.0.1:50443;
		default          127.0.0.1:8880;
	}

	server {
		listen      443;
		proxy_pass  $name;
		ssl_preread on;
	}
}
```

## 默认服务器
```config
# avoid to config ssl certificate for default server
map "" $empty {
        default "";
}

server {
        listen 127.0.0.1:8880 default_server;
        server_name _;

        ssl_certificate data:$empty;
        ssl_certificate_key data:$empty;
        return 444;
}
```

## FAQ
* location结尾有/和没有/的区别

1. url传递给后端时的区别

	由于proxy_pass后没有路径，那么proxy_pass直接忽略匹配好的location部分，
	所以proxy_pass都加上路径"/",此时对比就知道location有无“/”区别
```config
// http://nginx/images/a.jpg ==> http://backend/images/a.jpg
// 当请求经过nginx时，匹配到image后，不会忽略/images/，把image/a.jpg传递给后端
server{
	listen 80;
	location /images {
		proxy_pass http://domain.com/;
	}
}

// http://nginx/images/a.jpg ==> http://backend/a.jpg
// 当请求经过nginx时，匹配到image后，会忽略/images/，把a.jpg传递给后端
server{
	listen 80;
	location /images/ {
		proxy_pass http://domain.com/;
	}
}
```
2. 后端重定向区别

	还有一个重要的区别是，当服务端返回重定向时(如./login或者/login)

```conf
// 没有/结尾的location，/login重定向用的是绝对路径http://backend/login
server{
	listen 80;
	location /images {
		proxy_pass http://backend/;
	}
}

// 有/结尾的location，/login重定向用的是相对路径http://backend/images/login
server{
	listen 80;
	location /images/ {
		proxy_pass http://backend/;
	}
}
```

* proxy_pass结尾有/和没有/的区别
```config
// http://nginx/images/a.jpg ==> http://backend/a.jpg
// 当请求经过nginx时，匹配到image后，会忽略/images/，把a.jpg传递给后端
server{
	listen 80;
	location /images {
		proxy_pass http://domain.com;
	}
}

// http://nginx/images/a.jpg ==> http://backend/images/a.jpg
// 没有忽略/images/
server{
	listen 80;
	location /images {
		proxy_pass http://domain.com/;
	}
}
```
	总结分析: 虽然带上/images没有配置时，nginx自动301跳转到/images/下,这个自动跳转要注意可能存在错误
	自动跳转的域名加端口号可能不是所需要的，排查问题要小心

* permission denied

	文件部署在/root目录下(权限不对)

* 测试配置文件
```shell
nginx -t
```

* proxy_redirect
* SNI ssl error

 	当服务器配置了SNI时，会出现ssl握手错误，此时需要加上proxy_ssl_server_name on;
