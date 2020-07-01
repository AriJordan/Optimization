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
int N, M;
vector<vector<unsigned int>> G;
int last_val = -1;
LL n_changes = 0;
double change_sum = 0; //average: 1 e-5 = 0.00001

struct info { // sol and in_group need to be initialized
	vector<unordered_set<unsigned int>> sol; // unordered_set: (101779849)
	double val;
	double Modularity_C;
	double first_Reg_term;
	double second_Reg_term;
	vector<double> Modularities_c;
	vector<double> Regularizations_c;
	vector<double> densities_c;
	vector<double> E_ins;
	vector<double> deg_sums;
	vector<int> in_group;
	int total_n_comm;
	int nonempty_n_comm;
};

void compute_solution_val(info& inform) { // modifies a lot of inputs
	inform.total_n_comm = inform.sol.size();
	inform.nonempty_n_comm = 0;
	GT(group, inform.sol) {
		if (group.size() != 0) {
			inform.nonempty_n_comm++;
		}
	}
	inform.Modularity_C = 0;
	inform.second_Reg_term = -0.5 * (double)inform.nonempty_n_comm / (double)N;
	inform.first_Reg_term = 0;
	inform.Modularities_c.clear(); inform.Modularities_c.resize(inform.total_n_comm);
	inform.Regularizations_c.clear(); inform.Regularizations_c.resize(inform.total_n_comm);
	inform.densities_c.clear(); inform.densities_c.resize(inform.total_n_comm);
	inform.E_ins.clear(); inform.E_ins.resize(inform.total_n_comm);
	inform.deg_sums.clear(); inform.deg_sums.resize(inform.total_n_comm);
	FU(n, 0, inform.total_n_comm) {
		if (inform.sol.at(n).size() != 0) { // Ignore empty groups
			inform.E_ins.at(n) = 0;
			inform.deg_sums.at(n) = 0;
			GT(vert, inform.sol.at(n)) {
				GT(nei, G.at(vert)) {
					inform.deg_sums.at(n)++;
					if (inform.in_group.at(vert) == inform.in_group.at(nei)) {
						inform.E_ins.at(n) += 0.5;
					}
				}
			}
			inform.Modularities_c.at(n) = inform.E_ins.at(n) / (double)M - pow(inform.deg_sums.at(n) / (2. * (double)M), 2.);
			inform.densities_c.at(n) = 0;
			int c_size = inform.sol.at(n).size();
			if (c_size == 1) {
				inform.densities_c.at(n) = 1;
			}
			else {
				inform.densities_c.at(n) = inform.E_ins.at(n) / (0.5 * c_size * (c_size - 1.0));
			}
			inform.Regularizations_c.at(n) = 0.5 * (1. / (double)inform.nonempty_n_comm * inform.densities_c.at(n));
			inform.Modularity_C += inform.Modularities_c.at(n);
			inform.densities_c.at(n) = inform.densities_c.at(n);
			inform.first_Reg_term += inform.Regularizations_c.at(n);
		}
	}
	inform.val = inform.Modularity_C + inform.first_Reg_term + inform.second_Reg_term; // Not adjusted value
}

double compute_change(int vert, int old_group_id, int new_group_id, info& inform, bool apply) {
	double old_Modul_old_g = inform.Modularities_c.at(old_group_id);
	double old_Reg_old_g = inform.Regularizations_c.at(old_group_id);
	double old_Modul_new_g = inform.Modularities_c.at(new_group_id);
	double old_Reg_new_g = inform.Regularizations_c.at(new_group_id);
	double old_second_Reg_term = inform.second_Reg_term;

	// Modularities
	double E_in_old_g_change = 0; // positive
	GT(nei, G.at(vert)) { // TODO: precompute this
		if (inform.sol.at(old_group_id).count(nei)) {
			E_in_old_g_change++;
		}
	}
	double new_E_in_old_g = inform.E_ins.at(old_group_id) - E_in_old_g_change;
	double E_in_new_g_change = 0;
	GT(nei, G.at(vert)) {
		if (inform.sol.at(new_group_id).count(nei)) {
			E_in_new_g_change++;
		}
	}
	double new_E_in_new_g = inform.E_ins.at(new_group_id) + E_in_new_g_change;
	double deg_change = G.at(vert).size(); // positive
	double new_Modul_old_g = (new_E_in_old_g) / (double)M - pow(((double)inform.deg_sums.at(old_group_id) - (double)deg_change) / (2. * (double)M), 2.);
	double Modul_old_g_change = new_Modul_old_g - old_Modul_old_g;
	double new_Modul_new_g = (new_E_in_new_g) / (double)M - pow(((double)inform.deg_sums.at(new_group_id) + (double)deg_change) / (2. * (double)M), 2.);
	double Modul_new_g_change = new_Modul_new_g - old_Modul_new_g;

	//Regularities
	int new_nonempty_n_comm = inform.nonempty_n_comm;
	int new_c_old_g_size = inform.sol.at(old_group_id).size() - 1;
	double new_density_old_g = 0; // basically 0 when group empty
	if (new_c_old_g_size == 0) {
		new_nonempty_n_comm--;
	}
	else if (new_c_old_g_size == 1) {
		new_density_old_g = 1;
	}
	else {
		new_density_old_g = new_E_in_old_g / (0.5 * (double)new_c_old_g_size * ((double)new_c_old_g_size - 1.0));
	}
	double new_Reg_old_g = 0.5 / (double)inform.nonempty_n_comm * new_density_old_g; // Still using old nonempty_n_comm

	int new_c_new_g_size = inform.sol.at(new_group_id).size() + 1;
	double new_density_new_g;
	if (new_c_new_g_size == 1) {
		new_nonempty_n_comm++;
		new_density_new_g = 1;
	}
	else {
		new_density_new_g = new_E_in_new_g / (0.5 * (double)new_c_new_g_size * ((double)new_c_new_g_size - 1.0));
	}
	double new_Reg_new_g = 0.5 / (double)inform.nonempty_n_comm * new_density_new_g; // Still using old nonempty_n_comm

	// Adapt for new nonempty_n_comm
	double new_first_Reg_term = inform.first_Reg_term + (new_Reg_old_g - old_Reg_old_g) + (new_Reg_new_g - old_Reg_new_g);
	new_first_Reg_term *= ((double)inform.nonempty_n_comm / (double)new_nonempty_n_comm); // divide by new because 1/n in formula
	double new_second_Reg_term = inform.second_Reg_term;
	new_second_Reg_term *= ((double)new_nonempty_n_comm / (double)inform.nonempty_n_comm);
	double change =  Modul_old_g_change + Modul_new_g_change + (new_first_Reg_term - inform.first_Reg_term) + (new_second_Reg_term - old_second_Reg_term);
	if (apply) {
		inform.sol.at(old_group_id).erase(vert);
		inform.sol.at(new_group_id).insert(vert);
		inform.val += change;
		inform.Modularity_C += Modul_old_g_change + Modul_new_g_change;
		inform.first_Reg_term = new_first_Reg_term;
		inform.second_Reg_term = new_second_Reg_term;
		inform.Modularities_c.at(old_group_id) += Modul_old_g_change;
		inform.Modularities_c.at(new_group_id) += Modul_new_g_change;
		inform.Regularizations_c.at(old_group_id) = new_Reg_old_g;
		inform.Regularizations_c.at(new_group_id) = new_Reg_new_g;
		for(auto& reg : inform.Regularizations_c) {
			reg *= ((double)inform.nonempty_n_comm / (double)new_nonempty_n_comm);
		}
		inform.densities_c.at(old_group_id) = new_density_old_g;
		inform.densities_c.at(new_group_id) = new_density_new_g;
		inform.E_ins.at(old_group_id) = new_E_in_old_g;
		inform.E_ins.at(new_group_id) = new_E_in_new_g;
		inform.deg_sums.at(old_group_id) -= deg_change;
		inform.deg_sums.at(new_group_id) += deg_change;
		inform.in_group.at(vert) = new_group_id;
		inform.nonempty_n_comm = new_nonempty_n_comm;
		return -100000000000;
	}
	else {
		return change;
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	std::cout.precision(20);
	srand((unsigned int)TMP);

	///////////////////Modify////////////////////////
	string problem = "a1";
	string prev_val_input = to_string(213490); // from 162674 to: random: 164185, many_nei: 167036
	string prefix = problem + "_output_";
	bool reuse_old_solution = 1;
	/////////////////////////////////////////////////

	/////////////////Parameters//////////////////////
	// TODO: Prioritize groups where neighbours of element are or how big group is (+1)
	int MAX_GROUPS = 20; // try 900 group for a1, + 50 for a2
	int GROUP_INCREASE = 50;
	double STARTING_TEMP = 0.0000015; // average change = 0.000001, 0.000001 for a1, 0.000001 for a2, a3
	double milliseconds = 1000. * 60. * 60..;// (rand() % 10 + 1.);, 60 min for a1
	LL NEW_GROUP_RATE = 300000; // was 10 for a1, 1000 for a2, 100 for a3
	/////////////////////////////////////////////////

	FU(i, 0, 1) {
		STTMP = chrono::steady_clock::now();
		cin.clear();
		cerr.clear();
		G.clear();
		N = 0, M = 0;
		n_changes = 0, change_sum = 0;
		std::freopen((problem + ".in").c_str(), "r", stdin);
		unsigned int v1, v2;
		while (cin >> v1) {
			cin >> v2;
			M++;
			if (max(v1, v2) + 1 > G.size()) {
				G.resize(max(v1, v2) + 1);
			}
			G.at(v1).push_back(v2);
			G.at(v2).push_back(v1);
		}
		N = G.size();
		cerr << "read input, N: " << N << " M: " << M << "\n";
		info best; best.in_group.resize(N);
		best.sol.resize(N);

		int read_N = 0;
		if (reuse_old_solution) {
			cin.clear();
			if (last_val != -1) {
				prev_val_input = to_string(last_val);
			}
			std::freopen((prefix + prev_val_input + ".txt").c_str(), "r", stdin);
			string line;
			int group_id = 0;
			while (std::getline(cin, line)) {
				if(best.sol.at(group_id).size() != 0)
					group_id++;
				istringstream str_stream(line);
				unsigned int vertex;
				while (str_stream >> vertex) {
					read_N++;
					best.sol.at(group_id).insert(vertex);
					best.in_group.at(vertex) = group_id;
				}
			}
			if (read_N == N) {
				cerr << "read file successfully\n";
				cerr << "amount of nonempty groups: " << group_id + 1 << "\n";
				MAX_GROUPS = max(MAX_GROUPS, group_id + 1 + GROUP_INCREASE);
				best.sol.resize(MAX_GROUPS);
			}
			else {
				cerr << "reading file failed!\n";
			}
		}
		else {
			FU(n, 0, N) {
				int group = rand() % MAX_GROUPS;
				best.sol.at(group).insert(n); // All vertices in random group
				best.in_group.at(n) = group;
			}
		}

		// Compute best solution val
		compute_solution_val(best);

		if (reuse_old_solution) {
			if (abs(best.val - stoi(prev_val_input)) > 100000000) {
				cerr << "Computed value != previous value of input!\n";
				cerr << "Computed value: " << best.val << " previous val: " << prev_val_input << "\n";
				cerr << "Computed Modularity(C): " << best.Modularity_C << " Computed Regularization(C): " << best.first_Reg_term + best.second_Reg_term << "\n";
				return 1;
			}
		}

		info current = best;
		double cur_temp = STARTING_TEMP;

		do {
			auto ENTMP = chrono::steady_clock::now();
			double ELTIME = double(chrono::duration_cast<chrono::milliseconds>(ENTMP - STTMP).count());
			cur_temp = STARTING_TEMP * (milliseconds - ELTIME) / milliseconds;
			int vert;
			int new_group_id;
			do {
				vert = (rand() + (RAND_MAX + 1) * rand()) % N;
				new_group_id = (current.in_group.at(G.at(vert).at((rand() + (RAND_MAX + 1) * rand()) % G.at(vert).size())));
				//if(rand() % 10 == 0)
					//new_group_id = (rand() + (RAND_MAX + 1) * rand()) % MAX_GROUPS;
			} while (current.in_group.at(vert) == new_group_id);
			int old_group_id = current.in_group.at(vert);
			// sometimes put in empty group
			if (rand() % NEW_GROUP_RATE == 0 && current.nonempty_n_comm < current.total_n_comm) {
				FD(n, current.total_n_comm - 1, -1) {
					if (current.sol.at(n).size() == 0) {
						new_group_id = n;
						break;
					}
				}
			}

			// Calculate change
			double change = compute_change(vert, old_group_id, new_group_id, current, false);
			n_changes++;
			change_sum += abs(change);

			double pseudo_random = ((double)rand() + (double)(RAND_MAX + 1) * rand()) / ((double)RAND_MAX * RAND_MAX);
			if (exp(change / cur_temp) > pseudo_random) { // change current stuff
				compute_change(vert, old_group_id, new_group_id, current, true);
				// cerr << "new current val: " << current.val <<"\n";
			}
			if (current.val > best.val && ((cur_temp < 0.02 * STARTING_TEMP && rand() % 1000)  || rand() % 10000000 == 0)) {
				cerr << "new best val: " << current.val << "\n";
				best = current;
			}
		} while (cur_temp > 0);
		cerr << "Amount of changes: " << n_changes << "\n";
		cerr << "Best val: " << best.val << "\n";
		cerr << "Best Modularity(C) : " << best.Modularity_C << " ,best Regularization(C) : " << best.first_Reg_term + best.second_Reg_term << "\n";
		info actual; actual.sol = best.sol; actual.in_group = best.in_group;
		compute_solution_val(actual);
		cerr << "Actual val: " << actual.val << "\n";
		cerr << "Actual Modularity(C) : " << actual.Modularity_C << " ,actual Regularization(C) : " << actual.first_Reg_term + actual.second_Reg_term << "\n";

		double adjusted_actual_val = (actual.val + 1.0) * 100000.;
		cerr << "Adjusted actual val: " << adjusted_actual_val << "\n";
		cerr << "Remaining nonempty groups: " << actual.nonempty_n_comm << "\n";
		cerr << "Average change: " << change_sum / (double)n_changes << "\n";

		//TODO: compare actual to best


		cout.clear();
		std::freopen("log.txt", "a", stdout);
		if (int(adjusted_actual_val) > last_val) {
			cout << problem << " ";
			cout << "Average change: " << change_sum / (double)n_changes << " groups: " << actual.nonempty_n_comm << "\n";
			cout << "from " << last_val << " to " << int(adjusted_actual_val) << " improve: " << int(adjusted_actual_val) - last_val << "\n";
			cout << "start temp / 0.000000001: " << STARTING_TEMP / 0.000000001 << " " << "time: " << milliseconds / 1000. / 60. << "\n";
		}

		last_val = int(adjusted_actual_val);
		cout.clear();
		std::freopen((prefix + to_string(int(adjusted_actual_val)) + ".txt").c_str(), "w", stdout);
		GT(group, actual.sol) {
			if (group.size()) {
				GT(v, group) {
					cout << v << " ";
				}
				cout << "\n";
			}
		}
	}

	return 0;
}
