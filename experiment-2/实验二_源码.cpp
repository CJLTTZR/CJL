#include <bits/stdc++.h>
using namespace std;
using ll=long long;
ll ops=0;
const ll INF=LLONG_MAX/4;
// 0-1背包：dp[i][c]表示前i件物品在容量c内的最大价值。
ll knapsack(vector<int> w, vector<int> v,int C){
 int n=w.size(); vector<vector<ll>> d(n+1,vector<ll>(C+1));
 for(int i=1;i<=n;i++) for(int c=0;c<=C;c++){
  ++ops; d[i][c]=d[i-1][c];
  if(c>=w[i-1]) d[i][c]=max(d[i][c],d[i-1][c-w[i-1]]+v[i-1]);
 }
 return d[n][C];
}
// 数塔：滚动数组从底向顶更新，支持负数。
ll triangle(vector<vector<ll>> a){
 if(a.empty()) return 0;
 vector<ll> d=a.back();
 for(int i=(int)a.size()-2;i>=0;i--) for(int j=0;j<=i;j++){
  ++ops; d[j]=a[i][j]+max(d[j],d[j+1]);
 }
 return d[0];
}
// 钢条切割：价格表p从1开始；整根必须售出，允许不切割。
ll rod(vector<ll> p,int n){
 vector<ll>d(n+1,-INF); d[0]=0;
 for(int j=1;j<=n;j++) for(int i=1;i<=j;i++){
  ++ops; d[j]=max(d[j],p[i]+d[j-i]);
 }
 return d[n];
}
// LIS：严格递增，返回长度。
int lis(vector<int>a){
 int n=a.size(),ans=0; vector<int>d(n,1);
 for(int i=0;i<n;i++){
  for(int j=0;j<i;j++){
   ++ops; if(a[j]<a[i]) d[i]=max(d[i],d[j]+1);
  }
  ans=max(ans,d[i]);
 }
 return ans;
}
// LCS：返回长度。
int lcs(string a,string b){
 int m=a.size(),n=b.size(); vector<vector<int>>d(m+1,vector<int>(n+1));
 for(int i=1;i<=m;i++) for(int j=1;j<=n;j++){
  ++ops;
  d[i][j]=(a[i-1]==b[j-1])?d[i-1][j-1]+1:max(d[i-1][j],d[i][j-1]);
 }
 return d[m][n];
}
// 多段图：顶点按阶段拓扑编号，所有边满足u<v；可含负权。
ll multistage(vector<vector<pair<int,ll>>>g,int s,int t){
 int n=g.size(); vector<ll>d(n,INF); d[t]=0;
 for(int u=n-1;u>=0;u--){
  if(u==t) continue;
  for(auto e:g[u]){ ++ops;
   if(d[e.first]!=INF) d[u]=min(d[u],e.second+d[e.first]);
  }
 }
 return d[s];
}
int main(){
 auto check=[](string name,ll got,ll want){
  cout<<name<<" | actual="<<got<<" expected="<<want<<" "<<(got==want?"PASS":"FAIL")<<"\n";
  if(got!=want) exit(1);
 };
 check("knapsack sample",knapsack({2,3,4,5},{3,4,5,6},8),10);
 check("knapsack empty",knapsack({}, {},5),0);
 check("knapsack capacity0",knapsack({1},{5},0),0);
 check("knapsack tooheavy",knapsack({9},{10},3),0);
 check("triangle sample",triangle({{7},{3,8},{8,1,0},{2,7,4,4},{4,5,2,6,5}}),30);
 check("triangle negative",triangle({{-2},{-3,-4}}),-5);
 check("triangle single",triangle({{9}}),9);
 check("triangle empty",triangle({}),0);
 check("rod sample",rod({0,1,5,8,9,10,17,17,20},8),22);
 check("rod zero",rod({0},0),0);
 check("rod single",rod({0,3},1),3);
 check("rod negative",rod({0,-2,-5},2),-4);
 check("lis sample",lis({10,9,2,5,3,7,101,18}),4);
 check("lis equal",lis({2,2,2}),1);
 check("lis decreasing",lis({3,2,1}),1);
 check("lis empty",lis({}),0);
 check("lcs sample",lcs("ABCBDAB","BDCABA"),4);
 check("lcs empty",lcs("","ABC"),0);
 check("lcs disjoint",lcs("ABC","XYZ"),0);
 check("lcs same",lcs("AAAA","AAAA"),4);
 vector<vector<pair<int,ll>>>g(6);
 g[0]={{1,2},{2,5}};g[1]={{3,4},{4,6}};g[2]={{3,1},{4,2}};g[3]={{5,3}};g[4]={{5,1}};
 check("graph sample",multistage(g,0,5),8);
 check("graph unreachable",multistage(vector<vector<pair<int,ll>>>(2),0,1),INF);
 check("graph single",multistage(vector<vector<pair<int,ll>>>(1),0,0),0);
 check("graph negative",multistage({{{1,-3}},{}},0,1),-3);
 ofstream f("work/counts.csv");f<<"algorithm,n,parameter,count,result\n";
 for(int n:{10,20,40,80,160,320}){
  ops=0; ll r=knapsack(vector<int>(n,1),vector<int>(n,1),n);f<<"knapsack,"<<n<<","<<n<<","<<ops<<","<<r<<"\n";
  vector<vector<ll>> a(n);for(int i=0;i<n;i++)a[i]=vector<ll>(i+1,1);
  ops=0;r=triangle(a);f<<"triangle,"<<n<<",0,"<<ops<<","<<r<<"\n";
  vector<ll>p(n+1);iota(p.begin(),p.end(),0);
  ops=0;r=rod(p,n);f<<"rod,"<<n<<",0,"<<ops<<","<<r<<"\n";
  vector<int>b(n);iota(b.begin(),b.end(),0);
  ops=0;r=lis(b);f<<"lis,"<<n<<",0,"<<ops<<","<<r<<"\n";
  ops=0;r=lcs(string(n,'A'),string(n,'A'));f<<"lcs,"<<n<<","<<n<<","<<ops<<","<<r<<"\n";
  vector<vector<pair<int,ll>>>h(n);
  for(int u=0;u<n-1;u++)for(int v=u+1;v<min(n,u+3);v++)h[u].push_back({v,v-u});
  ops=0;r=multistage(h,0,n-1);f<<"graph,"<<n<<","<<2*n-3<<","<<ops<<","<<r<<"\n";
 }
}
