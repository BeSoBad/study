#include <iostream>
#include <map>
#include <fstream>
#include <set>
#include <vector>
#include <string>
#include <algorithm>

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <cmath>

const int MIN_FREQ_FOR_CORRECT_WORD = 750;
 

struct trie_t: std::map< char, trie_t >
{
    size_t w;
    trie_t() : w(0) {}
}
glossary;

trie_t & add(trie_t & t, const std::string & s)
{
    t.w++;
    return !s.empty() ? add(t[ s[0] ], s.substr(1))
                      : t['$'];
}
 
struct rambler_t
{
    std::string         todo;
    std::string         done;
    size_t         cost;
    const trie_t * road;
 
    rambler_t(const std::string & t, const std::string & d, size_t c, const trie_t * r)
        :  todo(t), done(d), cost(c), road(r) {}
    rambler_t() {}
};
 
typedef std::vector<rambler_t> team_t;
 
double ramb_chances(const rambler_t & a)  { return log((a.road->w + 1) * (a.done.size() + 1)) / (1 << a.cost); }
 
bool ramb_chance_cmp (const rambler_t & a, const rambler_t & b) { return ramb_chances(a) > ramb_chances(b); }
bool ramb_done_cmp   (const rambler_t & a, const rambler_t & b) { return a.done == b.done ? a.cost < b.cost : a.done < b.done; }
bool ramb_uniq       (const rambler_t & a, const rambler_t & b) { return a.done == b.done; }
 
void step_forward(const rambler_t & R, team_t & team, team_t & leaders)
{
    char           next  =       R.todo[0];
    const std::string & todo = next ? R.todo.substr(1) : "";
 
    for(std::map<char, trie_t>::const_iterator it = R.road->begin(); it != R.road->end(); ++it) {
        const trie_t* road  = &it->second;
        char dest  =  it->first;
        if (next == dest)
            team.push_back(rambler_t(  todo, R.done + dest,  R.cost    ,   road));  // rawway
        else {
            team.push_back(rambler_t(  todo, R.done + dest,  R.cost + 1,   road));  // change
            team.push_back(rambler_t(R.todo, R.done + dest,  R.cost + 1,   road));  // insert
            if (!next && dest == '$') {
                leaders.push_back(R);
            }
        }
    }
    if (next)
        team.push_back(    rambler_t(  todo, R.done,         R.cost + 1, R.road));  // delete
}
 
void print( const trie_t & t, const std::string & prefix = "" )
{
    for( std::map<char, trie_t>::const_iterator
                               it = t.begin(); it != t.end(); ++it )
        if ( it->first == '$' )
            std::cout << prefix << std::endl;
        else
            print( it->second, prefix + it->first );
}
team_t spellcheck(const std::string & word, const size_t max_cost, const size_t team_size) {
    team_t walkers, leaders;
           walkers.push_back(rambler_t(word, "", 0, &glossary)); // initial
    while (!walkers.empty())
    {
        team_t next_g;
        for(size_t i = 0; i < std::min(walkers.size(), team_size); ++i)
            if (walkers[i].cost < max_cost)
                step_forward(walkers[i], next_g, leaders);
 
        walkers.swap(next_g);
        sort(walkers.begin(), walkers.end(), ramb_chance_cmp);
    }
 
    sort(leaders.begin(), leaders.end(), ramb_done_cmp);
    leaders.resize(distance(leaders.begin(), unique(leaders.begin(), leaders.end(), ramb_uniq)));
    sort(leaders.begin(), leaders.end(), ramb_chance_cmp);
    return leaders;
}


bool isVowel(char c) {
    return ((c == 'a') || (c == 'e') || (c == 'i')  || (c == 'o')  || (c == 'u'));
}

std::string DeleteSymbols(std::string& str, int l, int r) {
    std::string tmp;
    for (int i = 0; i < l; i++) {
        tmp += str[i];
    }
    for (int i = r + 1; i < str.length(); i++) {
        tmp += str[i];
    }
    return tmp;
}

class TSoundexBase {
public:
    std::map < std::string, int > table;
    std::map < std::string, std::map < std::string, int > > collocation;
    std::string stats_path, output_path, input_path;
    uint64_t word_count;
    void init(std::ifstream& f) {
        word_count = 0;
        std::string trash;
        for (int i = 0; i < 5; i++) {
            f >> trash;
        }
        std::string words;
        while (getline(f, words)) {
            std::string word, prev_word;
            int cnt = 0;
            for (int i = 0; i < words.length(); i++) {
                if (words[i] == '\t') {
                    if (cnt == 0) {
                        cnt = 1;
                        word = "";
                        prev_word = "";
                        continue;
                    }
                    else {
                        word_count++;
                        table[word]++;
                        if (prev_word != "") {
                            collocation[prev_word][word]++;
                        }
                        break;
                    }
                }
                else if (words[i] == ' ') {
                    word_count++;
                    table[word]++;
                    if (prev_word != "") {
                        collocation[prev_word][word]++;
                    }
                    prev_word = word;
                    word = "";
                }
                else {
                    word += words[i];
                }
            }
        }
        std::set <std::string> added;
        for (auto i: table) {
            if (i.second > MIN_FREQ_FOR_CORRECT_WORD && added.count(i.first) == 0) {
                added.insert(i.first);
                add(glossary, i.first);
            }
        }
        f.close();
    }
};

struct TPair {
    std::string str;
    int cnt;
    TPair(std::string s, int c) {
        str = s;
        cnt = c;
    }
};

struct TTwoInt {
    int i, j;
    TTwoInt(int ii, int jj) {
        i = ii;
        j = jj;
    }
    TTwoInt() {};
};

class TQuery {
public:
    std::vector < std::string > words;
    std::vector < std::vector < TPair > > words_list;
    uint64_t radix;
    TSoundexBase tsb;
    void GetStats() {
        words_list.resize(words.size());
        uint64_t max = 0;
        for (int i = 0; i < words.size(); i++) {
            team_t leaders = spellcheck(words[i], 3, 512);
            int min_cost = 10;
            for (int j = 0; j < leaders.size(); j++)
                if (leaders[j].cost < min_cost)
                    min_cost = leaders[j].cost;
            if (min_cost == 0) {
                for (int j = 0; j < leaders.size(); j++) {
                    if (leaders[j].cost == min_cost) {
                        TPair tp(leaders[j].done, tsb.table[leaders[j].done]);
                        words_list[i].push_back(tp);
                        break;
                    }
                }
            }
            else {
                for (int j = 0; j < leaders.size(); j++) {
                    if (min_cost == leaders[j].cost) {
                        TPair tp(leaders[j].done, tsb.table[leaders[j].done]);
                        words_list[i].push_back(tp);
                    }
                }
            }
            if (words_list[i].empty()) {
                TPair tp(words[i], 0);
                words_list[i].push_back(tp);
            }
        }
    }
    void NewQuery(std::string query) {
        words.clear();
        words_list.clear();
        std::string tmp;
        for (int i = 0; i < query.length(); i++) {
            if ((query[i] == ' ' || query[i] == '\t') && tmp != "") {
		        words.push_back(tmp);
		        tmp = "";
            }
            else if (query[i] != ' ' && query[i] != '\t')
            	tmp += query[i];
        }
        if (tmp != "") {
		    words.push_back(tmp);
        }
    }
    void Fix(std::ofstream& output) {
        std::vector < std::vector < double > > viterbi(words_list.size());
        std::vector < std::vector < TTwoInt > > parents(words_list.size());
        for (int i = 0; i < words_list[0].size(); i++) {
            viterbi[0].push_back(double(words_list[0][i].cnt) / tsb.word_count);
        }
        for (int i = 1; i < words_list.size(); i++) {
            for (int j = 0; j < words_list[i].size(); j++) {
                double max = 0;
                TTwoInt tti(i - 1, 0);
                for (int k = 0; k < words_list[i - 1].size(); k++) {
                    double u1 = viterbi[i - 1][k], u2 = tsb.collocation[words_list[i - 1][k].str][words_list[i][j].str] / double(tsb.word_count);
					if (u2 == 0) u2 = 1 / double(tsb.word_count);
                    if (u1 * u2 > max) {
                        max = u1 * u2;
                        tti.i = i - 1;
                        tti.j = k;
                    }
                }
                viterbi[i].push_back(double(words_list[i][j].cnt) / tsb.word_count * max);
                parents[i].push_back(tti);
            }
        }
        double max = 0;
        uint64_t max_id;
        for (int i = 0; i < viterbi[viterbi.size() - 1].size(); i++) {
            if (viterbi[viterbi.size() - 1][i] >= max) {
                max = viterbi[viterbi.size() - 1][i];
                max_id = i;
            }
        }
        TTwoInt tti;
        tti.i = viterbi.size() - 1;
        tti.j = max_id;
        std::vector < std::string > answer;
        answer.push_back(words_list[tti.i][tti.j].str);
        if (tti.i != 0) {
            int i, j;
            i = parents[tti.i][tti.j].i;
            j = parents[tti.i][tti.j].j;
            tti.i = i;
            tti.j = j;
            while (tti.i != 0) {
                answer.push_back(words_list[tti.i][tti.j].str);
                i = parents[tti.i][tti.j].i;
                j = parents[tti.i][tti.j].j;
                tti.i = i;
                tti.j = j;
            }
            answer.push_back(words_list[tti.i][tti.j].str);
        }
        for (int i = answer.size() - 1; i >= 0; i--) {
            output << answer[i];
            output << " ";
        }
        output << std::endl;
    }
};

int main(int argc, char* argv[]) {
	std::vector < std::string > params(argc);
	for (int i = 0; i < argc; i++) {
		std::string s(argv[i]);
		params[i] = s;
	}
	std::string op, input_path, output_path, stats_path;
	std::ifstream input, stats;
    std::ofstream output;
    op = params[1];
	for (int i = 2; i < argc; i++)
		if (params[i] == "--input")
			input_path = params[i + 1];
	for (int i = 2; i < argc; i++)
		if (params[i] == "--output")
            output_path = params[i + 1];
    input.open(input_path);
	if (op == "learn") {
        output.open(output_path, std::ios::app);
        std::string str;
		while (getline(input, str)) {
		    output << str << "\n";
		}
	}
	else if (op == "correct") {
		for (int i = 2; i < argc; i++)
			if (params[i] == "--stats")
                stats_path = params[i + 1];
        output.open(output_path);
		stats.open(stats_path);
		TQuery tq;
		tq.tsb.init(stats);
		std::string str;
		while (getline(input, str)) {
		    tq.NewQuery(str);
		    tq.GetStats();
		    tq.Fix(output);
		}
	}
	input.close();
	output.close();
	stats.close();
}
