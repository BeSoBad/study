#include <iostream>
#include <cstdlib>
#include <cstdio>

const int RED = 11;
const int BLACK = 22;

struct TField {
    uint64_t num;
    char str[256];
};

struct TString {
    char* data;
    int capacity;
    int size;
};

struct TNode {
    TNode* left;
    TNode* right;
    TNode* parent;
    uint64_t number;
    int color;
    TString key;
};

#define NIL &nil
TNode nil = {NIL, NIL, NIL, 0, BLACK, 0};
const int MAX_LENGTH_OF_KEY = 256;

class TRBTree {
public:
    TNode* root;
    TRBTree();
    TNode* SearchSpecial(TNode* node, TString key);
    void Save(FILE* file, TNode* node);
    void Load(FILE* file);
    void Transplant(TNode* a, TNode* b);
    void TurnLeft(TNode* node);
    void TurnRight(TNode* node);
    void RemoveFixup(TNode* node);
    void InsertFixup(TNode* node);
    void DeleteSpecial(TNode* node); 
    void Insert(TString key, uint64_t num, int* succes);  
    void Remove(TString key);
    void RemoveMinimum(TNode* node, TNode* parent_node);
    void Delete();
    void Search(TString key);
};


int Min(int a, int b) {
    return (a >= b) ? b:a;
}

int CompareStrings(TString& a, TString& b) {
    int i;
    for (i = 0; i < Min(a.size, b.size); i++) {
        if (a.data[i] > b.data[i]) {
            return 1;
        }
        else if (a.data[i] < b.data[i]) {
            return 0;
        }
    }
    if (a.size > b.size) {
        return 1;
    }
    else if (a.size < b.size) {
        return 0;
    }
    return -1;
}

void CreateTString(TString* str) {
    str->capacity = 2;
    str->size = 0;
    str->data = (char*) malloc(sizeof(char) * 2);
}

void PushTString(TString* str, char c) {
    if (str->size == str->capacity) {
        str->data = (char*) realloc(str->data, str->capacity * 2);
        str->capacity *= 2;
    }
    str->data[str->size] = c;
    str->size++;
}

void DeleteTString(TString* str) {
    free(str->data);
}


void CreateTNode(TNode* node, uint64_t num, TString str, int color) {
    node->left = NIL;
    node->right = NIL;
    node->color = color;
    node->number = num;
    node->parent = NIL;
    node->key.data = str.data;
    node->key.size = str.size;
    node->key.capacity = str.capacity;
}


void DeleteTNode(TNode* node) {
    DeleteTString(&(node->key));  
    free(node);
}

void TRBTree::InsertFixup(TNode* node) {
    TNode* uncle;
    while (node->parent->color == RED) {
        if (node->parent == node->parent->parent->left) {
            uncle = node->parent->parent->right;
            if (uncle->color == RED) {
                uncle->color = BLACK;
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else {
                if (node == node->parent->right) {
                    node = node->parent;
                    TurnLeft(node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                TurnRight(node->parent->parent);
            }
        }
        else {
            uncle = node->parent->parent->left;
            if (uncle->color == RED) {
                uncle->color = BLACK;
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else { 
                if (node == node->parent->left) {
                    node = node->parent;
                    TurnRight(node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                TurnLeft(node->parent->parent);
            }
        }
    }
    root->color = BLACK;
}   

void TRBTree::RemoveFixup(TNode* node) {
    while (node != root && node->color == BLACK) {
        if (node == node->parent->left) {
            TNode* w = node->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                node->parent->color = RED;
                TurnLeft(node->parent);
                w = node->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                node = node->parent;
            }
            else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    TurnRight(w);
                    w = node->parent->right;
                }
                w->color = node->parent->color;
                node->parent->color = BLACK;
                w->right->color = BLACK;
                TurnLeft(node->parent);
                node = root;
            }
        }
        else {
            TNode* w = node->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                node->parent->color = RED;
                TurnRight(node->parent);
                w = node->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                node = node->parent;
            }
            else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    TurnLeft(w);
                    w = node->parent->left;
                }
                w->color = node->parent->color;
                node->parent->color = BLACK;
                w->left->color = BLACK;
                TurnRight(node->parent);
                node = root;
            }
        }
    }
    node->color = BLACK;
}

TRBTree::TRBTree() {
    root = NIL;
}

void TRBTree::Insert(TString key, uint64_t num, int* succes) {
    TNode* node = root;
    TNode* parent = NIL;
    int compareRes;
    while (node != NIL)
    {
        compareRes = CompareStrings(key, node->key);
        if (compareRes == 1) {
            parent = node;
            node = node->right;
        }
        else if (compareRes == 0) {
            parent = node;
            node = node->left;
        }
        else {
            *succes = 0;
            return;
        }
    }
    if (node == NIL) {
        TNode* tmp = (TNode*) malloc(sizeof(TNode));
        CreateTNode(tmp, num, key, RED);
        if (parent != NIL) {
            if (compareRes == 1) {
                parent->right = tmp;
                parent->right->parent = parent;
            }
            else {
                parent->left = tmp;      
                parent->left->parent = parent;      
            }
            if (tmp->parent->parent != NIL) {
                InsertFixup(tmp);
            }
        }
        else {
            root = tmp;
            root->parent= NIL;
            root->color = BLACK;
        }
    }
}

void TRBTree::TurnLeft(TNode* node) {
    if (node->right != NIL) {
        TNode* temp = node->right;
        node->right = temp->left;
        if (temp->left != NIL) {
            temp->left->parent = node;
        }
        temp->left = node;
        temp->parent = node->parent;
        if (node->parent != NIL) {
            if (node->parent->left == node) {
                node->parent->left = temp;
            }
            else {
                node->parent->right = temp;            
            }
        }
        else {
            root = temp;
        }
        node->parent = temp;
    }
}

void TRBTree::TurnRight(TNode* node) {
    if (node->left != NIL) {
        TNode* temp = node->left;
        node->left = temp->right;
        if (temp->right != NIL) {
            temp->right->parent = node;
        }
        temp->right = node;
        temp->parent = node->parent;
        if (node->parent != NIL) {
            if (node->parent->left == node) {
                node->parent->left = temp;
            }
            else {
                node->parent->right = temp;            
            }
        }
        else {
            root = temp;
        }
        node->parent = temp;
    }
}


void TRBTree::RemoveMinimum(TNode* node, TNode* parent_node) {
    TNode* spec = node->right;
    while (spec->left != NIL) {
        spec = spec->left;
    }
    TNode* x = spec->right;
    int orig_color = spec->color;
    if (spec->parent == node) {
        x->parent = spec;
    }
    else {
        Transplant(spec, spec->right);
        spec->right = node->right;
        spec->right->parent = spec;
    }
    Transplant(node, spec);
    spec->left = node->left;
    spec->left->parent = spec;
    spec->color = node->color;
    if (orig_color == BLACK) {
        RemoveFixup(x);
    }
}

void TRBTree::Transplant(TNode* a, TNode* b) {
    if (a->parent == NIL) {
        root = b;
    }
    else if (a == a->parent->left) {
        a->parent->left = b;
    }
    else {
        a->parent->right = b;
    }
    b->parent = a->parent;
}

void TRBTree::Remove(TString key) {
    TNode* node = root;
    TNode* parent = NIL;
    int compareRes;
    while (node != NIL) {
        compareRes = CompareStrings(key, node->key);
        if (compareRes == 1) {
            parent = node;
            node = node->right;
        }
        else if (compareRes == 0) {
            parent = node;
            node = node->left;
        }
        else {
            int check = 0;
            std::cout << "OK\n";
            int orig_color = node->color;
            TNode* x;
            if (node->left == NIL) {
                x = node->right;
                Transplant(node, node->right);
            }
            else if (node->right == NIL) {
                x = node->left;
                Transplant(node, node->left);
            }
            else {
                RemoveMinimum(node, parent);
                check = 1;
            }
            if (check == 0 && orig_color == BLACK) {
                RemoveFixup(x);
            }
            DeleteTNode(node);
            return;
        }
    }
    if (node == NIL) {
        std::cout << "NoSuchWord\n";
    }
}

TNode* TRBTree::SearchSpecial(TNode* node, TString key) {
    if (node == NIL) {
        return NIL;
    }
    int compareRes;
    TNode* tn = node;
    while (tn != NIL) {
        compareRes = CompareStrings(key, tn->key);
        if (compareRes == 1) {
            tn = tn->right;
        }
        else if (compareRes == 0) {
            tn = tn->left;           
        }
        else {
            return tn;
        }
    }
    return NIL;
}

void TRBTree::DeleteSpecial(TNode* node) {
    if (node->right != NIL) {
        DeleteSpecial(node->right);
    }
    if (node->left != NIL) {
        DeleteSpecial(node->left);
    }
    DeleteTNode(node);
}

void TRBTree::Delete() {
    if (root != NIL) {
        DeleteSpecial(root);
        root = NIL;
    }
}

void TRBTree::Search(TString key) {
    TNode* node = SearchSpecial(root, key);
    if (node != NIL) {
            std::cout << "OK: " << node->number << "\n";
    }
    else {
        std::cout << "NoSuchWord\n";
    }
}

void TRBTree::Save(FILE* file, TNode* node) {
    TField for_save;
    if (node != NIL) {
        for_save.num = node->number;
        for (int i = 0; i < node->key.size; i++) {
            for_save.str[i] = node->key.data[i];
        }
        for (int i = node->key.size; i < MAX_LENGTH_OF_KEY; i++) {
            for_save.str[i] = '\0';
        }
        fwrite(&for_save, sizeof(TField), 1, file);
        if (node->left != NIL) {
            Save(file, node->left);
        }
        if (node->right != NIL) {
            Save(file, node->right);
        }
    }
    else {
    }
}

void TRBTree::Load(FILE* file) {
    Delete();
    TField for_save;
    while (!feof(file)) {
        fread(&for_save, sizeof(TField), 1, file);
        TString key;
        CreateTString(&key);
        for (int i = 0; for_save.str[i] != '\0'; i++) {
            PushTString(&key, for_save.str[i]);
        }
        int succes = 1;
        Insert(key, for_save.num, &succes);
        if (succes == 0) {
            DeleteTString(&key);
        }
    }

}

int main() {
    std::cin.tie(0);
    TRBTree tree;
    char c;
    while ((c = getchar()) != EOF) {
        if (c == '+') {
            TString key;
            CreateTString(&key);
            char cc;
            uint64_t number;
            cc = getchar();
            while (1) {
                cc = getchar();
                if (cc != ' ' && cc != EOF) {
                    if (cc >= 'A' && cc <= 'Z') {
                        cc = cc + 32;
                    }
                    PushTString(&key, cc);
                }
                else {
                    int succes = 1;
                    std::cin >> number;
                    tree.Insert(key, number, &succes);
                    if (succes == 0) {
                        std::cout << "Exist\n";
                        DeleteTString(&key);
                    }
                    else {
                        std::cout << "OK\n";
                    }
                    cc = getchar();
                    break;
                }
            }
        }
        else if (c == '-') {
            TString key;
            CreateTString(&key);
            char cc;
            cc = getchar();
            while (1) {
                cc = getchar();
                if (cc != '\n' && cc != EOF) {
                    if (cc >= 'A' && cc <= 'Z') {
                        cc = cc + 32;
                    }
                    PushTString(&key, cc);
                }
                else {
                    tree.Remove(key);
                    break;
                }
            }
            DeleteTString(&key);
        }
        else if (c == '!') {
            char cc[4];
            std::cin >> cc;
            char t;
            TString path;
            CreateTString(&path);
            t = getchar();
            while (1) {
                t = getchar();
                if (t != '\n' && t != EOF) {
                    PushTString(&path, t);
                }
                else {
                    PushTString(&path, '\0');
                    path.size--;
                    FILE* file;
                    if (cc[0] == 'S') {
                        if ((file = fopen(path.data, "wb")) == NULL) {
                            std::cout << "ERROR: Couldn't create file\n";
                        }
                        else {
                            tree.Save(file, tree.root);
                            std::cout << "OK\n";
                            fclose(file);
                        }
                        break;
                    }
                    if (cc[0] == 'L') {
                        if ((file = fopen(path.data, "rb")) == NULL) {
                            std::cout << "ERROR: Couldn't read file\n";
                        }
                        else {
                            tree.Load(file);
                            std::cout << "OK\n";
                            fclose(file);
                        }
                        break;
                    }
                }
            }
            DeleteTString(&path);
        }
        else {
            TString key;
            CreateTString(&key);
            if (c >= 'A' && c <= 'Z') {
                c = c + 32;
            }
            PushTString(&key, c);
            char cc;
            while (1) {
                cc = getchar();
                if (cc != '\n' && cc != EOF) {
                    if (cc >= 'A' && cc <= 'Z') {
                        cc = cc + 32;
                    }
                    PushTString(&key, cc);
                }
                else {
                    tree.Search(key);
                    break;
                }
            }
            DeleteTString(&key);
        }
    }
    tree.Delete();
    return 0;
}
