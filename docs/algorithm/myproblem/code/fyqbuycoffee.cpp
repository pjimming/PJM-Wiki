#include <bits/stdc++.h>
using namespace std;

const int N = 10010;

int n, m, q, s;
int w[N], v[N], p[N];
int dp[N];

int find(int x) {
	if (p[x] != x) p[x] = find(p[x]);
	return p[x];
}

int main() {
	cin >> n >> m >> q >> s;
	for (int i = 1; i <= n; i++) {
		cin >> v[i] >> w[i];
		p[i] = i;
	}

	while (m--) {
		int a, b;
		cin >> a >> b;
		p[find(a)] = find(b);
	}

	vector<int> bags;
	for (int i = 1; i <= n; i++) {
		if (p[i] != i) {
			w[find(i)] += w[i];
			v[find(i)] += v[i];
		} else bags.push_back(i);
	}

	for (auto &i : bags)
		for (int j = v[i]; j <= q; j++)
			dp[j] = max(dp[j], dp[j - v[i]] + w[i]);

	cout << dp[q] << endl;
	if (dp[q] >= s) puts("I'm going to dance");
	else puts("I'm so tired");

	return 0;
}