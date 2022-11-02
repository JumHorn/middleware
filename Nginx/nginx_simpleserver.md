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

## 问题
1. Permission denied

* 检查所有目录都具有执行权限
	nginx需要所有目录都有执行权限，所以不要将根目录放在$HOME下

* 检查upload文件夹所属者

	要将upload文件夹改为http用户所属
```shell
chown http upload
```