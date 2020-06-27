#include<bits/stdc++.h>
#define rep(i,a,n) for(int i=a;i<=n;i++)
#define per(i,a,n) for(int i=n;i>=a;i--)
#define pb push_back
#define mp make_pair
#define FI first
#define SE second
#define maxn 100000
#define mod 1000000007
#define inf 0x3f3f3f3f
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;
typedef vector<int> vi;
typedef double db;

int score[maxn+5],init_score[maxn+5];
int id[maxn+5],pl[maxn+5];

struct 
{
	int N,T,M;
	vi bid,sum,ban,cnt;
	void init()
	{
		bid.resize(N+1,0);
		sum.resize(N+1,0);
		cnt.resize(N+1,0);
		ban.resize(N+1,0);
	}
	void reset()
	{
		rep(i,0,N) sum[i]=0;
		rep(i,0,N) ban[i]=0;
		rep(i,0,N) cnt[i]=0;
		rep(j,1,N) add(j,score[bid[j]]);
		rep(j,1,N) inc(j,1);
	}
	void inc(int x,int v)
	{
		for(;x<=N;x+=x&-x) cnt[x]+=v;
	}
	int CC(int x)
	{
		int res=0;
		for(;x;x-=x&-x) res+=cnt[x];
		return res; 
	}
	void add(int x,int v)
	{
		for(;x<=N;x+=x&-x) sum[x]+=v;
	}
	int ask(int x)
	{
		int res=0;
		for(;x;x-=x&-x) res+=sum[x];
		return res; 
	}
	void del(int x)
	{
		int p=lower_bound(bid.begin(),bid.end(),x)-bid.begin();
		inc(p,-1);
		add(p,-score[x]);
		ban[p]=1;
	}
	int find(int x)
	{
		int l=1,r=N+1;
		while(l<r)
		{
			int mid=(l+r)>>1;
			if(CC(mid)>x) r=mid;
			else l=mid+1;
		}
		return l-1;
	}
	int que(int x)
	{
		int n=find(x);
		if(n>N) n=N;
		return ask(n);
	}
}lib[1005];
int BAN[1005];

vi lst[maxn+5];

bool cmp(int x,int y) {return score[x]>score[y];}


void del_book(int x)
{
	for(auto l: lst[x]) lib[l].del(x);
}

int ans[1000+5],final_ans[1000+5],final_tot;
vi bk[1000+5],final_bk[1000+5];
int best_ans;

int B,L,D,OD; 
bool chosen[maxn+5];

void check(int tot)
{
	rep(i,0,B-1) chosen[i]=0;
	rep(i,1,tot) for(auto b: bk[i]) chosen[b]=1;
	int score=0;
	rep(i,0,B-1) if(chosen[i]) score+=init_score[i];
	if(score>best_ans)
	{
		best_ans=score;
		final_tot=tot;
		rep(i,1,tot) final_ans[i]=ans[i];
		rep(i,1,tot) final_bk[i]=bk[i];
	}
}

void solve(int fid)
{
	D=OD;
	rep(i,1,L) lib[i].reset();
	
	int tot=0;
	rep(i,1,L) BAN[i]=0;

	rep(t,1,L)
	{
		if(D==0) break;
		int mid=-1,mp=0,mq=1;
		rep(i,1,L) if(BAN[i]==0 && D>lib[i].T)
		{
			int P=lib[i].que((D-lib[i].T)*lib[i].M);
			int Q=lib[i].T;
			if((mid==-1 && P!=0) || 1ll*P*mq>1ll*Q*mp)
			{
				mid=i;
				mp=P;
				mq=Q;
			}
		}
		if(t==1) mid=fid;
		if(mid==-1) break;
		int n=lib[mid].find((D-lib[mid].T)*lib[mid].M);
		if(n>lib[mid].N) n=lib[mid].N;
		tot++;
		ans[tot]=mid-1;
		/*if(ans[tot]==66)
		{
			printf("?? %d\n",lib[mid].N);
			rep(i,1,lib[mid].N) printf("%d: %d\n",i,lib[mid].ban[i]);
		}*/
		bk[tot].clear();
		rep(i,1,n) if(lib[mid].ban[i]==0) 
		{
			bk[tot].pb(id[lib[mid].bid[i]]-1);
			del_book(lib[mid].bid[i]);
		}
		//sc[tot]=bk[tot].size();
		BAN[mid]=1;
		D-=lib[mid].T;
	}
	check(tot);
	//printf("?? %d\n",final_tot);
}

int main()
{
	scanf("%d%d%d",&B,&L,&OD);
	rep(i,1,B) scanf("%d",&score[i]),init_score[i-1]=score[i];
	rep(i,1,B) id[i]=i;
	sort(id+1,id+B+1,cmp);
	sort(score+1,score+B+1,greater<int>());
	rep(i,1,B) pl[id[i]]=i;

	rep(i,1,L)
	{
		scanf("%d%d%d",&lib[i].N,&lib[i].T,&lib[i].M);
		lib[i].init();
		rep(j,1,lib[i].N)
		{
			int x; scanf("%d",&x); x++;
			lib[i].bid[j]=pl[x];
			lst[pl[x]].pb(i);
		}
		sort(lib[i].bid.begin(),lib[i].bid.end());
	}
	rep(i,1,L) solve(i);
	
	printf("%d\n",final_tot);
	rep(i,1,final_tot) 
	{
		int sz=final_bk[i].size();
		printf("%d %d\n",final_ans[i],sz);
		rep(j,0,sz-1) printf("%d%c",final_bk[i][j]," \n"[j==sz-1]);
	}
	//printf("??? %d\n",best_ans);
	return 0;
}
