/**
 * @file cryto_test.cpp
 * @author JumHorn (JumHorn@gamil.com)
 * @brief cryto test
 * @version 0.1
 * @date 2020-11-24
 *
 * @copyright Copyright (c) 2020
 *
 * g++ -std=c++11 -o test cryto_test.cpp crypto.cpp -lcrypto
 * -L/path/to/openssl/lib -I/path/to/openssl/include
 *
 */

#include "crypto.h"
#include <iostream>
using namespace std;

void print(vector<unsigned char> &v)
{
	for (auto c : v)
		printf("%02x", c);
	cout << endl;
}

int main()
{
	// this is the same result as md5sum command
	Crypto crypto;
	crypto.addData("hello world");
	auto encoded = crypto.result();
	print(encoded);

	//empty md5 hash d41d8cd98f00b204e9800998ecf8427e
	// unsigned char data[MD5_DIGEST_LENGTH];
	// MD5((unsigned char *)"", 0, data);
	// for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
	// 	printf("%02x", data[i]);
	// cout << endl;

	auto res = Crypto::hash("hello world", Crypto::SHA256);
	print(res);

	return 0;
}