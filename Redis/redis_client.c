#include <hiredis/hiredis.h>

/*
compile command
gcc -o client redis_client.c -lhiredis
*/

#define REDIS_SERVERIP "192.168.10.155"
#define REDIS_SERVERPORT 6379

int main()
{
	redisContext *context = redisConnect(REDIS_SERVERIP, 6379);
	if (context == NULL || context->err)
	{
		if (context)
			printf("%s\n", context->errstr);
		else
			printf("connect error");
		return 0;
	}

	//auth
	redisReply *reply = redisCommand(context, "auth hj");
	if (reply->type == REDIS_REPLY_STATUS)
	{
		printf("authentification success\n");
	}
	freeReplyObject(reply);

	//get str key/value
	char *key = "name";
	reply = redisCommand(context, "get %s", key);
	if (reply->type == REDIS_REPLY_STRING)
	{
		printf("value: %s\n", reply->str);
	}
	freeReplyObject(reply);

	//get array key/value
	char *db[] = {"redis", "mysql", "mongodb"};
	for (int i = 0; i < 3; ++i)
		reply = redisCommand(context, "sadd db %s", db[i]);
	reply = redisCommand(context, "smembers db");
	if (reply->type == REDIS_REPLY_ARRAY)
	{
		for (int i = 0; i < reply->elements; ++i)
		{
			if (reply->element[i]->type == REDIS_REPLY_STRING)
			{
				printf("%s\n", reply->element[i]->str);
			}
		}
	}
	freeReplyObject(reply);
	return 0;
}