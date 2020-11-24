/**
 * @file crypto.cpp
 * @author JumHorn (JumHorn@gamil.com)
 * @brief
 * @version 0.1
 * @date 2020-11-24
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "crypto.h"
#include <openssl/des.h>
#include <openssl/md5.h>

Crypto::Crypto(Crypto::Algorithm method) : method(method), context(new MD5_CTX)
{
	MD5_Init(context.get());
}

Crypto::~Crypto()
{
}

void Crypto::addData(const char *data, int length)
{
	if (length < 0) //null terminated str
		length = strlen(data);
	MD5_Update(context.get(), data, length);
}

void Crypto::addData(const string &data)
{
	addData(data.c_str());
}

void Crypto::reset()
{
	MD5_Init(context.get());
}

vector<unsigned char> Crypto::result() const
{
	vector<unsigned char> data(MD5_DIGEST_LENGTH);
	MD5_Final(data.data(), context.get());
	return data;
}