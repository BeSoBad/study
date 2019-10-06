#include <string.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <thread>
#include <iostream>
#include <vector>
#include <map>
#include "zmq.h"


//Operations:
const int CREATE_LOBBY = 1;
const int JOIN_LOBBY = 2;
const int PRINT_LOBBIES = 3;
const int GAME = 6;

struct Message {
    int id;
    int num;
};


struct sockets {
    Message m;
    void* socket_pub;
};

struct Request {
	int type;
    int uid;
    int game_id;
    int i1, j1, i2, j2;
};

struct AnswerRep {
	int success;
    int code;
    int lobbies[50];
    int uid_of_enemy;
    int game_id;
};

struct AnswerPub {
	int success;
    int type_of_answer;
    int game_id;
    int uid_of_enemy;
    char field[8][8];
};

struct Game {
    int p1;
    int p2;
    char field[8][8];
};

void SendPubAnswer(void* socket, AnswerPub* ans, int receiver) {
    zmq_msg_t msg;
    zmq_msg_init_size(&msg, sizeof(AnswerPub));
    memcpy(zmq_msg_data(&msg), ans, sizeof(AnswerPub));
    zmq_send(socket, &receiver, sizeof(int), ZMQ_SNDMORE);
    std::cout << "[SERVER] Sending to filter id: " << receiver << std::endl;
    zmq_msg_send(&msg, socket, 0);
    zmq_msg_close(&msg);
}

void SendAnswer(void* socket, AnswerRep* ans) {
	zmq_msg_t msg;
	zmq_msg_init_size(&msg, sizeof(AnswerRep));
	memcpy(zmq_msg_data(&msg), ans, sizeof(AnswerRep));
	zmq_msg_send(&msg, socket, 0);
	zmq_msg_close(&msg);
}

int CheckField(char field[8][8]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 7; j++) {
            if (field[i][j] == ' ' && field[i][j + 1] == ' ') {
                return 1;
            }
        }
    }
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 8; j++) {
            if (field[i][j] == ' ' && field[i + 1][j] == ' ') {
                return 1;
            }
        }
    }
    return 0;
}

int main() {
    std::cout << "[SERVER] Server starting." << std::endl;
	void* context1 = zmq_ctx_new();
    void* socket_pub = zmq_socket(context1, ZMQ_PUB);
    void* context2 = zmq_ctx_new();
    void* socket_rep = zmq_socket(context2, ZMQ_REP);
    zmq_bind(socket_rep, "tcp://*:4040");
    zmq_bind(socket_pub, "tcp://*:4041");
    std::map < int, Game > games;
    int count = 1;
    for (;;) {
        zmq_msg_t message;
		zmq_msg_init_size(&message, sizeof(Request));
		zmq_msg_recv(&message, socket_rep, 0);
		Request req;
        memcpy(&req, zmq_msg_data(&message), sizeof(Request));
        std::cout << "[SERVER] Получен запрос номер " << count << " от UID: " << req.uid << std::endl;        
        if (req.type == CREATE_LOBBY) {
            std::srand(unsigned(time(0)));
            int num_of_lobby = std::rand() % 100000;
            Game g;
            g.p1 = req.uid;
            g.p2 = -1;
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    g.field[i][j] = ' ';
                }
            }
            games[num_of_lobby] = g;
            std::cout << "[SERVER] [LOBBY] Лобби создано. Номер лобби: " << num_of_lobby << ". UID игрока: " << games[num_of_lobby].p1 << "." << std::endl;
            AnswerRep ans;
            ans.success = 1;
            ans.code = -1;
            ans.game_id = num_of_lobby;
            SendAnswer(socket_rep, &ans);
        }
        if (req.type == JOIN_LOBBY) {
            AnswerRep ans;
            if (games.count(req.game_id) > 0) {
                if (games[req.game_id].p2 == -1) {
                    games[req.game_id].p2 = req.uid;
                    ans.success = 1;
                    ans.code = -1;
                    ans.uid_of_enemy = games[req.game_id].p1;
                    AnswerPub ans_p;
                    ans_p.success = 1;
                    ans_p.type_of_answer = 1;
                    ans_p.game_id = req.game_id;
                    ans_p.uid_of_enemy = req.uid;
                    for (int i = 0; i < 8; i++) {
                        for (int j = 0; j < 8; j++) {
                            ans_p.field[i][j] = games[req.game_id].field[i][j];
                        }
                    }
                    SendPubAnswer(socket_pub, &ans_p, ans.uid_of_enemy);
                }
                else {
                    ans.success = 0;
                    ans.code = -1;
                    ans.uid_of_enemy = -1;
                }
            }
            else {
                ans.success = 0;
                ans.code = -1;
                ans.uid_of_enemy = -1;
            }
            SendAnswer(socket_rep, &ans);  
        }
        if (req.type == PRINT_LOBBIES) {
            AnswerRep ans;
            ans.success = 1;
            ans.code = -1;
            int j = 0;
            for (auto i: games) {
                ans.lobbies[j] = i.first;
                j++;
            }
            ans.lobbies[j] = -1;
            SendAnswer(socket_rep, &ans);
        }
        if (req.type == GAME) {
            AnswerRep ans;
            ans.success = 1;
            ans.code = -1;
            AnswerPub ans_p;
            ans_p.success = 1;
            ans_p.game_id = req.game_id;
            if (games[req.game_id].p1 == req.uid) {
                games[req.game_id].field[req.i1][req.j1] = '*';
                games[req.game_id].field[req.i2][req.j2] = '*';
            }
            else if (games[req.game_id].p2 == req.uid) {
                games[req.game_id].field[req.i1][req.j1] = '#';
                games[req.game_id].field[req.i2][req.j2] = '#';
            }
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    std::cout << games[req.game_id].field[i][j];
                    ans_p.field[i][j] = games[req.game_id].field[i][j];
                }
                std::cout << std::endl;
            }
            if (games[req.game_id].p1 == req.uid) {
                SendPubAnswer(socket_pub, &ans_p, games[req.game_id].p2);
            }
            else if (games[req.game_id].p2 == req.uid) {
                SendPubAnswer(socket_pub, &ans_p, games[req.game_id].p1);
            }
            SendAnswer(socket_rep, &ans);
            if (CheckField(games[req.game_id].field) == 0) {
                games.erase(req.game_id);
            }
        }
        count++;
    }
}