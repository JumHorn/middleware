/*

Coroutines in C

based on this trick(Duff Device),
so the following macro crBegin,crReturn,crFinish are the approach

void DuffsDevice()
{
	switch (count % 8)
	{
		case 0:        do {  *to = *from++;
		case 7:              *to = *from++;
		case 6:              *to = *from++;
		case 5:              *to = *from++;
		case 4:              *to = *from++;
		case 3:              *to = *from++;
		case 2:              *to = *from++;
		case 1:              *to = *from++;
						} while ((count -= 8) > 0);
	}
}

there is a consumer and producer example to illustrate this idea
*/

#include <iostream>
using namespace std;

// stackless coroutines
#define crBegin           \
	static int state = 0; \
	switch (state)        \
	{                     \
	case 0:

#define crReturn(x)       \
	do                    \
	{                     \
		state = __LINE__; \
		return x;         \
	case __LINE__:;       \
	} while (0)

#define crFinish }

//the shared buffer
#define SIZE (1 << 3)
char buffer[8];
int freed;
int used;

//producer function
void produce()
{
	crBegin;
	while (1)
	{
		buffer[used] = "coroutin"[used];
		cout << "producer input "
			 << "coroutin"[used] << endl;
		used++;
		used &= SIZE - 1;
		crReturn(); //yeild to consume
	}
	crFinish;
}

//consumer function
void consume()
{
	crBegin;
	while (1)
	{
		cout << "consumer output " << buffer[freed] << endl;
		freed++;
		freed &= SIZE - 1;
		crReturn(); //yeild to produce
	}
	crFinish;
}

int main()
{
	while (1)
	{
		char c = getchar();
		produce();
		consume();
	}
	return 0;
}