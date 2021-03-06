/**
 * @file crypto.h
 * @author JumHorn (JumHorn@gamil.com)
 * @brief cryto wrapper openssl as c++ class
 * @version 0.1
 * @date 2020-11-24
 *
 * @copyright Copyright (c) 2020
 *
 */
#ifndef _CRYPTO_H_
#define _CRYPTO_H_

#include <memory>
#include <string>
#include <vector>
using std::shared_ptr;
using std::string;
using std::vector;

typedef struct MD5state_st MD5_CTX;

class Crypto
{
public:
	enum Algorithm
	{
		MD5,
		SHA256,
		BASE64
	};

public:
	/**
	 * @brief now only implement MD5
	 *
	 * @param method
	 */
	Crypto(Algorithm method = MD5);
	~Crypto();
	void addData(const char *data, int length = -1);
	void addData(const string &data);
	void reset();
	vector<unsigned char> result() const;

	/**
	 * @brief static only once hash method include MD5,SHA256,BASE64
	 *
	 * @param data
	 * @param method
	 * @return vector<unsigned char>
	 */
	static vector<unsigned char> hash(const char *data, Algorithm method = MD5);
	static vector<unsigned char> hash(const string &data, Algorithm method = MD5);
	static vector<unsigned char> hash(const unsigned char *data, int datalen, Algorithm method);

	/**
	 * @brief encrypt/decrypt for des using ecb with default keys
	 *
	 */
	static string des_encrypt(const string &input);
	static string des_decrypt(const string &input);

private:
	Algorithm method;
	shared_ptr<MD5_CTX> context;
};

#endif