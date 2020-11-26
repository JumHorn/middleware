# openssl

1. md5

compile
```shell
g++ -std=c++11 -o test cryto_test.cpp crypto.cpp base64.c -L/usr/local/opt/openssl@1.1/lib -I/usr/local/opt/openssl@1.1/include -lcrypto
```

2. cource file
* base64.c
* base64.h
* crypto.cpp
* crypto.h

# alternative

## crypto++

> https://www.cryptopp.com
> https://github.com/weidai11/cryptopp