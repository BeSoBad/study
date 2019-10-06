/*
* Stock Market Client
* Connects SUB socket to tcp://localhost:4040
* Collects stock exchange values
*/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "zmq.h"
typedef struct {
	int a;
	int b;
	int number;
} TwoInt;
const int LINGER_VAL = 0;
int main (int argc, char const *argv[]) 
{
	void* context_push = zmq_ctx_new();
	void* subscriber_push = zmq_socket(context_push, ZMQ_PUSH);
	printf("Collecting stock information from the server.\n");
	zmq_connect(subscriber_push, "tcp://localhost:4043");
	void* context_pull = zmq_ctx_new();
	void* subscriber_pull = zmq_socket(context_pull, ZMQ_PULL);
	zmq_connect(subscriber_pull, "tcp://localhost:4044");
	//zmq_setsockopt(subscriber, ZMQ_LINGER, &LINGER_VAL, sizeof(LINGER_VAL)); 
	int i = 1;
	for(;;)
	{
		TwoInt ti;
		ti.a = rand() % 100;
		ti.b = -1;
		ti.number = i;

		zmq_msg_t message_out;
		zmq_msg_init_size(&message_out, sizeof(TwoInt));
		memcpy(zmq_msg_data(&message_out), &ti, sizeof(TwoInt));
		zmq_msg_send(&message_out, subscriber_push, 0);
		zmq_msg_close(&message_out);

		zmq_msg_t message_answer;
		zmq_msg_init_size(&message_answer, sizeof(TwoInt));
		zmq_msg_recv(&message_answer, subscriber_pull, 0);
		TwoInt t_i;
		memcpy(&t_i, zmq_msg_data(&message_answer), sizeof(TwoInt));
		//printf("p: %p\n", zmq_msg_data(&message_answer));
		printf("a: %d, b: %d, number: %d", t_i.a, t_i.b, t_i.number);
		zmq_msg_close(&message_answer);
		printf(":%d\n", i);
		sleep(1);
		i++;
	}
	zmq_close(subscriber_push);
	zmq_close(subscriber_pull);
	zmq_ctx_destroy(context_push);
	zmq_ctx_destroy(context_pull);
	return 0;
}