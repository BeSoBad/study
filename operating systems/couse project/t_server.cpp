#include <string.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <thread>
#include <iostream>
#include "zmq.h"


struct Message {
    int id;
    int num;
};

struct sockets {
    Message m;
    void* socket_pub;
};

void* answer(void* args) {
    zmq_msg_t msg;
    sockets* sock = (sockets*) args;
    void* socket_pub = sock->socket_pub;
    zmq_msg_init_size(&msg, sizeof(Message));
    memcpy(zmq_msg_data(&msg), &(sock->m), sizeof(Message));
    sleep(3);
    zmq_msg_send(&msg, socket_pub, 0);
    zmq_msg_close(&msg);
}

int main() {
    printf("[server] server starting.\n");
	void* context1 = zmq_ctx_new();
    void* socket_pub = zmq_socket(context1, ZMQ_PUB);
    void* context2 = zmq_ctx_new();
    void* socket_pull = zmq_socket(context2, ZMQ_REP);
    zmq_bind(socket_pull, "tcp://*:4040");
    zmq_bind(socket_pub, "tcp://*:4041");
    int count = 0;
    for (;;) {
        zmq_msg_t message;
		zmq_msg_init_size(&message, sizeof(Message));
		zmq_msg_recv(&message, socket_pull, 0);
		printf("[SERVER] Получен запрос номер %d.\n", count);
        Message* m = (Message*) zmq_msg_data(&message);
        std::cout << "[SERVER]" << " " << m->num << std::endl;
        
        sockets sock;
        sock.socket_pub = socket_pub;
        sock.m = *m;
        //answer((void*)&sock);
        void* status;
        pthread_t tid;
        pthread_create(&tid, NULL, answer, &sock);
        count++;
        sleep(1);
    }
}
