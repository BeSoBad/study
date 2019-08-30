#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdio>
#include <sys/file.h>
const int DELETE = 2;
const int REPLACE = 1;
const int SUFFIX = 1;
const int PREFIX = 2;
const int ANY = 3;
void menu() {
    std::cout << "Введите 1 для вывода строки файла" << std::endl;
    std::cout << "Введите 2 для поиска подстроки" << std::endl;
    std::cout << "Введите 3 для замены/удаления фрагмента строки" << std::endl;
    std::cout << "Введите 4 для проверки, открыт ли файл в других процессах" << std::endl;
    std::cout << "Введите 7 для повторного вызова меню" << std::endl;
    std::cout << "Введите 0 для выхода" << std::endl;
}
int main(int argc, char *argv[])
{
    int fdin, fdout;
    void *src, *dst;
    struct stat statbuf;
    if (argc <= 1) {
        std::cout << "Используйте: ./a.out /file:path" << std::endl;
        return 0;
    }
    menu();
    while (1) {
        int menu_answer;
        std::cin >> menu_answer;
        if (menu_answer == 1) {
            void* ptr;
            fdin = open(argv[1] + 6, O_RDONLY);
            struct stat statbuf;
            fstat(fdin, &statbuf);
            std::cout << "* Введите номер строки: ";
            int num_of_string, status = 0, list_cnt = 0, string_cnt = 1;
            std::cin >> num_of_string;
            std::cout << "* Результат:" << std::endl;
            if (num_of_string == 1) {
                status = 1;
            }
            while (getpagesize() + getpagesize() * list_cnt < statbuf.st_size + getpagesize()) {
                if (list_cnt > 0) {
                    munmap(src, getpagesize());
                }
                ptr = mmap(0, getpagesize(), PROT_READ, MAP_SHARED, fdin, getpagesize() * list_cnt);
                int i;
                for (i = 0; i < getpagesize(); i++) {
                    if (i + getpagesize() * list_cnt == statbuf.st_size - 1) {
                        break;
                    }
                    if (status == 0) {
                        if (*((char*)ptr + i) == '\n') {
                            string_cnt++;
                        }
                        if (string_cnt == num_of_string) {
                            status = 1;
                        }
                    }
                    else if (status == 1) {
                        if (*((char*)ptr + i) != '\n') {
                            std::cout << *((char*)ptr + i);
                        }
                        else {
                            status = 2;
                            break;
                        }
                    }
                }
                if (status == 2) {
                    std::cout << std::endl;
                    break;
                }
                list_cnt++;
            }
            if (status == 0) {
                std::cout << "* Строка не найдена" << std::endl;
            }
            close(fdin);
        }
        if (menu_answer == 2) {
            void* ptr;
            fdin = open(argv[1] + 6, O_RDONLY);
            struct stat statbuf;
            fstat(fdin, &statbuf);
            std::cout << "* Ввод для поиска: 1 - по суффиксу, 2 - по префиксу, 3 - по любой части: ";
            int search, border;
            std::cin >> search;
            std::string str;
            std::cout << "* Введите подстроку: ";
            std::cin >> str;
            if (statbuf.st_size % getpagesize() == 0) {
                border = statbuf.st_size / getpagesize();
            }
            else {
                border = statbuf.st_size / getpagesize() + 1;
            }
             //Будем сохранять часть слова до и после вхождения, которая получается.
            std::string before_answer;
            std::string after_answer;
            for (int i = 0; i < border; i++) {
                int bborder, pos_text = 0;
                if (i == border - 1) {
                    bborder = statbuf.st_size - getpagesize() * i;
                }
                else {
                    bborder = getpagesize();
                }
                ptr = mmap(0, getpagesize(), PROT_READ, MAP_SHARED, fdin, getpagesize() * i);
                for (int j = 0; j < bborder; j++) {
                    pos_text = j;
                    int offset = 0, looked_next_block = 0; //Смещение, смотрели ли следующие блоки
                    if (*((char*)ptr + j) == ' ' || *((char*)ptr + j) == '\n' || *((char*)ptr + j) == '\t') {
                        before_answer = "";
                    }
                    else {
                        before_answer += *((char*)ptr + j);
                    }
                    for (int k = 0; k < str.length(); k++) { // Цикл, проверяющий вхождение паттерна
                        //std::cout << "!!!" << std::endl;
                        if (*((char*)ptr + pos_text) == str[k]) {
                            if (k == str.length() - 1) { //Если последний символ паттерна, то есть вся подстрока совпала.
                                //Ищем всё слово и символ, который за ним идёт
                                int offset_for_separator = offset, pos_for_separator = pos_text + 1, looked_next_block_for_separator = 0, found_separator = 0;
                                for (int ii = i + offset; ii < border; ii++) { //Идём от текущего блока
                                    std::cout << "@!!!: " << ii << std::endl;
                                    if (ii > i + offset) { // Если мы не на изначальном блоке, с которого начинали идти вперёд, то отображаем следующий блок
                                        offset_for_separator++;
                                        if (looked_next_block_for_separator == 0) {
                                            looked_next_block_for_separator = 1;
                                        }
                                        munmap(ptr, getpagesize());
                                        ptr =  mmap(0, getpagesize(), PROT_READ, MAP_SHARED, fdin, getpagesize() * ii); // Отображаем память след. блока
                                        pos_for_separator = 0;
                                    }
                                    int sborder;
                                    if (ii == border - 1) { // Определяем границу 
                                        sborder = statbuf.st_size - getpagesize() * ii;
                                    }
                                    else {
                                        sborder = getpagesize();
                                    }
                                    std::cout << "~!!!: " << ii << std::endl;
                                    std::cout << "$!!!: " << pos_for_separator << " " << sborder << " " << ii << " " << border << std::endl;
                                    for (int jj = pos_for_separator; jj < sborder; jj++) { //Идём от позиции до границы
                                        //std::cout << "!!!: " << jj << std::endl;
                                        //std::cout << "#!!!: " << after_answer << std::endl;
                                        if (*((char*)ptr + jj) == ' ' || *((char*)ptr + jj) == '\n' || *((char*)ptr + jj) == '\t' || (ii == border - 1 && jj == sborder - 1)) {
                                            found_separator = 1;
                                            break;
                                        }
                                        else {
                                            after_answer += *((char*)ptr + jj);
                                        }
                                    }
                                    if (found_separator == 1) {
                                        break;
                                    }
                                }
                                if (looked_next_block_for_separator == 1) { // Если отображали след. блоки, то возвращаем отображение изначального блока
                                    munmap(ptr, getpagesize());
                                    ptr = mmap(0, getpagesize(), PROT_READ, MAP_SHARED, fdin, getpagesize() * (i + offset));
                                }
                                //
                                before_answer.pop_back();
                                int left_pos = j - before_answer.length(), right_pos = j + str.length() + after_answer.length() - 1;
                                if (search == SUFFIX) { // Проверяем, если происходит поиск по суффиксу
                                    if (after_answer == "") {
                                        std::cout << "FIND: левая позиция - " << left_pos << ", правая позиция - "  << right_pos << "." << std::endl;
                                        std::cout << "СЛОВО: " << std::endl << before_answer + str << std::endl;
                                    }
                                    else {
                                        break; //Иначе, поиск неудался
                                    }
                                }
                                else if (search == PREFIX) {
                                    if (before_answer == "") {
                                        std::cout << "FIND: левая позиция - " << left_pos << ", правая позиция - "  << right_pos << "." << std::endl;
                                        std::cout << "СЛОВО: " << std::endl << str + after_answer << std::endl;
                                    }
                                }
                                else if (search == ANY) {
                                    std::cout << "FIND: левая позиция - " << left_pos << ", правая позиция - "  << right_pos << "." << std::endl;
                                    std::cout << "СЛОВО: " << std::endl << before_answer + str + after_answer << std::endl;
                                }
                            }
                            else if (pos_text == bborder - 1) { // Иначе, если стоим на границе
                                if (i + offset < border - 1) { // Проверяем, что мы не на последнем блоке. (на последнем блоке не можем дальше выделить память)
                                    std::cout << "offset: " << offset << std::endl;
                                    offset++; //Смещаем на 1 оффсет
                                    if (looked_next_block == 0) {
                                        looked_next_block = 1;
                                    }
                                    munmap(ptr, getpagesize());
                                    ptr =  mmap(0, getpagesize(), PROT_READ, MAP_SHARED, fdin, getpagesize() * (i + offset)); // Отображаем память след. блока
                                    pos_text = 0;
                                    if (i + offset == border - 1) {    //Меняем bborder, т.к. можем дойти до последнего блока
                                        bborder = statbuf.st_size - getpagesize() * (i + offset); // Учитываем, что последний блок не обязательно 4096 байтов
                                    }
                                    else {
                                        bborder = getpagesize();
                                    }
                                }
                                else {
                                    break;
                                }
                            }
                            else {
                                pos_text++;
                            }
                        }
                        else {
                            break;
                        }
                    }
                    if (looked_next_block == 1) { // Если отображали след. блоки, то возвращаем отображение изначального блока
                        munmap(ptr, getpagesize());
                        ptr = mmap(0, getpagesize(), PROT_READ, MAP_SHARED, fdin, getpagesize() * i);
                    }
                }
            }
            close(fdin);
        }
        if (menu_answer == 3) {
            void* ptr;
            chmod(argv[1] + 6, S_IREAD | S_IWRITE);
            fdin = open(argv[1] + 6, O_RDWR);
            std::cout << "::! " << fdin << std::endl;
            struct stat statbuf;
            fstat(fdin, &statbuf);
            int num_of_string, status = 0, list_cnt = 0, string_cnt = 1, left, right, action;
            std::string str;
            std::cout << "* Введите 1 для замены, 2 для удаления: ";
            std::cin >> action;
            std::cout << "* Введите левую границу: ";
            std::cin >> left;
            std::cout << "* Введите правую границу: ";
            std::cin >> right;
            if (action == REPLACE) {
                std::cout << "* Введите строку: ";
                std::cin >> str;
                list_cnt = left / getpagesize();
                int to_end = right / getpagesize() - list_cnt;
                std::cout << "L: " << list_cnt << " " << to_end << std::endl;
                for (int i = 0; i <= to_end; i++) {
                    if (i > 0) {
                        munmap(ptr, getpagesize());
                    }
                    ptr = mmap(0, getpagesize(),  PROT_READ | PROT_WRITE, MAP_SHARED, fdin, getpagesize() * (list_cnt + i));
                    if (i == 0 && i == to_end) {
                        for (int j = left - getpagesize() * list_cnt - 1; j <= right - getpagesize() * list_cnt; j++) {
                                *((char*)ptr + j) = str[j - left + getpagesize() * list_cnt];
                        }
                    }
                    else if (i == 0) {
                        for (int j = left - getpagesize() * list_cnt - 1; j <= getpagesize(); j++) {
                            *((char*)ptr + j) = str[i + getpagesize() * i];
                        }
                    }
                    else if (i == to_end) {
                        for (int j = 0; j <= right - getpagesize() * list_cnt; j++) {
                            *((char*)ptr + j) = str[i + getpagesize() * i];
                        }
                    }
                    else {
                        for (int j = 0; j <= getpagesize(); j++) {
                            *((char*)ptr + j) = str[i + getpagesize() * i];
                        }
                    }
                }
                std::cout << "* Запись выполнена" << std::endl;
            }
            if (action == DELETE) {
                void* tptr;
                int fdtemp = open("temp_file_", O_RDWR | O_CREAT);
                chmod("temp_file_", S_IREAD | S_IWRITE);
                lseek(fdtemp, statbuf.st_size - (right - left + 1), SEEK_SET);
                write(fdtemp, "", 1);
                list_cnt = left / getpagesize();
                int to_end = right / getpagesize(), current_recordable_byte = 0;
                std::cout << "INFO: " << list_cnt << " " << to_end << std::endl;
                for (int i = 0; i <= list_cnt; i++) {
                    ptr = mmap(0, getpagesize(), PROT_READ, MAP_SHARED, fdin, getpagesize() * i);
                    tptr = mmap(0, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, fdtemp, getpagesize() * i);
                    if (i == list_cnt) {
                        memcpy(tptr, ptr, left - getpagesize() * list_cnt);
                        current_recordable_byte += left - getpagesize() * list_cnt;
                    }
                    else {
                        memcpy(tptr, ptr, getpagesize());
                        current_recordable_byte += getpagesize();
                    }
                    munmap(ptr, getpagesize());
                    munmap(tptr, getpagesize());
                }
                for (int i = to_end; i <= statbuf.st_size / getpagesize(); i++) {
                    ptr = mmap(0, getpagesize(), PROT_READ, MAP_SHARED, fdin, getpagesize() * i);
                    tptr = mmap(0, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, fdtemp, (current_recordable_byte / getpagesize()) * getpagesize());
                    if (i == to_end) {
                        char *from, *to;
                        int number_of_bytes;
                        from = (char*)ptr + right - getpagesize() * to_end + 1;
                        to = (char*)tptr + current_recordable_byte - (current_recordable_byte / getpagesize()) * getpagesize();
                        if (to_end == statbuf.st_size / getpagesize()) {
                            number_of_bytes = statbuf.st_size - right;
                            memcpy(to, from, number_of_bytes);
                        }
                        else {
                            number_of_bytes = getpagesize() - right + getpagesize() * to_end;
                            memcpy(to, from, number_of_bytes);
                        }
                        current_recordable_byte += number_of_bytes;
                        break;
                    }
                    else {
                        char *from, *to;
                        int number_of_bytes;
                        from = (char*)ptr;
                        to = (char*)tptr + current_recordable_byte - (current_recordable_byte / getpagesize()) * getpagesize();
                        number_of_bytes = getpagesize();
                        memcpy(to, from, number_of_bytes);
                        current_recordable_byte += getpagesize();
                    }
                    munmap(ptr, getpagesize());
                    munmap(tptr, getpagesize());
                }
                close(fdtemp);
            }
            close(fdin);
            if (rename("temp_file_", argv[1] + 6) == -1) {
               // std::cout << "FAIL" << std::endl;
            }
            else {
                std::cout << "DONE" << std::endl;
            }
        }
        if (menu_answer == 4) {
            fdin = open(argv[1] + 6, O_RDWR);
            if (flock(fdin, LOCK_EX | LOCK_NB) == -1) {
                //std::cout << errno << std::endl;
                if (errno == EWOULDBLOCK) {
                    std::cout << "Файл уже заблокирован другим процессом" << std::endl;
                }
            }
            else {
                std::cout << "Файл нигде не открыт" << std::endl;
            }
        }
        if (menu_answer == 7) {
            menu();
        }
        if (menu_answer == 0) {
            break;
        }
    }
}
