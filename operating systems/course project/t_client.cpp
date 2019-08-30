#include <string.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <thread>
#include <iostream>
#include "zmq.h"
#include <vector>

//Operations:
const int CREATE_LOBBY = 1;
const int JOIN_LOBBY = 2;
const int PRINT_LOBBIES = 3;
const int PRINT_MENU = 4;
const int EXIT = 5;
const int GAME = 6;
//Requests:



struct Message {
    int id;
    int num;
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

void SendRequest(void* socket, Request* req) {
	zmq_msg_t msg;
	zmq_msg_init_size(&msg, sizeof(Request));
	memcpy(zmq_msg_data(&msg), req, sizeof(Request));
	zmq_msg_send(&msg, socket, 0);
	zmq_msg_close(&msg);
}

AnswerRep ReceiveRepAnswer(void* socket) {
	zmq_msg_t msg;
	zmq_msg_init_size(&msg, sizeof(AnswerRep));
    zmq_msg_recv(&msg, socket, 0);
	AnswerRep ans;
	memcpy(&ans, zmq_msg_data(&msg), sizeof(AnswerRep));
	zmq_msg_close(&msg);
	return ans;
}

AnswerPub ReceivePubAnswer(void* socket) {
    int* in = new int;
    zmq_msg_t msg;
    zmq_msg_init(&msg);
    zmq_recv(socket, in, sizeof(int), 0);
    zmq_msg_recv(&msg, socket, 0);
    AnswerPub ans;
    memcpy(&ans, zmq_msg_data(&msg), sizeof(AnswerPub));
    zmq_msg_close(&msg);
    free(in);
	return ans;
}

void Menu() {
	std::cout << "[МЕНЮ]\n";
	std::cout << "* Введите 1 для создания лобби.\n";
	std::cout << "* Введите 2 для входа в лобби.\n";
	std::cout << "* Введите 3 для просмотра всех доступных лобби.\n";
	std::cout << "* Введите 4 для повторного вызова меню.\n";
	std::cout << "* Введите 5 для выхода.\n";
}

void PrintField(char field[8][8]) {
    std::cout << " |12345678|" << std::endl;
    std::cout << "-----------" << std::endl;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (j == 0) {
                std::cout << i + 1 << "|" << field[i][j];
            }
            else if (j == 7) {
                std::cout << field[i][j] << "|" << std::endl;
            }
            else {
                std::cout << field[i][j];
            }
        }
    }
    std::cout << "-----------" << std::endl;
}

int CheckProgress(int i1, int j1, int i2, int j2) {
    if (i1 >= 1 && i1 <= 8 && j1 >= 1 && j1 <= 8 && i2 >= 1 && i2 <= 8 && j2 >= 1 && j2 <= 8) {
        if (abs(i1 - i2) >= 0 && abs(i1 - i2) <= 1 && abs(j1 - j2) >= 0 && abs(j1 - j2) <= 1) {
            if ((abs(i1 - i2) != 0 || abs(j1 - j2) != 0) && (abs(i1 - i2) != 1 || abs(j1 - j2) != 1)) {
                return 1;
            }
        }
    }
    return 0;
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
    std::cout << "[CLIENT] Client Starting." << std::endl;
	void* context1 = zmq_ctx_new();
    void* socket_sub = zmq_socket(context1, ZMQ_SUB);
    void* context2 = zmq_ctx_new();
    void* socket_req = zmq_socket(context2, ZMQ_REQ);
    
    std::srand(unsigned(time(0)));
    int id = std::rand() % 1000;
    int conn = zmq_setsockopt(socket_sub, ZMQ_SUBSCRIBE, &id, sizeof(int));
    std::cout << "[CLIENT] Ваш UID: " << id << std::endl;
    zmq_connect(socket_req, "tcp://localhost:4040");
    zmq_connect(socket_sub, "tcp://localhost:4041");
    Menu();
    for (;;) {
        int operation;
        std::cin >> operation;
        if (operation == CREATE_LOBBY) {
            Request req;
            req.type = CREATE_LOBBY;
            req.uid = id;
            SendRequest(socket_req, &req);

            AnswerRep ans = ReceiveRepAnswer(socket_req);
            if (ans.success) {
                std::cout << "[CLIENT] Лобби успешно создано. ID лобби: " << ans.game_id << std::endl;
                std::cout << "[CLIENT] Ожидание соперника." << std::endl;
                AnswerPub ans_p = ReceivePubAnswer(socket_sub);
                if (ans_p.success) {
                    std::cout << "[CLIENT] Соперник присодинился. Его UID: " << ans_p.uid_of_enemy << ". ID игры:  " << ans_p.game_id << std::endl;
                    std::cout << "[CLIENT] Игра началась. Ваш символ на поле - '*'." << std::endl;
                    PrintField(ans_p.field);
                    while (1) {
                        if (CheckField(ans_p.field) == 1) {
                            int i1, j1, i2, j2;
                            std::cout << "[CLIENT] Делайте ход. Подсказка: введите четыре числа." << std::endl;
                            std::cin >> i1 >> j1 >> i2 >> j2;
                            if (CheckProgress(i1, j1, i2, j2)) {
                                if (ans_p.field[i1 - 1][j1 - 1] == ' ' && ans_p.field[i2 - 1][j2 - 1] == ' ') {
                                    Request req_g;
                                    req_g.type = GAME;
                                    req_g.uid = id;
                                    req_g.game_id = ans_p.game_id;
                                    req_g.i1 = i1 - 1;
                                    req_g.i2 = i2 - 1;
                                    req_g.j1 = j1 - 1;
                                    req_g.j2 = j2 - 1;
                                    ans_p.field[i1 - 1][j1 - 1] = '*';
                                    ans_p.field[i2 - 1][j2 - 1] = '*';
                                    SendRequest(socket_req, &req_g);
                                    if (CheckField(ans_p.field) == 0) {
                                        std::cout << "[CLIENT] Вы победили. Игра окончена." << std::endl;
                                        std::cout << "[CLIENT] Вы в меню." << std::endl;
                                        ReceiveRepAnswer(socket_req);
                                        break;
                                    }
                                    ans = ReceiveRepAnswer(socket_req);
                                    if (ans.success) {
                                        std::cout << "[CLIENT] Ход противника..." << std::endl;
                                        ans_p = ReceivePubAnswer(socket_sub);
                                        PrintField(ans_p.field);
                                    }
                                }
                                else {
                                    std::cout << "[CLIENT] Сделан неправильный ход!. Повторите попытку." << std::endl;
                                }
                            }
                            else {
                                std::cout << "[CLIENT] Сделан неправильный ход. Повторите попытку." << std::endl;
                            }
                        }
                        else {
                            std::cout << "[CLIENT] Вы проиграли. Игра окончена." << std::endl;
                            std::cout << "[CLIENT] Вы в меню." << std::endl;
                            break;
                        }
                    }
                }
            }
        }
        if (operation == JOIN_LOBBY) {
            Request req;
            req.type = JOIN_LOBBY;
            req.uid = id;
            std::cout << "[CLIENT] Введите ID лобби, к которому желаете присоединиться: ";
            std::cin >> req.game_id;

            SendRequest(socket_req, &req);
            AnswerRep ans = ReceiveRepAnswer(socket_req);
            if (ans.success) {
                std::cout << "[CLIENT] Удалось присоединиться к лобби. UID вашего соперника: " << ans.uid_of_enemy <<  std::endl;
                std::cout << "[CLIENT] Игра началась. Ваш символ на поле - '#'. Ожидайте хода противника." << std::endl;
                AnswerPub ans_p = ReceivePubAnswer(socket_sub);
                PrintField(ans_p.field);
                while (1) {
                    if (CheckField(ans_p.field) == 1) {
                        int i1, j1, i2, j2;
                        std::cout << "[CLIENT] Делайте ход. Подсказка: введите четыре числа." << std::endl;
                        std::cin >> i1  >> j1 >> i2 >> j2;                  
                        if (CheckProgress(i1, j1, i2, j2)) {
                            if (ans_p.field[i1 - 1][j1 - 1] == ' ' && ans_p.field[i2 - 1][j2 - 1] == ' ') {
                                Request req_g;
                                req_g.type = GAME;
                                req_g.uid = id;
                                req_g.game_id = ans_p.game_id;
                                req_g.i1 = i1 - 1;
                                req_g.i2 = i2 - 1;
                                req_g.j1 = j1 - 1;
                                req_g.j2 = j2 - 1;
                                ans_p.field[i1 - 1][j1 - 1] = '#';
                                ans_p.field[i2 - 1][j2 - 1] = '#';
                                SendRequest(socket_req, &req_g);
                                if (CheckField(ans_p.field) == 0) {
                                    std::cout << "[CLIENT] Вы победили. Игра окончена." << std::endl;
                                    std::cout << "[CLIENT] Вы в меню." << std::endl;
                                    ReceiveRepAnswer(socket_req);
                                    break;
                                }
                                ans = ReceiveRepAnswer(socket_req);
                                if (ans.success) {
                                    std::cout << "[CLIENT] Ход противника..." << std::endl;
                                    ans_p = ReceivePubAnswer(socket_sub);
                                    PrintField(ans_p.field);
                                }
                            }
                            else {
                                std::cout << "[CLIENT] Сделан неправильный ход!. Повторите попытку." << std::endl;
                            }
                        }
                        else {
                            std::cout << "[CLIENT] Сделан неправильный ход. Повторите попытку." << std::endl;
                        }
                    }
                    else {
                        std::cout << "[CLIENT] Вы проиграли. Игра окончена." << std::endl;
                        std::cout << "[CLIENT] Вы в меню." << std::endl;
                        break;
                    }
                }
            }
            else {
                std::cout << "[CLIENT] Присоединиться к лобби не удалось" << std::endl;
            }
        }
        if (operation == PRINT_LOBBIES) {
            Request req;
            req.type = PRINT_LOBBIES;
            req.uid = id;
            SendRequest(socket_req, &req);

            AnswerRep ans = ReceiveRepAnswer(socket_req);
            if (ans.success) {
                std::cout << "[CLIENT] Список доступных лобби: " << std::endl;
                for (int i = 0; ans.lobbies[i] != -1; i++) {
                    std::cout << i + 1 << ". LOBBY ID: " << ans.lobbies[i] << std::endl;
                }
            }
        }
    }
}