#include <cstdio>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <fcntl.h>
const int NUMBER_OF_BYTES = 50;
int main() {
	pid_t pid;
    int fh[2];
    if (pipe(fh))
    {
        std::cout << "Pipe failed." << std::endl;
        exit(1);
    }
	pid = fork();
	if (pid == -1) {
		std::cout << "Fork failed." << std::endl;
		exit(1);
	}
	else if (pid == 0) {
        char rread[NUMBER_OF_BYTES];
        int r = read(fh[0], rread, NUMBER_OF_BYTES);
        if (r == -1) {
            std::cout << "Read failed." << std::endl;
            exit(1);
        }
        int number_1 = 0, number_2 = 0, i = 0, j, answer;
        std::cout << "* Начало вычислений (pid: " << getpid() << ")" << std::endl;
        while (rread[i] != '+' && rread[i] != '-') {
            number_1 *= 10;
            number_1 += rread[i] - '0';
            i++;
        }
        j = i + 1;
        while (rread[j] != '\0') {
            number_2 *= 10;
            number_2 += rread[j] - '0';
            j++;
        }
        if (rread[i] == '+') {
            answer = number_1 + number_2;
        }
        else if (rread[1] = '-') {
            answer = number_1 - number_2;
        }
        char answer_c[NUMBER_OF_BYTES];
        for (int i = 0; i < NUMBER_OF_BYTES; i++) {
            answer_c[i] = '\0';
        }
        i = 0;
        int minus = 0;
        if (answer < 0) {
            minus = 1;
            answer = -answer;
        }
        while (answer != 0) {
            answer_c[i] = answer % 10 + '0';
            answer /= 10;
            i++;
        }
        if (minus == 1) {
            answer_c[i] = '-';
        }
        int w = write(fh[1], answer_c, NUMBER_OF_BYTES);
        if (w == -1) {
            std::cout << "Write failed." << std::endl;
            exit(1);
        }
	}
	else {
        std::cout << "[КАЛЬКУЛЯТОР (pid процесса: " << getpid() <<  ")] " << std::endl;
        std::cout << "* Введите запись типа: a+b или a-b: " << std::endl;
        std::string c;
        getline(std::cin, c);
        char rread[NUMBER_OF_BYTES];
        for (int i = 0; i < NUMBER_OF_BYTES; i++) {
            rread[i] = '\0';
        }
        for (int i = 0; i < c.length(); i++) {
            rread[i] = c[i];
        }
        int w = write(fh[1], rread, NUMBER_OF_BYTES);
        if (w == -1) {
            std::cout << "Write failed." << std::endl;
            exit(1);
        }
		int status;
        if (waitpid(-1, &status, 0) == -1) {
            std::cout << "Waitpid failed." << std::endl;
            exit(1);
        } 
        int r = read(fh[0], rread, NUMBER_OF_BYTES);
        if (r == -1) {
            std::cout << "Read failed." << std::endl;
            exit(1);;
        }
        std::cout << "* Результат: ";
        for (int i = 49; i >= 0; i--) {
            if (rread[i] != '\0') {
                std::cout << rread[i];
            }
        }
        std::cout << std::endl;
		exit(0);
    }
}
