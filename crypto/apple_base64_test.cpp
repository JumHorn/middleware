/**
 * @file apple_base64_test.cpp
 * @author JumHorn (JumHorn@gamil.com)
 * @brief apple base64 code test
 * @version 0.1
 * @date 2020-11-24
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "base64.h"
#include <array>
#include <iostream>
#include <string>
using namespace std;

int main()
{
	string str = "hello base64";
	cout << "original str: " << str << endl;
	int encode_len = Base64encode_len(str.length());
	char encodestr[encode_len + 1];
	Base64encode(encodestr, str.data(), str.length());
	cout << "encoded str: " << encodestr << endl;
	int decode_len = Base64decode_len(encodestr);
	char decodestr[decode_len + 1];
	Base64decode(decodestr, encodestr);
	cout << "decoded str: " << decodestr << endl;
	return 0;
}