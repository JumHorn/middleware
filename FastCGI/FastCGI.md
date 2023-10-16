# FastCGI

    FastCGI Developer's Kit

## 安装开发包和工具
1. 启动工具
```shell
sudo apt install spawn-fcgi
sudo apt-get install fcgiwrap # cgi over FastCGI
```
2. 开发包

## 配置nginx
```nginx
location /cgi/ {
    # Fastcgi socket
    fastcgi_pass  unix:/var/run/fcgiwrap.socket;

    # Fastcgi parameters, include the standard ones
    include fastcgi_params;

    # Adjust non standard parameters (SCRIPT_FILENAME)
    root /home/amdin/www; # root必须填，否则默认的root是/usr/share/nginx/html;
    fastcgi_param SCRIPT_FILENAME  $document_root$fastcgi_script_name;
}
```

## 生成FastCGI进程
```shell
systemctl restart fcgiwrap
```

## FAQ
1. 502 error

	用浏览器调试时，如果返回的不是完整的网页，则浏览器无法显示并报错。
	CGI必须返回Content-type: text/html，否则web服务器都会报错502 Internal Server Error
	Content-type类型 text/plain,application/json
	Content-type之后会后两个换行(\r\n)

2. DOCUMENT_ROOT and SCRIPT_NAME

	报错时fcgiwrap如果没有设置document_root，那么默认的root是nginx网页根目录，
	也就是nginx的默认目录，如/usr/share/nginx/html
