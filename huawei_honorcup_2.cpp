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
#define UI unsigned int
#define LL long long
#define GT(it,v) for(auto it:v)
#define FU(i,a,b) for(LL i=(a);i<(b);i++)
#define FD(i,a,b) for(LL i=(a);i>(b);i--)
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
#define GSORT(v,what) sort(v.begin(),v.end(),greater<what>())
using namespace std; LL inp; LL i1, i2, i3, i4, i5, i6, i7, i8;
LL MOD = 998244353, MOD1 = (LL)1e9 + 7, MOD2 = (LL)1e9 + 9; long long MOD3 = (long long)1e9 + 21 /*33*/;  double eps = 1e-9;
LL TMP = (chrono::high_resolution_clock::now().time_since_epoch().count() % MOD + MOD) % MOD;
auto STTMP = chrono::steady_clock::now();//auto ENTMP = chrono::steady_clock::now();
//ELTIME = double(chrono::duration_cast<chrono::milliseconds>(ENTMP - STTMP).count());
VVLL nn = { {1,0}, {0,1}, {-1,0}, {0,-1} };
struct prec_score {//precomputed scores for neighbours
	VD first_here = { 10000., 10000., 10000., 10000. };// first_top, first_left, first_bot, first_right;
};
double calc_cor_val(double fi_in, double se_in, double fi_ed, double se_ed, LL st_var) {
	return (min(fi_ed, se_ed));
	if (st_var == 0) {
		return fi_ed + se_ed;
	}
	else if (st_var == 1) {
		return (fi_ed + se_ed) / (fi_in + se_in + 0.1);
	}
	else if (st_var == 2) {
		return (min(fi_ed, se_ed));
	}
	else if (st_var == 3) {
		return (min(fi_ed, se_ed)) / (fi_in + se_in + 0.1);
	}
	else if (st_var == 4) {
		return min(fi_ed, se_ed) - fi_in - se_in;
	}
}
double dist_penalty(double xdist, double ydist, double M, LL st_var) {
	//return max(xdist, ydist) / M;
	if (st_var == 0) {
		return max(xdist, ydist) / M *0.8;
	}
	else if (st_var == 1) {
		return max(xdist, ydist) / M * 1.6;
	}
	else if (st_var == 2) {
		return max(xdist, ydist) / M * 4;
	}
	else if (st_var == 3) {
		return sqrt(xdist * xdist + ydist * ydist) / M;
	}
	else if (st_var == 4) {
		return sqrt(xdist * xdist + ydist * ydist) / M * 2;
	}
	else if (st_var == 5) {
		return sqrt(xdist * xdist + ydist * ydist) / M * 4;
	}
}
int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.precision(20);
	srand((UI)(TMP));
	//change these//
	LL P = 16, M = 32, RUNS = 12, TOT_ST_VARS = 1;
	LL file_id_begin = 2963, file_id_end = 2986;//not +1, (end with 99),2700-2962 done, 3000-3299 done, 3300-3599 done
	LL try_st_var = 1;
	bool DEBUG = 0, CORNERS = 1, EDGES = 1;
	////
	LL N = 512;
	FU(outer_file_id, file_id_begin, file_id_end + 1) {
		stringstream ss_read; ss_read << outer_file_id << ".txt";
		fstream fs(ss_read.str());
		vector<VVLL> rgb_comp(3u, VVLL(N, VLL(N))); vector<VVD> adapted_comp(3u, VVD(N, VD(N)));
		VVLL pdf_v(3u, VLL(256u)); VVD cdf_v(3u, VD(256u));
		double EDGE_FIELDS = double(4 * M * N);
		//reads input:
		FU(color, 0, 3) {
			FU(nj, 0, N) {
				FU(ni, 0, N) {
					fs >> inp;
					if (inp >= pdf_v.at(0).size()) {
						FU(re, 0, 3) {
							pdf_v.at(re).resize(inp + 1u); cdf_v.at(re).resize(inp + 1u);
						}
					}
					rgb_comp.at(color).at(ni).at(nj) = inp;
					pdf_v.at(color).at(inp) += (LL)(nj % P == 0 || nj % P == P - 1) + (LL)(ni % P == 0 || ni % P == P - 1);
				}
			}
		}
		fs.close();
		//computes cdf_v:
		FU(color, 0, 3) {
			FU(i, 1, (LL)(pdf_v.at(0).size())) {
				cdf_v.at(color).at(i) = cdf_v.at(color).at(i - 1) + (double)(pdf_v.at(color).at(i)) / EDGE_FIELDS;
			}
			FU(i, 1, (LL)(pdf_v.at(0).size())) {//rescale to 1
				cdf_v.at(color).at(i) = cdf_v.at(color).at(i) * 1. / cdf_v.at(color).back();
			}
		}
		//converts to adapted_comp:
		FU(color, 0, 3) {
			FU(ni, 0, N) {
				FU(nj, 0, N) {
					adapted_comp.at(color).at(ni).at(nj) = cdf_v.at(color).at(rgb_comp.at(color).at(ni).at(nj));
				}
			}
		}
		vector<vector<prec_score> > score_v(M * M, vector<prec_score>(M * M));
		vector<prec_score> low_sc(M * M);
		vector<prec_score> best_rat(M * M);
		if (DEBUG) { cout << "precomputing scores\n"; }
		//precomputes scores:
		FU(fii, 0, M) {
			FU(fij, 0, M) {
				FU(sei, 0, M) {
					FU(sej, 0, M) {
						if (fii != sei || fij != sej) {
							double score = 0;
							FU(p, 0, P) {
								FU(color, 0, 3) {
									score += abs(adapted_comp.at(color).at(P * fii + (P - 1)).at(P * fij + p) - adapted_comp.at(color).at(P * sei).at(P * sej + p));
								}
							}
							low_sc.at(fii * M + fij).first_here.at(0) = min(low_sc.at(fii * M + fij).first_here.at(0), score);
							score_v.at(fii * M + fij).at(sei * M + sej).first_here.at(0) = score; score = 0;
							FU(p, 0, P) {
								FU(color, 0, 3) {
									score += abs(adapted_comp.at(color).at(P * fii + p).at(P * fij + (P - 1)) - adapted_comp.at(color).at(P * sei + p).at(P * sej));
								}
							}
							low_sc.at(fii * M + fij).first_here.at(1) = min(low_sc.at(fii * M + fij).first_here.at(1), score);
							score_v.at(fii * M + fij).at(sei * M + sej).first_here.at(1) = score; score = 0;
							FU(p, 0, P) {
								FU(color, 0, 3) {
									score += abs(adapted_comp.at(color).at(P * fii).at(P * fij + p) - adapted_comp.at(color).at(P * sei + (P - 1)).at(P * sej + p));
								}
							}
							low_sc.at(fii * M + fij).first_here.at(2) = min(low_sc.at(fii * M + fij).first_here.at(2), score);
							score_v.at(fii * M + fij).at(sei * M + sej).first_here.at(2) = score; score = 0;
							FU(p, 0, P) {
								FU(color, 0, 3) {
									score += abs(adapted_comp.at(color).at(P * fii + p).at(P * fij) - adapted_comp.at(color).at(P * sei + p).at(P * sej + (P - 1)));
								}
							}
							low_sc.at(fii * M + fij).first_here.at(3) = min(low_sc.at(fii * M + fij).first_here.at(3), score);
							score_v.at(fii * M + fij).at(sei * M + sej).first_here.at(3) = score; score = 0;
						}
					}
				}
			}
		}
		//precomputes the best ratio for each direction for corners and edges:
		FU(dir_id, 0, 4) {
			FU(t_test_id, 0, M * M) {
				FU(t_nei_id, 0, M * M) {
					if (t_test_id != t_nei_id) {
						best_rat.at(t_test_id).first_here.at(dir_id) = min(best_rat.at(t_test_id).first_here.at(dir_id),
							score_v.at(t_test_id).at(t_nei_id).first_here.at(dir_id) / (low_sc.at(t_nei_id).first_here.at((dir_id + 2) % 4) + 0.0000000000001));
					}
				}
			}
		}
		VD minc_rat(4, -1.);// topleft, botleft, botright, topright 
		VLL minc_id(4, -1);
		//precomputes tiles highest corner score
		FU(c_id, 0, 4) {
			FU(t_id, 0, M * M) {
				double here_rat = calc_cor_val(best_rat.at(t_id).first_here.at(c_id), best_rat.at(t_id).first_here.at((c_id + 1) % 4),
					best_rat.at(t_id).first_here.at((c_id + 2) % 4), best_rat.at(t_id).first_here.at((c_id + 3) % 4), 0ll);
				if (here_rat > minc_rat.at(c_id)) {
					minc_rat.at(c_id) = here_rat, minc_id.at(c_id) = t_id;
				}
			}
		}
		//fills puzzle several times and saves result:
		FU(inner_run_id, 0, RUNS) {
			FU(st_var, 0, TOT_ST_VARS) {
				cout << "running HuaweiCOFO new_idea, " << "st_var:" << st_var << " file_id:" << outer_file_id << " inner_run_id:" << inner_run_id << endl;
				LL starting_field_i = rand() % M, starting_field_j = rand() % M;
				LL mini = M + 1, maxi = M + 1, minj = M + 1, maxj = M + 1;
				//computes best "corner tiles":
				if (CORNERS && inner_run_id < 4) {				
					if (1/*DEBUG*/) { cout << "corner " << inner_run_id << ": " << minc_id.at(inner_run_id) << "\n"; }
					starting_field_i = minc_id.at(inner_run_id) / M, starting_field_j = minc_id.at(inner_run_id) % M;
					//sets mini, maxi, minj, maxj
					if (inner_run_id == 0) {
						maxi = 2 * M, maxj = 2 * M;
					}
					else if (inner_run_id == 1) {
						mini = 2, maxj = 2 * M;
					}
					else if (inner_run_id == 2) {
						mini = 2, minj = 2;
					}
					else if(inner_run_id == 3) {
						maxi = 2 * M, minj = 2;
					}
				}
				//computes best "edge tiles":
				if (EDGES && inner_run_id >= 4 && inner_run_id < 8) {
					VLL mine_id(4, -1);
					VD mine_rat(4, -1.);// top, left, bot, right
					LL e_id = inner_run_id - 4;
					FU(t_id, 0, M * M) {
						double here_e_rat = best_rat.at(t_id).first_here.at((e_id + 2) % 4);
						if (here_e_rat > mine_rat.at(e_id)) {
							mine_rat.at(e_id) = here_e_rat, mine_id.at(e_id) = t_id;
						}
					}
					if (1/*DEBUG*/) { cout << "edge " << inner_run_id << ": " << mine_id.at(inner_run_id - 4) << "\n"; }
					starting_field_i = mine_id.at(inner_run_id - 4) / M, starting_field_j = mine_id.at(inner_run_id - 4) % M;
					if (inner_run_id == 4) {
						maxi = 2 * M;
					}
					else if (inner_run_id == 5) {
						maxj = 2 * M;
					}
					else if (inner_run_id == 6) {
						mini = 2;
					}
					else if (inner_run_id == 7) {
						minj = 2;
					}
				}
				vector<vector<vector<pair<double, LL>>>> options(2 * M + 3, vector < vector<pair<double, LL>>>(2 * M + 3, vector<pair<double, LL>>(0)));//all options to insert next tile
				options.at(M + 1).at(M + 1).PB({ 1, M * starting_field_i + starting_field_j });
				map<pair<LL, LL>, pair<LL, LL>> orimap;//save where puzzle tile comes from
				VVLL used(M, VLL(M)), index(2 * M + 3, VLL(2 * M + 3, -1));
				double sum_all_i = 0., sum_all_j = 0.;
				//inserts a tile in each run:
				FU(tile, 0, M*M) {
					double best_score = 100000.; LL best_i = -1, best_j = -1;
					//finds best option:
					FU(opti, mini - (LL)(maxi - mini < M - 1), maxi + 1 + (LL)(maxi - mini < M - 1)) {
						FU(optj, minj - (LL)(maxj - minj < M - 1), maxj + 1 + (LL)(maxj - minj < M - 1)) {
							if (options.at(opti).at(optj).size() != 0 && index.at(opti).at(optj) == -1) {
								double here_score = 0.;
								while (used.at((options.at(opti).at(optj).back().second) / M).at((options.at(opti).at(optj).back().second) % M)) {
									options.at(opti).at(optj).pop_back();
								}
								pair<double, LL> back_p = options.at(opti).at(optj).back();
								options.at(opti).at(optj).pop_back();
								while (options.at(opti).at(optj).size() != 0 && used.at((options.at(opti).at(optj).back().second) / M).at((options.at(opti).at(optj).back().second) % M)) {
									options.at(opti).at(optj).pop_back();
								}
								if (options.at(opti).at(optj).size() == 0) {
									here_score = 1. + dist_penalty(abs(opti - sum_all_i / max(tile, 1ll)), abs(optj - sum_all_j / max(tile, 1ll)), (double)M, st_var + try_st_var * (inner_run_id % 2));//TODO: test factor before M
								}
								else {
									pair<double, LL> sec_back_p = options.at(opti).at(optj).back();
									if (sec_back_p.first == 0) {
										here_score = 1. + dist_penalty(abs(opti - sum_all_i / max(tile, 1ll)), abs(optj - sum_all_j / max(tile, 1ll)), (double)M, st_var + try_st_var*(inner_run_id%2));
									}
									else {
										here_score = back_p.first / sec_back_p.first + dist_penalty(abs(opti - sum_all_i / max(tile, 1ll)), abs(optj - sum_all_j / max(tile, 1ll)), (double)M, st_var + try_st_var * (inner_run_id % 2));
									}
								}
								options.at(opti).at(optj).PB(back_p);
								if (here_score < best_score) {
									best_score = here_score, best_i = opti, best_j = optj;
								}
							}
						}
					}
					LL piece_id = options.at(best_i).at(best_j).back().second;
					used.at(piece_id / M).at(piece_id % M) = 1, index.at(best_i).at(best_j) = options.at(best_i).at(best_j).back().second;
					maxi = max(maxi, best_i), mini = min(mini, best_i), maxj = max(maxj, best_j), minj = min(minj, best_j);
					sum_all_i += best_i, sum_all_j += best_j;
					orimap.insert({ {best_i,best_j}, { piece_id / M, piece_id % M } });
					if (DEBUG) {
						if (1) { cout << "best_score: " << best_score << "\n"; }
						FU(opti, mini - 1, maxi + 2) {
							FU(optj, minj - 1, maxj + 2) {
								if (index.at(opti).at(optj) == -1) {
									cout << "____ ";
								}
								else {
									FU(ii, 0, (LL)(index.at(opti).at(optj) < 1000) + (LL)(index.at(opti).at(optj) < 100) + (LL)(index.at(opti).at(optj) < 10)) { cout << " "; }
									cout << index.at(opti).at(optj) << " ";
								}
							}
							cout << "\n";
						}
					}
					//determines new options:
					FU(fi_dir_id, 0, 4) {
						LL pos_i = best_i + nn.at(fi_dir_id).at(0), pos_j = best_j + nn.at(fi_dir_id).at(1);
						if (index.at(pos_i).at(pos_j) == -1) {
							options.at(pos_i).at(pos_j).clear();
							FU(uui, 0, M) {
								FU(uuj, 0, M) {
									if (!used.at(uui).at(uuj)) {
										double score_sum = 0;
										FU(se_dir_id, 0, 4) {
											if (index.at(pos_i + nn.at(se_dir_id).at(0)).at(pos_j + nn.at(se_dir_id).at(1)) != -1) {
												PLL ori_p = orimap.at({ pos_i + nn.at(se_dir_id).at(0), pos_j + nn.at(se_dir_id).at(1) });
												score_sum += score_v.at(M * uui + uuj).at(M * ori_p.first + ori_p.second).first_here.at(se_dir_id);
											}
										}
										options.at(pos_i).at(pos_j).PB({ score_sum, M * uui + uuj });
									}
								}
							}
							sort(options.at(pos_i).at(pos_j).begin(), options.at(pos_i).at(pos_j).end(), greater<pair<double, LL>>());
						}
					}
				}
				vector<VVD> adapted_sol(3, VVD(N, VD(N)));
				double min_gb = 10000000;
				//makes solution rgb:
				FU(ui, mini, maxi + 1) {
					FU(uj, minj, maxj + 1) {
						LL posi = index.at(ui).at(uj) / M;
						LL posj = index.at(ui).at(uj) % M;
						FU(p1, 0, P) {
							FU(p2, 0, P) {
								FU(color, 0, 3) {
									adapted_sol.at(color).at((ui - mini) * P + p1).at((uj - minj) * P + p2) = adapted_comp.at(color).at(posi * P + p1).at(posj * P + p2);
									if (color) { min_gb = min(min_gb, adapted_comp.at(color).at(posi * P + p1).at(posj * P + p2)); }
								}
							}
						}
					}
				}
				//handles greyscale case:
				if (min_gb == 1.) {
					FU(ntc1, 0, N) {
						FU(ntc2, 0, N) {
							adapted_sol.at(1).at(ntc1).at(ntc2) = max(0., adapted_sol.at(0).at(ntc1).at(ntc2) + 1. - cdf_v.at(0).back());
							adapted_sol.at(2).at(ntc1).at(ntc2) = max(0., adapted_sol.at(0).at(ntc1).at(ntc2) + 1. - cdf_v.at(0).back());
						}
					}
				}
				stringstream ss_rgb; ss_rgb << "Hu_COFO_out_rgb_" << outer_file_id << "_" << inner_run_id << "_st_var_" << st_var << "_pdf.txt";
				ofstream o_rgb(ss_rgb.str());
				//writes to rgb txt file:
				FU(color, 0, 3) {
					FU(i, 0, N) {
						FU(j, 0, N) {
							o_rgb << adapted_sol.at(color).at(i).at(j);
							if (j < N - 1)
								o_rgb << ",";
						}
						o_rgb << "\n";
					}
				}
				o_rgb.close();
				stringstream ss_index; ss_index << "Hu_COFO_out_index_" << outer_file_id << "_" << inner_run_id << "_st_var_" << st_var << "_pdf.txt";
				ofstream o_index(ss_index.str());
				//writes indices to txt file:
				FU(i, mini, maxi + 1) {
					FU(j, minj, maxj + 1) {
						o_index << M * (index.at(i).at(j) / M) + (index.at(i).at(j) % M) << " ";
					}
					o_index << "\n";
				}
				o_index.close();
			}
		}
	}
	return 0;
}
