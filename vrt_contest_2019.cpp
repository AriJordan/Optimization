// https://codeforces.com/contest/1160

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
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
 
 
 
#define GT(it,v) for(auto it:v)
#define FU(i,a,b) for(LL i=(a);i<(b);i++)
#define FD(i,a,b) for(LL i=(a);i>(b);i--)
#define LL long long
#define LD long double
#define PI 3.1415926535897932384626
#define PII pair<int,int>
#define PLL pair<LL,LL>
#define VLL vector<long long >
#define VVLL vector<vector<long long > >
#define VPLL vector<pair<long long, long long>>
#define VVPLL vector<vector<pair<long long, long long>>>
#define VD vector<double>
#define VPD vector<pair<double,double> >
#define MP make_pair
#define PB push_back
#define FI first
#define SE second
#define SORT(v) sort(v.begin(),v.end())
#define GSORT(v,what) sort(v.begin(),v.end(),greater<what>())
using namespace std; LL inp; LL i1, i2, i3, i4, i5, i6, i7, i8;
LL MOD = (LL)1e9 + 7, MOD2 = (LL)1e9 + 9; double eps = 1e-9;
LL TMP = (chrono::high_resolution_clock::now().time_since_epoch().count() % MOD + MOD) % MOD;
auto STTMP = chrono::steady_clock::now();//auto ENTMP = chrono::steady_clock::now();
//ELTIME = double(chrono::duration_cast<chrono::milliseconds>(ENTMP - STTMP).count());
 
bool read_from_file = 0;
 
struct employee {//TODO: update x,y
	LL x, y, finish_time, employee_index; LL wpx, wpy; VLL workstarts; VLL workends; VLL workids;
	employee(LL x, LL y, LL finish_time,LL employee_index) : x(x), y(y), finish_time(finish_time), employee_index(employee_index) {}
};
 
struct workpl {
	LL x, y, d, p, l, h, inpindex;
	bool done = 0;
	workpl(LL x, LL y, LL d, LL p, LL l, LL h, LL inpindex) :x(x), y(y), d(d), p(p), l(l), h(h), inpindex(inpindex) {}
};
 
bool wpsort(workpl a, workpl b) {
	if (a.h - a.d != b.h - b.d) {
		return a.h - a.d < b.h - b.d;
	}
	else {
		return a.inpindex < b.inpindex;
	}
}
 
bool emplsort(employee e1, employee e2) {
	if (e1.finish_time + abs(e1.x - e1.wpx) + abs(e1.y - e1.wpy) != e2.finish_time + abs(e2.x - e2.wpx) + abs(e2.y - e2.wpy)) {
		return e1.finish_time + abs(e1.x - e1.wpx) + abs(e1.y - e1.wpy) < e2.finish_time + abs(e2.x - e2.wpx) + abs(e2.y - e2.wpy);
	}
	else {
		return e1.employee_index < e2.employee_index;
	}
}
 
 
LL wpwpdist(workpl w1, workpl w2) {
	return abs(w1.x - w2.x) + abs(w1.y - w2.y);
}
 
LL emwpdist(employee e1, workpl w1) {
	return abs(e1.x - w1.x) + abs(e1.y - w1.y);
}
 
int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.precision(20);
	srand(TMP);
	LL employee_amount = 0;
	LL n;
	vector<workpl>wps,oriwps;
	if (read_from_file) {
		ifstream myfile;
		myfile.open("001.txt");
		if (!myfile.is_open()) {
			cout << "Error, input file not open";
			return 0;
		}
		myfile >> n;
		myfile >> i1 >> i2 >> i3 >> i4 >> i5 >> i6;
		workpl w(i1, i2, i3, i4, i5, i6, 0);
		wps.PB(w);
		FU(i, 1, n) {
			myfile >> i1 >> i2 >> i3 >> i4 >> i5 >> i6;
			workpl w(i1, i2, i3, i4, i5, i6, i);
			wps.PB(w);
			oriwps.PB(w);
		}
		myfile.close();
	}
	else {
		cin >> n;
		FU(i, 0, n) {
			cin >> i1 >> i2 >> i3 >> i4 >> i5 >> i6;
			workpl w(i1, i2, i3, i4, i5, i6, i);
			wps.PB(w);
			oriwps.PB(w);
		}
	}
	LL income = 0, recruitment = 0, upkeep = 0, totprof = 0;
	wps.at(0).done = 1;
	sort(wps.begin(), wps.end(), wpsort);	
	vector<employee>employees;
	FU(wpi, 1, n) {
		workpl nextwp = wps.at(wpi);
		vector<employee>employees_before_start;
		vector<employee>employees_after_start;
		FU(emi, 0, employees.size()) {
			employee nextemp = employees.at(emi);
			nextemp.wpx = nextwp.x, nextemp.wpy = nextwp.y;
			if (nextemp.finish_time + emwpdist(nextemp, nextwp) <= nextwp.l) {
				employees_before_start.PB(nextemp);
			}
			else if (nextemp.finish_time + emwpdist(nextemp, nextwp) <= nextwp.h - nextwp.d) {
				employees_after_start.PB(nextemp);
			}
		}
		if (employees_before_start.size() >= nextwp.p) {
			sort(employees_before_start.begin(), employees_before_start.end(), emplsort);
			FD(i, employees_before_start.size() - 1, employees_before_start.size() - nextwp.p - 1) {
				employees.at(employees_before_start.at(i).employee_index).workstarts.PB(nextwp.l);
				employees.at(employees_before_start.at(i).employee_index).workends.PB(nextwp.l+nextwp.d);
				employees.at(employees_before_start.at(i).employee_index).finish_time = nextwp.l + nextwp.d;
				employees.at(employees_before_start.at(i).employee_index).x = nextwp.x;
				employees.at(employees_before_start.at(i).employee_index).y = nextwp.y;
				employees.at(employees_before_start.at(i).employee_index).workids.PB(nextwp.inpindex);
			}
		}
		else {
			sort(employees_after_start.begin(), employees_after_start.end(), emplsort);
			LL aftn = min((LL)employees_after_start.size(), nextwp.p - employees_before_start.size());//amount of workers after starttime
 
			LL startt;
			if (aftn > 0) {
				employee lastemp = employees_after_start.at(aftn-1);
				startt = lastemp.finish_time + abs(lastemp.x - lastemp.wpx) + abs(lastemp.y - lastemp.wpy);
			}
			else {
				startt = nextwp.l;
			}
			FU(i, 0, aftn) {
				employees.at(employees_after_start.at(i).employee_index).workstarts.PB(startt);
				employees.at(employees_after_start.at(i).employee_index).workends.PB(startt + nextwp.d);
				employees.at(employees_after_start.at(i).employee_index).finish_time = startt + nextwp.d;
				employees.at(employees_after_start.at(i).employee_index).x=nextwp.x;
				employees.at(employees_after_start.at(i).employee_index).y=nextwp.y;
				employees.at(employees_after_start.at(i).employee_index).workids.PB(nextwp.inpindex);
			}
			FU(i, 0, employees_before_start.size()) {
				employees.at(employees_before_start.at(i).employee_index).workstarts.PB(startt);
				employees.at(employees_before_start.at(i).employee_index).workends.PB(startt + nextwp.d);
				employees.at(employees_before_start.at(i).employee_index).finish_time = startt + nextwp.d;
				employees.at(employees_before_start.at(i).employee_index).x = nextwp.x;
				employees.at(employees_before_start.at(i).employee_index).y = nextwp.y;
				employees.at(employees_before_start.at(i).employee_index).workids.PB(nextwp.inpindex);
			}
			FU(i, 0, nextwp.p - aftn - employees_before_start.size()) {
				employee tmpem(nextwp.x, nextwp.y, startt, employee_amount);
				employees.PB(tmpem);
				employees.at(employee_amount).workstarts.PB(startt);
				employees.at(employee_amount).workends.PB(startt + nextwp.d);
				employees.at(employee_amount).finish_time = startt + nextwp.d;
				employees.at(employee_amount).workids.PB(nextwp.inpindex);
				employee_amount++;
			}		
		}
	}
	FU(i, 0, employees.size()) {
		employee ee = employees.at(i);
		cout << "start " << ee.workstarts.at(0) - abs(wps.at(0).x - oriwps.at(ee.workids.at(0)).x) - abs(wps.at(0).y - oriwps.at(ee.workids.at(0)).y) << " 1\n";
		FU(j, 0, ee.workstarts.size()) {
			cout << "arrive " << ee.workstarts.at(j) << " " << ee.workids.at(j) + 1 << "\n";
			cout << "work " << ee.workstarts.at(j) << " "<< ee.workends.at(j) <<" " << ee.workids.at(j) + 1 << "\n";
		}
		cout << "arrive " << ee.workends.at(ee.workends.size()-1) + abs(wps.at(0).x - oriwps.at(ee.workids.at(ee.workends.size()-1)).x) + abs(wps.at(0).y - oriwps.at(ee.workids.at(ee.workends.size()-1)).y) << " " << 1 << "\n";
		cout << "end" << "\n";
 
	}
 
	return 0;
}
