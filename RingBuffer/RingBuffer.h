/*
Copyright (c) 2018 by JumHorn <JumHorn@gmail.com>
Distributed under the MIT License. (See accompanying file LICENSE)
*/

/*
this data pool class was implemented with circlar buffer based on consumer and producer model
*/

#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

#include <mutex>
using namespace std;

class Semaphore;

class RingBuffer
{
public:
	RingBuffer(int size = 65535);
	~RingBuffer();

	inline bool isReady() const { return useful; }
	inline bool isEmpty() const { return usedindex == freeindex; }
	bool isFull() const;
	int getFreeSize() const;
	int getUsedSize() const;

	bool pushData(const char *buffer, int size);
	template <typename T>
	bool pushData(const T &t);

	bool popData(const char *buffer, int &size);
	template <typename T>
	bool popData(const T &t);

private:
	void push(const char *buffer, int size);
	void pop(const char *buffer, int size);
	unsigned int setPoolSize(unsigned int size) const;

private:
	char *pool;
	bool useful;
	int poolsize;
	int freeindex;
	int usedindex;
	Semaphore *semfree;
	Semaphore *semused;
	mutex mfree;
	mutex mused;
};

#endif