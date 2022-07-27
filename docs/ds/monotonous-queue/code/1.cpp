#include <iostream>

using namespace	std;

const int N = 1000010;

int n, k, hh, tt;
int q[N], a[N];

int main() {
	scanf("%d%d", &n, &k);
	for (int i = 0; i < n; i++)
		scanf("%d", &a[i]);

	hh = 0, tt = -1;
	for (int i = 0; i < n; i++) {
		if (hh <= tt && i - k + 1 > q[hh]) hh ++;

		while (hh <= tt && a[i] <= a[q[tt]]) tt --;

		q[++tt] = i;
		if (i >= k - 1)
			printf("%d ", a[q[hh]]);
	}

	puts("");

	hh = 0, tt = -1;
	for (int i = 0; i < n; i++) {
		if (hh <= tt && i - k + 1 > q[hh]) hh ++;

		while (hh <= tt && a[i] >= a[q[tt]]) tt --;

		q[++tt] = i;
		if (i >= k - 1)
			printf("%d ", a[q[hh]]);
	}

	return 0;
}