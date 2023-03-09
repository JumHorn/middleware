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

# location代理

	配置服务器的一个location作为代理，通过该location访问特定服务.
	以下例子表示通过服务器的/vscode路径，访问内部vscode服务

## 配置
```config
location /vscode/ {
	proxy_pass  http://127.0.0.1:8080/;

	// 当vscode返回/login时默认被location /匹配了,此时需要修改该请求
	// 将/login转为/vscode/login,继续由/location /vscode/处理请求
	proxy_redirect http://127.0.0.1:8080/ https://$host/vscode/;

	// 支持websocket
	proxy_http_version 1.1;
	proxy_read_timeout 300;
	proxy_set_header Upgrade $http_upgrade;
	proxy_set_header Connection "upgrade";
	proxy_set_header Host $http_host;
	proxy_set_header X-Real-IP $remote_addr;
	proxy_set_header X-Real-PORT $remote_port;
	}
```

# 文件上传服务器

	配置简单的文件上传服务器

## 配置
```conf
server {
	listen       80 default_server;  # 默认ipv4端口
	listen       [::]:80 default_server;
	server_name  jumhorn.com;
	root         /home/www; #html文件的根目录

	# Load configuration files for the default server block.
	include /etc/nginx/default.d/*.conf;

	location / {
		index main.html; #默认为空，则访问index.html
	}

	# 配置put上传服务
	location /upload{
		alias     /home/www/upload;
        client_body_temp_path  /home/www/upload;
		client_max_body_size 100M;
        dav_methods  PUT DELETE MKCOL COPY MOVE;
        create_full_put_path   on;
        dav_access             group:rw  all:r;
	}

	error_page 404 /404.html;
		location = /40x.html {
	}

	error_page 500 502 503 504 /50x.html;
		location = /50x.html {
	}
}
```

```shell
# 最后一个/是必须的，表示到这个目录，否则会被认为upload是文件名
curl -T data.txt http://jumhorn.com/upload/
# 改变名字上传到服务器
curl -T data.txt http://jumhorn.com/upload/newname.txt
```

## 视频流服务器

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

## 问题
1. Permission denied

* 检查所有目录都具有执行权限
	nginx需要所有目录都有执行权限，所以不要将根目录放在$HOME下

* 检查upload文件夹所属者

	要将upload文件夹改为http用户所属
```shell
chown http upload
```