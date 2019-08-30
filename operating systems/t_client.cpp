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

int main() {
    printf("[CLIENT] Client Starting.\n");
	void* context1 = zmq_ctx_new();
    void* socket_sub = zmq_socket(context1, ZMQ_SUB);
    void* context2 = zmq_ctx_new();
    void* socket_push = zmq_socket(context2, ZMQ_REQ);
    std::srand(unsigned(time(0)));
    const int id = rand() % 1001;
    const int* filter = &id;
    int conn = zmq_setsockopt(socket_sub, ZMQ_SUBSCRIBE, filter, sizeof(int));
    zmq_connect(socket_push, "tcp://localhost:4040");
    zmq_connect(socket_sub, "tcp://localhost:4041");
    for (;;) {
        zmq_msg_t msg;
        zmq_msg_init_size(&msg, sizeof(Message));
        std::srand(unsigned(time(0)));
        int a = std::rand()%10000;
        Message m;
        m.id = id;
        m.num = a;
        memcpy(zmq_msg_data(&msg), &m, sizeof(Message));
        zmq_msg_send(&msg, socket_push, 0);
        std::cout << "[CLIENT1] Send: " << a << " from id " << id <<  std::endl;
        zmq_msg_close(&msg);

        zmq_msg_t msg_ans;
        zmq_msg_init_size(&msg_ans, sizeof(int));
        zmq_msg_recv(&msg_ans, socket_sub, 0);
        int b;
        Message mm;
        memcpy(&mm, zmq_msg_data(&msg_ans), sizeof(Message));
        zmq_msg_close(&msg_ans);
        std::cout << "[CLIENT2] Receive: " << mm.num << std::endl;
    }
}