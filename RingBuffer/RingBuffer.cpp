/*
Copyright (c) 2020 by JumHorn <JumHorn@gmail.com>
Distributed under the MIT License. (See accompanying file LICENSE)
*/

/*
this data pool class was implemented with circlar buffer based on consumer and producer model
*/

#include "RingBuffer.h"
#include "Semaphore.h"

RingBuffer::RingBuffer(int size) : usedindex(0), freeindex(0)
{
	size = setPoolSize(size);
	pool = new char[size];
	poolsize = size;
	useful = (pool == NULL || size <= 0);
	semfree = new Semaphore(size - 1);
	semused = new Semaphore(0);
}

RingBuffer::~RingBuffer()
{
	if (pool != NULL)
	{
		delete[] pool;
		pool = NULL;
	}
	if (semfree != NULL)
	{
		delete semfree;
		semfree = NULL;
	}
	if (semused != NULL)
	{
		delete semused;
		semused = NULL;
	}
}

/*
these state variable are of nonsense without any locks
*/
bool RingBuffer::isFull() const
{
	return ((usedindex + 1) & (poolsize - 1)) == freeindex;
}

int RingBuffer::getFreeSize() const
{
	return poolsize - 1 - getUsedSize();
}

int RingBuffer::getUsedSize() const
{
	return (usedindex + poolsize - freeindex) & (poolsize - 1);
}

bool RingBuffer::pushData(const char *buffer, int size)
{
	bool res = false;
	if (useful)
	{
		semfree->wait(size + sizeof(int));
		mused.lock();
		push((char *)&size, sizeof(int));
		push(buffer, size);
		mused.unlock();
		semused->post(size + sizeof(int));
		res = true;
	}
	return res;
}

template <typename T>
bool RingBuffer::pushData(const T &t)
{
	return pushData((char *)&t, sizeof(T));
}

bool RingBuffer::popData(const char *buffer, int &size)
{
	bool res = false;
	if (useful)
	{
		mfree.lock();
		semused->wait(sizeof(int));
		pop((char *)&size, sizeof(int));
		semused->wait(size);
		pop(buffer, size);
		semfree->post(size + sizeof(int));
		mfree.unlock();
		res = true;
	}
	return res;
}

template <typename T>
bool RingBuffer::popData(const T &t)
{
	int tmp = 0;
	return popData((char *)&t, tmp);
}

void RingBuffer::push(const char *buffer, int size)
{
	if (size <= poolsize - usedindex)
	{
		memcpy((char *)(pool + usedindex), buffer, size);
	}
	else
	{
		memcpy((char *)(pool + usedindex), buffer, poolsize - usedindex);
		memcpy((char *)pool, buffer + poolsize - usedindex, size - (poolsize - usedindex));
	}
	usedindex = ((usedindex + size) & (poolsize - 1));
}

void RingBuffer::pop(const char *buffer, int size)
{
	if ((usedindex > freeindex) || (freeindex + size <= poolsize))
	{
		memcpy((char *)buffer, (char *)(pool + freeindex), size);
	}
	else
	{
		memcpy((char *)buffer, (char *)(pool + freeindex), poolsize - freeindex);
		memcpy((char *)(buffer + poolsize - freeindex), pool, size - (poolsize - freeindex));
	}
	freeindex = ((freeindex + size) & (poolsize - 1));
}

unsigned int RingBuffer::setPoolSize(unsigned int size) const
{
	unsigned int tmp = size;
	tmp |= tmp >> 1;
	tmp |= tmp >> 2;
	tmp |= tmp >> 4;
	tmp |= tmp >> 8;
	tmp |= tmp >> 16;
	return (size & (tmp >> 1)) != 0 ? (tmp + 1) : size;
}