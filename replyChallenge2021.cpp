#include <cassert>
#include <iostream>
#include <algorithm>
#include <cstdio> 
#include <complex>
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include <queue>
#include <string>
#include <cstdlib>
#include <memory.h>
#include <ctime>
#include <bitset>
#include <fstream>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <time.h>
#include <chrono>
#include <random>
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
#define VPLL vector<pair<long long, long long>>
#define VVPLL vector<vector<pair<long long, long long>>>
#define VD vector<double>
#define VVD vector<vector<double> >
#define VPD vector<pair<double,double> >
#define MP make_pair
#define PB push_back
#define FI first
#define SE second
#define SORT(v) std::sort(v.begin(),v.end())
#define GSORT(v,T) sort(v.begin(),v.end(),greater<T>)
#define UNIQUE(v) decltype(v.begin()) it = unique(v.begin(),v.end()); v.resize(distance(v.begin(),it))
using namespace std;
LL inp, Inp; LL i1, i2, i3, i4, i5, i6, i7, i8;
LL MOD = 998244353, MOD1 = (LL)1e9 + 7, MOD2 = (LL)1e9 + 9; long long MOD3 = (long long)1e9 + 21 /*33*/;  double eps = 1e-9;
LL TMP = (chrono::high_resolution_clock::now().time_since_epoch().count() % MOD + MOD) % MOD;
auto STTMP = chrono::steady_clock::now();//auto ENTMP = chrono::steady_clock::now();
//ELTIME = double(chrono::duration_cast<chrono::milliseconds>(ENTMP - STTMP).count());
//time_t result = time(NULL); char SS[26]; ctime_s(SS, sizeof SS, &result);
//transform(SS, SS + 26, SS, [](char ch) {return ch == ' ' ? '_' : ch; }); printf("%s", SS);;
#ifdef _DEBUG
bool debug = true;
#else
bool debug = false;
#endif // DEBUG

//////////// Parameters ////////////
vector<string> names = { "f" };//{ "a", "b", "c", "d", "e", "f" };
string sols = "f_20407133911.txt";
double totalMilliseconds = 240.0 * 1000;
const double startTemp = 5000;
//////////// Parameters ////////////

struct building {
    LL X, Y, latency, speed;
    multiset<LL> scores;
};

struct antenna {
    LL range, speed;
    LL X, Y;
    set<LL> connects;
};

struct gridInfo {
    LL aId;
    LL bId;
};

//////////// Globals /////////////
vector<vector<gridInfo>> grid;
vector<building> builds;
vector<antenna> ants;
LL W, H;
/////////////////////////////////

mt19937_64 mt_rand(TMP);
LL randTo(LL max) { // exclusive
    return mt_rand() % max;
}

LL calcScore(LL aId, LL bId) {
    LL dist = abs(builds.at(bId).X - ants.at(aId).X) + abs(builds.at(bId).Y - ants.at(aId).Y);
    return builds.at(bId).speed * ants.at(aId).speed - builds.at(bId).latency * dist;
}

tuple<LL, LL, LL, LL> moveAntenna(LL aId, LL tX, LL tY) {
    antenna& ant = ants.at(aId);
    auto& [aRange, aSpeed, aX, aY, aConnects] = ant;
    LL oldFromS = 0;
    LL newFromS = 0;
    for (int bX = max(0ll, aX - aRange); bX <= min(W - 1, aX + aRange); bX++) {
        for (int bY = max(0ll, aY - aRange + abs(bX - aX)); bY <= min(H - 1, aY + aRange - abs(bX - aX)); bY++) {
            if (grid.at(bX).at(bY).bId != -1) {
                LL bId = grid.at(bX).at(bY).bId;
                auto& build = builds.at(bId);
                oldFromS += *build.scores.rbegin();
                builds.at(bId).scores.erase(builds.at(bId).scores.find(calcScore(aId, bId)));
                if (build.scores.size()) {
                    newFromS += *build.scores.rbegin();
                }
            }
        }
    }
    grid.at(aX).at(aY).aId = -1;
    grid.at(tX).at(tY).aId = aId;
    aX = tX, aY = tY;
    LL oldToS = 0;
    LL newToS = 0;
    for (int bX = max(0ll, tX - aRange); bX <= min(W - 1, tX + aRange); bX++) {
        for (int bY = max(0ll, tY - aRange + abs(bX - tX)); bY <= min(H - 1, tY + aRange - abs(bX - tX)); bY++) {
            if (grid.at(bX).at(bY).bId != -1) {
                LL bId = grid.at(bX).at(bY).bId;
                auto& build = builds.at(bId);
                if (build.scores.size()) {
                    oldToS += *build.scores.rbegin();
                }
                builds.at(bId).scores.insert(calcScore(aId, bId));
                newFromS += *build.scores.rbegin();
            }
        }
    }
    return { oldFromS, newFromS, oldToS, newToS };
}

int main() {
    ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.precision(6);
    srand((unsigned int)TMP);
    for (string casename : names) {
        ifstream fin(casename + ".in");
        fin >> W >> H;
        grid.assign(W, vector<gridInfo>(H, { -1, -1 }));
        builds.clear();
        ants.clear();
        LL N, M, R; fin >> N >> M >> R;
        for (int n = 0; n < N; n++) {
            LL X, Y, lat, speed; fin >> X >> Y >> lat >> speed;
            builds.push_back({X, Y, lat, speed});
            grid.at(X).at(Y).bId = n;
        }
        double mxRange = 0;
        for (int m = 0; m < M; m++) {
            LL range, speed; fin >> range >> speed;
            ants.push_back({ range, speed });
        }
        ifstream sin(sols);
        LL Mp;
        sin >> Mp;
        assert(Mp == M);
        for (int m = 0; m < Mp; m++) {
            LL idi, X, Y; sin >> idi >> X >> Y;
            ants.at(idi).X = X;
            ants.at(idi).Y = Y;
            grid.at(X).at(Y).aId = m;
        }

        for (int aId = 0; aId < M; aId++){
            antenna& ant = ants.at(aId);
            auto& [aRange, aSpeed, aX, aY, aConnects] = ant;
            for (int bX = max(0ll, aX - aRange); bX <= min(W-1, aX + aRange); bX++) {
                for (int bY = max(0ll, aY - aRange + abs(bX - aX)); bY <= min(H-1, aY + aRange - abs(bX - aX)); bY++) {
                    if (grid.at(bX).at(bY).bId != -1) {
                        LL bId = grid.at(bX).at(bY).bId;
                        builds.at(bId).scores.insert(calcScore(aId, bId));
                    }
                }
            }
        }

        LL totScore = 0;
        for (int bId = 0; bId < N; bId++) {
            auto& build = builds.at(bId);
            if (build.scores.size()) {
                totScore += *build.scores.rbegin();
            }
        }
        cout << "Score before: " << totScore << "\n";

        double timePassed = 0;
        do {
            auto ENTMP = chrono::steady_clock::now();
            timePassed = double(chrono::duration_cast<chrono::milliseconds>(ENTMP - STTMP).count());
            LL aId = randTo(M);
            antenna oldAntenna = ants.at(aId);
            LL tX = -1, tY = -1;
            while (tX == -1 || grid.at(tX).at(tY).aId != -1) {
                tX = randTo(W);
                tY = randTo(H);
            }
            // TODO: undo change, update grid
            auto [oldFromS, newFromS, oldToS, newToS] = moveAntenna(aId, tX, tY);
            LL change = (newFromS - oldFromS) + (newToS - oldToS);
            double pseudo_random = randTo(1e9) / 1e9;
            double temp = startTemp * (totalMilliseconds - timePassed) / totalMilliseconds;
            if (change >= 0 || exp(change / temp) > pseudo_random) {
                totScore += change;
                cout << "New tot score: " << totScore << " time passed: " << timePassed/1000 << "\n";
            }
            else {
                moveAntenna(aId, oldAntenna.X, oldAntenna.Y);
            }
        } while (timePassed < totalMilliseconds);

        cout << "Final score: " << totScore << "\n";

        ofstream fout(casename + "_" + to_string(totScore) + ".txt");
        fout << M << "\n";
        for (int aId = 0; aId < M; aId++) {
            antenna& ant = ants.at(aId);
            fout << aId << " " << ant.X << " " << ant.Y << "\n";
        }

    }
    return 0;
}
