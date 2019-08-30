#include <stdio.h>
#include <stdlib.h>

const int MAX_LENGTH_OF_NUM = 21;

typedef struct {
    unsigned long long number;
    int size;
    int capacity;
    char* str;
} TVectorItem;
typedef struct {
    TVectorItem* data;
    int size;
    int capacity;
} TVector;

void DeleteTVectorItem(TVectorItem* tvi);

unsigned long long Pow(unsigned long long a, unsigned long long b) {
	unsigned long long tmp = 1;
	for (int i = 0; i < b; i++) {
        tmp *= a;
    }
	return tmp;
}

void CreateTVector(TVector* v) {
    v->capacity = 2;
    v->size = 0;
    v->data = (TVectorItem*)malloc(sizeof(TVectorItem) * v->capacity);
}
void SCreateTVector(TVector* v, int size) {
    v->capacity = 2;
    while (v->capacity < size) {
        v->capacity *= 2;
    }
    v->size = size;
    v->data = (TVectorItem*)malloc(sizeof(TVectorItem) * v->capacity);
}
void PushTVector(TVector* v, TVectorItem* tvi) {
    if (v->size == v->capacity) {
        v->data = (TVectorItem*)realloc(v->data, sizeof(TVectorItem) * v->capacity * 2);
        v->capacity *= 2;
    }
    v->data[v->size] = *tvi;
    v->size++;
}
void DeleteTVector(TVector* v) {
    for (int i = 0; i < v->size; i++) {
        DeleteTVectorItem(&v->data[i]);
    }
    free(v->data);
    v->data = NULL;
}
void CreateTVectorItem(TVectorItem* tvi) {
    tvi->capacity = 2;
    tvi->size = 0;
    tvi->str = (char*)malloc(sizeof(char) * tvi->capacity);
}
void PushTVectorItem(TVectorItem* tvi, char c) {
    if (tvi->size == tvi->capacity) {
        tvi->str = (char*)realloc(tvi->str, sizeof(char) * tvi->capacity * 2);
        tvi->capacity *= 2;
    }
    tvi->str[tvi->size] = c;
    tvi->size++;
}
void DeleteTVectorItem(TVectorItem* tvi) {
    free(tvi->str);
}
int main() {
    TVector elements;
    CreateTVector(&elements);
    unsigned long long inputed_key;
    while (scanf("%llu\t", &inputed_key) != EOF) {
        char tmp_char;
        TVectorItem new_tvi;
        CreateTVectorItem(&new_tvi);
        new_tvi.number = inputed_key;
        while (1) {
            tmp_char = getchar();
            if (tmp_char == '\n' || tmp_char == EOF) {
                break;
            }
            PushTVectorItem(&new_tvi, tmp_char);
        }
        PushTVector(&elements, &new_tvi);
    }
    TVector b_elements;
    SCreateTVector(&b_elements, elements.size);
    for (int j = 0; j < elements.size; j++) {
        b_elements.data[j].number = 0;
    }
	for (int i = 1; i < MAX_LENGTH_OF_NUM; i++) {
		int c[10];
		for (int j = 0; j < 10; j++) {
            c[j] = 0;
        }
		for (int j = 0; j < elements.size; j++) {
				c[(elements.data[j].number / Pow(10, i - 1)) % 10]++;
		}
		for (int j = 1; j < 10; j++) {
            c[j] += c[j - 1];
        }
		for (int j = elements.size - 1; j >= 0; j--) {
			if (c[(elements.data[j].number / Pow(10, i - 1)) % 10] > 0) {
				b_elements.data[c[(elements.data[j].number / Pow(10, i - 1)) % 10] - 1] = elements.data[j];
				c[(elements.data[j].number / Pow(10, i - 1)) % 10]--;
			}
		}
		for (int j = 0; j < elements.size; j++) {
            elements.data[j] = b_elements.data[j];
        }
    }
    for (int i = 0; i < elements.size; i++) {
        printf("%llu\t", elements.data[i].number);
        for (int j = 0; j < elements.data[i].size; j++) {
            printf("%c", elements.data[i].str[j]);
        }
        printf("\n");
    }
    free(b_elements.data);
    DeleteTVector(&elements);
    return 0;
}
