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

static const string default_key = "default_key";

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

string Crypto::des_encrypt(const string &input)
{
	DES_cblock key;
	DES_key_schedule key_schedule;

	/* generate key  */
	DES_string_to_key(default_key.c_str(), &key);
	if (DES_set_key_checked(&key, &key_schedule) != 0)
		return "";

	//calc output len
	size_t output_len = (input.length() + 7) / 8 * 8;
	string output(output_len, ' ');

	DES_cblock ivec;
	memset((char *)&ivec, 0, sizeof(ivec));

	DES_ncbc_encrypt((const unsigned char *)input.c_str(), (unsigned char *)output.c_str(), input.length(),
					 &key_schedule, &ivec, DES_ENCRYPT);
	return output;
}

string Crypto::des_decrypt(const string &input)
{
	DES_cblock key;
	DES_key_schedule key_schedule;

	/* generate key  */
	DES_string_to_key(default_key.c_str(), &key);
	if (DES_set_key_checked(&key, &key_schedule) != 0)
		return "";

	DES_cblock ivec;
	memset((char *)&ivec, 0, sizeof(ivec));
	unsigned char *output = new unsigned char[input.length()];
	memset(output, 0, sizeof(char) * input.length());

	DES_ncbc_encrypt((const unsigned char *)input.c_str(), output, input.length(),
					 &key_schedule, &ivec, DES_DECRYPT);
	string res((char *)output, strlen((char *)output));
	delete[] output;
	return res;
}