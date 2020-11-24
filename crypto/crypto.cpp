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
#include "base64.h"
#include <openssl/des.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

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

vector<unsigned char> Crypto::hash(const unsigned char *data, int datalen, Algorithm method)
{
	if (method == MD5)
	{
		vector<unsigned char> digest(MD5_DIGEST_LENGTH);
		::MD5(data, datalen, digest.data());
		return digest;
	}
	else if (method == BASE64)
	{
		int encode_len = Base64encode_len(datalen);
		vector<unsigned char> encodestr(encode_len + 1);
		Base64encode((char *)encodestr.data(), (const char *)data, datalen);
		return encodestr;
	}
	else if (method == SHA256)
	{
		vector<unsigned char> digest(SHA256_DIGEST_LENGTH);
		::SHA256(data, datalen, digest.data());
		return digest;
	}

	return {};
}

vector<unsigned char> Crypto::hash(const char *data, Algorithm method)
{
	return hash((unsigned char *)data, strlen(data), method);
}

vector<unsigned char> Crypto::hash(const string &data, Algorithm method)
{
	return hash((unsigned char *)data.c_str(), data.length(), method);
}