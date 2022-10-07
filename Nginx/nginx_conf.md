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

## FAQ
1. permition denied

文件部署在/root目录下(权限不对)

2. 测试配置文件
```shell
nginx -t
```