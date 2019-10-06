#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
const int SUCCES = 0;
const int ERROR_CREATE_THREAD = -11;
const int ERROR_JOIN_THREAD = -12;

struct Part {
    std::string* arr;
    int left;
    int right;
};

int cnt = 0;

void* quickSort(void* args) {
    Part* item = (Part*) args;
    int i = item->left, j = item->right;
    int tmp;
    int pivot = item->arr[(i + j) / 2];
    while (i <= j) {
        while (item->arr[i] < pivot)
        i++;
        while (item->arr[j] > pivot)
        j--;
        if (i <= j) {
            tmp = item->arr[i];
            item->arr[i] = item->arr[j];
            item->arr[j] = tmp;
            i++;
            j--;
        }
    }
    Part p1, p2;
    if (item->left < j) {
        pthread_t th;
        p1.arr = item->arr;
        p1.left = item->left;
        p1.right = j;
        int status, status_addr;
        status = pthread_create(&th, NULL, quickSort, &p1);
        if (status != SUCCES) {
            std::cout << "[THREAD CREATE] Error" << std::endl;
            exit(ERROR_CREATE_THREAD);
        }
        status = pthread_join(th, (void**)&status_addr);
        if (status != SUCCES) {
            std::cout << "[THREAD JOIN] Error" << std::endl;
            exit(ERROR_JOIN_THREAD);
        }
    }
    if (i < item->right) {
        pthread_t th;
        p2.arr = item->arr;
        p2.left = i;
        p2.right = item->right;
        int status, status_addr;
        status = pthread_create(&th, NULL, quickSort, &p2);
        if (status != SUCCES) {
            std::cout << "[THREAD CREATE] Error" << std::endl;
            exit(ERROR_CREATE_THREAD);
        }
        status = pthread_join(th, (void**)&status_addr);
        if (status != SUCCES) {
            std::cout << "[THREAD JOIN] Error" << std::endl;
            exit(ERROR_JOIN_THREAD);
        }
    }
    cnt++;
    return NULL;
}

void print(int arr[], int n) {
   for (int i = 0; i < n; i++) {
    std::cout << arr[i] << " ";
   }
   std::cout << std::endl;
}

int main () {
    int n, i;
    std::cout << "* Array size: ";
    std::cin >> n;
    std::string* arr = new std::string [n];
    for (i = 0; i < n; i++) {
        std::cout << "* Array[" << i + 1 << "]: ";
        std::cin >> arr[i];
    }
    Part item = {arr, 0, n - 1};
    std::cout << "* Inputed array: ";
    print(arr, n);
    pthread_t basic_stream;
    int status, status_addr;
    status = pthread_create(&basic_stream, NULL, quickSort, &item);
    if (status != SUCCES) {
        std::cout << "[THREAD CREATE] Error" << std::endl;
        exit(ERROR_CREATE_THREAD);
    }
    status = pthread_join(basic_stream, (void**) &status_addr);
    if (status != SUCCES) {
        std::cout << "[THREAD JOIN] Error" << std::endl;
        exit(ERROR_JOIN_THREAD);
    }
    std::cout << "* Result: ";
    print(arr,n);
    std::cout << "Number of used threads: " << cnt << std::endl;
    delete[] arr;
}
