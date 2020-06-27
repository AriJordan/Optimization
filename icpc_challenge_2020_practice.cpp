#include <cassert>
#include<iostream>
#include<algorithm>
#include<cstdio> 
#include<complex>
#include<vector>
#include<set>
#include<map>
#include<cmath>
#include<queue>
#include<string>
#include<cstdlib>
#include<memory.h>
#include<ctime>
#include<bitset>
#include<fstream>
#include<queue>
#include<stack>
#include<unordered_map>
#include<unordered_set>
#include<functional>
#include <time.h>
#include<chrono>
#include <ctime>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#define UI unsigned int
#define LL long long
#define ll long long
#define GT(it,v) for(auto it:v)
#define FU(i,a,b) for(int i=(a);i<(b);i++)
#define FD(i,a,b) for(int i=(a);i>(b);i--)
#define LD long double
#define PI 3.1415926535897932384626
#define PII pair<int,int>
#define PLL pair<LL,LL>
#define VLL vector<long long >
#define VVLL vector<vector<long long > >
#define VVVLL vector<vector<long long > >
#define VPLL vector<pair<long long, long long>>
#define VVPLL vector<vector<pair<long long, long long>>>
#define VD vector<double>
#define VVD vector<vector<double> >
#define VPD vector<pair<double,double> >
#define MP make_pair
#define PB push_back
#define FI first
#define SE second
#define SORT(v) sort(v.begin(),v.end())
#define GSORT(v,what) sort(v.begin(),v.end(),greater<what>)
#define UNIQUE(it,v) (it = unique(v.begin(),v.end()), v.resize(distance(v.begin(),it)))
using namespace std; LL inp, Inp; LL i1, i2, i3, i4, i5, i6, i7, i8;
LL MOD = 998244353, MOD1 = (LL)1e9 + 7, MOD2 = (LL)1e9 + 9; long long MOD3 = (long long)1e9 + 21 /*33*/;  double eps = 1e-9;
LL TMP = (chrono::high_resolution_clock::now().time_since_epoch().count() % MOD + MOD) % MOD;
auto STTMP = chrono::steady_clock::now();//auto ENTMP = chrono::steady_clock::now();
//ELTIME = double(chrono::duration_cast<chrono::milliseconds>(ENTMP - STTMP).count());
//time_t result = time(NULL); char SS[26]; ctime_s(SS, sizeof SS, &result);
//transform(SS, SS + 26, SS, [](char ch) {return ch == ' ' ? '_' : ch; }); printf("%s", SS);

/////////// Classic Simualted Annealing ////////////

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	std::cout.precision(20);
	srand((unsigned int)TMP);
	std::freopen("mis/b4.in", "r", stdin);
	int N, M; cin >> N >> M;
	vector<vector<int>> G(N);
	vector<bool> best_sol(N);
	int best_val = 0;
	FU(i, 0, M) {
		cin >> i1 >> i2; i1--, i2--;
		G.at(i1).push_back(i2);
		G.at(i2).push_back(i1);
	}

	///////////////////Modify////////////////////////
	LL prev_val_input = 0;
	string prefix = "prac_b4_output_";
	double starting_temp = 0.8;
	bool reuse_old_solution = 0;
	////////////////////////////////////////////////

	if (reuse_old_solution) {
		std::freopen((prefix + to_string(prev_val_input) + ".txt").c_str(), "r", stdin);
		LL actual_prev_val; cin >> actual_prev_val;
		if (prev_val_input != actual_prev_val) {
			cout << "Error: Values don't match";
			return 1;
		}
		FU(n, 0, N) {
			cin >> i1; best_sol.at(n) = i1;
		}
	}

	vector<bool> current_sol = best_sol;
	int current_val = prev_val_input;
	best_val = prev_val_input;

	double temp = starting_temp;
	double milliseconds = 200000;
	do {
		auto ENTMP = chrono::steady_clock::now();
		double ELTIME = double(chrono::duration_cast<chrono::milliseconds>(ENTMP - STTMP).count());
		temp = starting_temp * (milliseconds - ELTIME) / milliseconds;
		int pos;
		do {
			pos = (rand() + (RAND_MAX + 1) * rand()) % N;
		} while (current_sol.at(pos) == 1 || 2. / (1. + (double)G.at(pos).size()) < double(rand()) / RAND_MAX);
		double change = 1;
		GT(nei, G.at(pos)) {
			if (current_sol.at(nei)) {
				change--;
			}
		}
		double pseudo_random = ((double)rand() + (double)(RAND_MAX + 1) * rand()) / ((double)RAND_MAX * RAND_MAX);
		if (change >= 0 || exp(change / temp) > pseudo_random) {
			current_val += change;
			current_sol.at(pos) = 1;
			GT(nei, G.at(pos)) {
				current_sol.at(nei) = 0;
			}
		}
		if (current_val > best_val && temp < 0.8) {
			cout << "new best val: " << current_val << "\n";
			best_val = current_val;
			best_sol = current_sol;
		}
	} while (temp > 0);

	LL actual_res = 0;
	FU(n, 0, N) {
		if (best_sol.at(n)) {
			actual_res++;
		}
		GT(nei, G.at(n)) {
			if (best_sol.at(n) && best_sol.at(nei)) {
				cout << "Violation!\n";
			}
		}
	}
	cout << "actual res: " << actual_res << "\n";



	std::freopen((prefix + to_string(actual_res) + ".txt").c_str(), "w", stdout);
	cout << best_val << "\n";
	GT(it, best_sol) {	
		cout << it << " ";
	}

	return 0;
}
