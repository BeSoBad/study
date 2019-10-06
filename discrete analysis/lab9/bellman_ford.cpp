#include <iostream>
#include <vector>
using namespace std;

struct TEdge {
	int from, to, weight;
};

const long long INF = 10e17;

int main() {
	int n, m, start, finish;
	cin >> n >> m >> start >> finish;
	start--;
	finish--;
	vector < long long > d(n, INF);
	vector < TEdge > e;
	for (int i = 0; i < m; i++) {
		struct TEdge ed;
		cin >> ed.from >> ed.to >> ed.weight;
		ed.from--;
		ed.to--;
		e.push_back(ed);
	}
	d[start] = 0;
	for (int i = 0; i < n - 1; i++) {
		bool any = false;
		for (int j = 0; j < m; j++) {
			if (d[e[j].from] < INF) {
				if (d[e[j].to] > d[e[j].from] + e[j].weight) {
					d[e[j].to] = d[e[j].from] + e[j].weight;
					any = true;
				}
			}
		}
		if (!any) {
			break;
		}
	}
	if (d[finish] == INF) {
		cout << "No solution" << endl;
	}
	else {
		cout << d[finish] << endl;
	}
}
