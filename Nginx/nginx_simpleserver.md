# 静态网页

	配置最简单的http服务器，
	当nginx启动时便是一个默认的服务器了

## 配置

	所有的配置都是默认的
```conf
server {
	listen       80 default_server;  # 默认ipv4端口
	listen       [::]:80 default_server;
	server_name  _;
	root         /usr/share/nginx/html; #html文件的根目录

	# Load configuration files for the default server block.
	include /etc/nginx/default.d/*.conf;

	location / {
		index main.html; #默认为空，则访问index.html
	}

	error_page 404 /404.html;
		location = /40x.html {
	}

	error_page 500 502 503 504 /50x.html;
		location = /50x.html {
	}
}
```
