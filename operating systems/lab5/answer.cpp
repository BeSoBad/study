/*
* Stock Market Server
* Binds PUB socket to tcp://*:4040
* Publishes random stock values of random companies
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
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
	void* context_pull = zmq_ctx_new();
	void* socket_pull = zmq_socket(context_pull, ZMQ_PULL);
	zmq_bind(socket_pull, "tcp://*:4043");
	//zmq_setsockopt(socket_pull, ZMQ_LINGER, &LINGER_VAL, sizeof(LINGER_VAL)); 	
	void* context_push = zmq_ctx_new();
	void* socket_push = zmq_socket(context_push, ZMQ_PUSH);
	zmq_bind(socket_push, "tcp://*:4044");
	//zmq_setsockopt(socket_push, ZMQ_LINGER, &LINGER_VAL, sizeof(LINGER_VAL)); 	
	printf("Starting server...\n");
	
	int count = 0;
	for(;;) 
	{
		printf("-[]-\n* Waiting for the message!\n");
 		zmq_msg_t message;
		zmq_msg_init_size(&message, sizeof(TwoInt));
		zmq_msg_recv(&message, socket_pull, 0);
		TwoInt* ti = (TwoInt*) zmq_msg_data(&message);
		printf("* Message is received: a:%d b:%d number:%d\n", ti->a, ti->b, ti->number);
		zmq_msg_t message_answer;
		zmq_msg_init_size(&message_answer, sizeof(TwoInt));
		TwoInt t_i;
		t_i.a = ti->a;
		t_i.number = ti->number;
		if (t_i.a % 2 == 0) {
			t_i.b = 1;
		}
		else {
			t_i.b = 0;
		}
		memcpy(zmq_msg_data(&message_answer), &t_i, sizeof(TwoInt));
		zmq_msg_send(&message_answer, socket_push, 0);
		printf("* Message sent: a:%d, b:%d, number:%d\n-[]-\n", t_i.a, t_i.b, t_i.number);
		zmq_msg_close(&message_answer);
		zmq_msg_close(&message);
		count++;
		sleep(1);
	}
	zmq_close(socket_pull);
	zmq_close(socket_push);
	zmq_ctx_destroy(context_pull);
	zmq_ctx_destroy(context_push);
	return 0;
}
