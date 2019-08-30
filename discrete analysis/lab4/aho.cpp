#include <iostream>
#include <map>
#include <queue>
#include <stack>

struct TTrieNode {
    std::map < std::string, TTrieNode* > next;
    TTrieNode* parent;
    TTrieNode* link;
    std::string symbol;
    int number;
    int distance;
};

void CreateTTrieNode(TTrieNode* ttn, std::string symbol, TTrieNode* parent) {
    ttn->symbol = symbol;
    ttn->parent = parent;
    ttn->link = NULL;
    ttn->number = -1;
    ttn->distance = 0;
}

class TTrie {
public:
    TTrieNode* root;
    int size;
    TTrie();
    void Insert();
    void Search();
    void SetLink();
    ~TTrie();
};

TTrie::TTrie() {
    root = new TTrieNode;
    CreateTTrieNode(root, "#", NULL);
    size = 0;
}

TTrie::~TTrie() {
    std::stack < TTrieNode* > q;
    TTrieNode* temp;
    q.push(root);
    while (!q.empty()) {
        temp = q.top();
        q.pop();
        for (auto i: temp->next) {
            if (i.second != NULL)
                q.push(i.second);
        }
        delete temp;
    }
}

int BinarySearch(std::vector < int >& arr, int left, int right, int key)
{
	int midd = 0;
	while (1)
	{
		midd = (left + right) / 2;
		if (key < arr[midd])
			right = midd - 1;
		else if (key > arr[midd])
			left = midd + 1;
		else {
            for (int i = midd; i < arr.size() - 1; i++) {
                if (arr[i] == arr[i + 1]) {
                    midd++;
                }
                else {
                    break;
                }
            }
            return midd;
        }
 
        if (left > right) {
            for (int i = right; i < arr.size() - 1; i++) {
                if (arr[i] == arr[i + 1]) {
                    right++;
                }
                else {
                    break;
                }
            }
            return right;
        }
	}
}

const int NEW_LINE = 1;
const int EMPTY_LINE = 2;
const int END_OF_INPUT = 3;
char prev_delimiter = '\n';
std::string GetWord(int* status) {
    char c;
    std::string s = "";
    while ((c = getchar()) != EOF) {
        if (c == '\n') {
            *status = NEW_LINE;
            if (s.empty() && prev_delimiter == '\n') {
                *status = EMPTY_LINE;
            }
            prev_delimiter = '\n';
            break;
        }
        else if (c == ' ') {
            prev_delimiter = ' ';
            *status = 0;
            break;
        }
        else {
            s += tolower(c);
        }
    }
    if (c == EOF && s.empty()) {
        *status = END_OF_INPUT;
    }
    return s;
}

void TTrie::Search() {
    TTrieNode* node = root;
    int number_of_word = 1, number_of_string = 1, status = 0;
    std::vector < int > length_of_strings(1);
    length_of_strings[0] = 0;
    std::string s;
    s = GetWord(&status);
    while (status != END_OF_INPUT) {
        if (s.empty()) {
            if (status == NEW_LINE || status == EMPTY_LINE) {
                number_of_string++;
                length_of_strings.push_back(number_of_word - 1);
                status = 0;
            }
            s = GetWord(&status);
            continue;
        }
        if (node->next[s] != NULL) {
            std::queue < TTrieNode* > Ps;
            if (node->next[s]->number != -1) {
                Ps.push(node->next[s]);
            }
            TTrieNode* temp_node;
            temp_node = node->next[s];
            while (temp_node != root) {
                temp_node = temp_node->link;
                if (temp_node->number != -1) {
                    Ps.push(temp_node);
                }
            }
            while (!Ps.empty()) {
                temp_node = Ps.front();
                Ps.pop();
                int current_string = BinarySearch(length_of_strings, 0, length_of_strings.size() - 1, number_of_word - temp_node->distance);
                int output_number_of_word = number_of_word - temp_node->distance + 1 - length_of_strings[current_string];
                printf("%d, %d, %d\n", current_string + 1, output_number_of_word, temp_node->number + 1);
            }
            node = node->next[s];
            if (status == 1) {
                number_of_string++;
                length_of_strings.push_back(number_of_word);
                status = 0;
            }
            s = GetWord(&status);
            number_of_word++;
        }
        else if (node == root) {
            if (status == 1) {
                number_of_string++;
                length_of_strings.push_back(number_of_word);
                status = 0;
            }
            s = GetWord(&status);
            number_of_word++;
        }
        else {
            node = node->link;
        }
    }
}

void TTrie::SetLink() {
    std::queue < TTrieNode* > q;
    q.push(root);
    while (!q.empty()) {
        TTrieNode* ttn = q.front();
        q.pop();
        if (ttn == root || ttn->parent == root) {
            ttn->link = root;
        }
        else {
            TTrieNode* temp_link = ttn->parent->link;
            while (temp_link != root && temp_link->next[ttn->symbol] == NULL) {
                temp_link = temp_link->link;
            }
            if (temp_link->next[ttn->symbol] != NULL) {
                ttn->link = temp_link->next[ttn->symbol];
            }
            else {
                ttn->link = root;
            }
        }
        if (!ttn->next.empty()) {
            for (auto i: ttn->next) {
                q.push(i.second);
            }
        }
    }
}

void TTrie::Insert() {
    TTrieNode* present = root;
    int i = 0;
    while (1) {
        int status = 0;
        std::string s = GetWord(&status);
        if (status != EMPTY_LINE) {
            if (!s.empty()) {
                if (present->next[s] == NULL) {
                    present->next[s] = new TTrieNode;
                    CreateTTrieNode(present->next[s], s, present);
                }
                present = present->next[s];
                present->distance = i + 1;
                i++;
            }
            if (status == 1) {
                present->number = size++;
                present = root;
                i = 0;
            }
        }
        else {
            break;
        }
    }
}

int main() {
    TTrie tt;
    tt.Insert();
    tt.SetLink();
    tt.Search();
}
