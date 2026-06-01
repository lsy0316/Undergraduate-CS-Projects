//递归法
//斐波那契数列
#include<stdio.h>
int fun(int n){
	if(n<=1)
	return n;
	return fun(n-2)+fun(n-1);
}
int main(){
	int n;
	scanf("%d",&n);
	int result=fun(n);
	printf("%d",result);
	return 0;
}
//逆序输出正整数的各位数
#include<stdio.h>
void fun(int n){
	if(n==0) return;
	else{
	  printf("%d ",n%10);
	  fun(n/10);
	}	
}
int main(){
	int n;
	scanf("%d",&n);
	fun(n);
	return 0;
}
//最大公约数和最小公倍数
#include<stdio.h>
void swap(int *m,int *n){
	int temp;
	temp=*m;
	*m=*n;
	*n=temp;
}
int Rgcb(int m,int n){
	if(m==0) return n;
	return Rgcb(n%m,m);
}
int gcb(int m,int n){
	if(m>n) swap(&m,&n);
	return Rgcb(m,n);
}
int main(){
	int m,n;
	scanf("%d%d",&m,&n);
	int G,L;//最大公约数G，最小公倍数L
	G=gcb(m,n);
	L=(m*n)/G;
	printf("%d %d",G,L);
	return 0; 
}
//已知最大公约数和最小公倍数，怎么算两数之和最小。
#include<stdio.h>
#include<math.h>
int gcb(int m,int n){
	if(m==0)
	return n;
	return gcb(n%m,m);
}
int main(){
	int G,L,m,n;
	scanf("%d%d",&G,&L);
	int mul,sum=100000;
	mul=L/G;
	for(m=1;m<sqrt(mul);m++){
		if(mul%m==0){
			n=mul/m;	
		}
		if(gcb(m,n)==1){
				if(m+n<sum){
					sum=m+n;
				}
		}
	}
	printf("%d",sum*G);
	return 0;
}
//全排列 
//分治法
//最大值和最小值
#include<stdio.h>
void FindMaxMin(int a[],int i,int j,int *max,int *min){
	int max1,min1;
	if(i==j) *max=*min=a[i];
	else if(i==j-1){
		*max=a[i]>a[j]?a[i]:a[j];
		*min=a[i]<a[j]?a[i]:a[j];
	}
	else{
		int m=(i+j)/2;
		FindMaxMin(a,i,m,max,min);
		FindMaxMin(a,m+1,j,&max1,&min1);
		if(*max<max1) *max=max1;
		if(*min>min1) *min=min1;
	}
}
int main(){
	int a[100];
	int n,i;
	int max,min;
	scanf("%d",&n);
	for(i=0;i<n;i++){
		scanf("%d",&a[i]);
	}
	FindMaxMin(a,0,n-1,&max,&min);
	printf("%d %d",max,min);
	return 0;
}
//二分查找元素x
#include<stdio.h>
int FindX(int a[],int i,int j,int x){
	if(i<=j){
		int m=(i+j)/2;
		if(x<a[m]){
			return FindX(a,i,m-1,x);	
		}
		else if(x>a[m]){
			return FindX(a,m+1,j,x);
		}
		else{
			return m;
		}	 
	}	
	return -1;
}
int main(){
	int a[100];
	int n,x;
	scanf("%d",&n);
	for(int i=0;i<n;i++){
		scanf("%d",&a[i]);
	}
	scanf("%d",&x);
	if(FindX(a,0,n-1,x)==-1)
	printf("NO!");
	else{
		printf("YES!\n");
		printf("%d",FindX(a,0,n-1,x));
	}
	return 0;
}
//次小值和次大值
#include <bits/stdc++.h> 
using namespace std;
vector <int> a; 
void f(int left,int right,int &maxv,int &minv,int &cmaxv,int &cminv){
	int maxv1,minv1,cmaxv1,cminv1;
	if (left==right){
		maxv=minv=a[left];
		cmaxv=INT_MIN;
		cminv=INT_MAX;
		return ;
	}
	if (left+1==right){
		cminv=maxv=max(a[left],a[right]);
		cmaxv=minv=min(a[left],a[right]);
		return ;
	}
	int mid=(left+right)/2;
	f(left,mid,maxv,minv,cmaxv,cminv);
	f(mid+1,right,maxv1,minv1,cmaxv1,cminv1);
	cmaxv=max(min(maxv,maxv1),max(cmaxv,cmaxv1));
	cminv=min(max(minv,minv1),min(cminv,cminv1));
	maxv=max(maxv,maxv1);
	minv=min(minv,minv1);

}
int main(){
	int n,maxv,cmaxv;
	int minv,cminv;
	cin>>n;
	a.resize(n);
	int i;
	for(i=0;i<n;i++){
		cin>>a[i];
	}
	f(0,n-1,maxv,minv,cmaxv,cminv);
	cout<<maxv<<" "<<cmaxv;
	cout<<endl;
	cout<<cminv<<" "<<minv;
	return 0;
}
//合并排序
//快速排序 
#include<bits/stdc++.h>
using namespace std;
int Partion(vector<int> &a,int left,int right){
	srand(time(NULL));
	int p=left+rand()%(right-left+1);
	swap(a[left],a[p]);
	int pivot=a[left];
	int i=left,j;
	for(j=i+1;j<=right;j++){
		if(a[j]<=pivot){
			i++;
			swap(a[i],a[j]);
		}
	}
	swap(a[i],a[left]);
	return i;
}
//求第k小元素 
//查找给定值的最接近元素
//贪心法
//贪心背包问题
//带时限作业排序问题
//最小代价生成树
//单源最短路径 
#include <bits/stdc++.h>
using namespace std;

typedef char Vertex;
typedef int ArcType;
const int MAXV = 100;
const int INF = INT_MAX;

// 定义邻接矩阵图的结构体
typedef struct {
    int vexs[MAXV]; // 存储顶点的数组
    ArcType arcs[MAXV][MAXV]; // 存储边的二维数组
    int vexnum, arcnum; // 顶点数和边数
} AMGraph;

// 创建邻接矩阵的函数
void createAdjMatrix(AMGraph *G) {
    int i, j, k, v1, v2, w;
    for (i = 0; i < G->vexnum; i++) {
        for (j = 0; j < G->vexnum; j++) {
            G->arcs[i][j] = INF; // 初始化所有边的权重为无穷大
        }
    }
    for (k = 0; k < G->arcnum; k++) {
        cin >> v1 >> v2 >> w;
        int index1 = -1, index2 = -1;
        for (i = 0; i < G->vexnum; i++) {
            if (G->vexs[i] == v1) index1 = i; //找到起点和终点 
            if (G->vexs[i] == v2) index2 = i; 
        }
        if (index1 != -1 && index2 != -1) {
            G->arcs[index1][index2] = w;
        }
    }
}

int choose(int d[], bool inS[], int n) {
    int i, minpos, min;
    min = INF;
    minpos = -1;
    for (i = 0; i < n; i++) {
        if (d[i] < min && !inS[i]) {
            min = d[i];
            minpos = i;
        }
    }
    return minpos;
}

void Dijkstra(int s, int d[], int path[], AMGraph *G) {
    int k, i, j;
    if (s < 0 || s >= G->vexnum) {
        return;
    }
    bool inS[MAXV] = {false};
    for (i = 0; i < G->vexnum; i++) {
        inS[i] = false;
        d[i] = G->arcs[s][i];
        if (i != s && d[i] < INF)
            path[i] = s;
        else
            path[i] = -1;
    }
    inS[s] = true; d[s] = 0;
    for (i = 1; i < G->vexnum; i++) {
        k = choose(d, inS, G->vexnum);
        if (k == -1) break; // 如果找不到未访问的节点，则退出循环
        inS[k] = true;
        for (j = 0; j < G->vexnum; j++) {
            if (!inS[j] && G->arcs[k][j] < INF && d[k] + G->arcs[k][j] < d[j]) {
                d[j] = d[k] + G->arcs[k][j];
                path[j] = k;
            }
        }
    }
}

int main() {
    AMGraph G;
    int n, m, k;
    cin >> n >> m >> k;
    int i;
    G.vexnum = n;
    G.arcnum = m;
    int d[MAXV] = {0};
    int path[MAXV] = {0};
    for (i = 0; i < n; i++) {
        G.vexs[i] = i + 1; // 假设顶点编号从1开始
    }
    createAdjMatrix(&G);
    Dijkstra(k - 1, d, path, &G); // 将源点转换为索引（从0开始）
    for (i = 0; i < n; i++) {
        if (i != k - 1) { // 不输出源点到自身的距离
            if (d[i] == INF) {
                cout << "INFTY" << " ";
            } else {
                cout << d[i] << " ";
            }
        }
    }
    cout << endl;
    return 0;
}
//动态规划法
//多段图问题
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#define MVNum 100  // 最大顶点数 
#define OK 1
#include <bits/stdc++.h>
using namespace std;
typedef int Status;
typedef int ArcType;
typedef char VerTexType;

typedef struct ArcNode           //边结点
{
    int adjvex;                  //该边所指向的顶点的位置
    struct ArcNode *nextarc;     //指向下一条边的指针
    int info;              //和边相关的信息(权值等)
}ArcNode;
typedef struct VNode
{
    VerTexType data;         //顶点信息
    ArcNode *firstarc;       //指向第一条依附该顶点的边的指针
}VNode,AdjList[MVNum];
typedef struct
{
    AdjList vertices;      //存放各个顶点的数组
    int vexnum,arcnum;    //图的当前顶点数和弧数
}ALGraph;
// 创建邻接表
Status CreateAdjlist(ALGraph *G) {
    int i,k,v1,v2,w; 
    ArcNode *s;
    // 初始化顶点表，将每个顶点的邻接链表头指针设为NULL
    for (i=0;i<G->vexnum;i++) {
        G->vertices[i].data=i;
        G->vertices[i].firstarc=NULL;
    }
    for (k = 0; k < G->arcnum; k++) {
        cin>>v1>>v2>>w;
        s = (ArcNode *)malloc(sizeof(ArcNode));
        s->adjvex=v2;
        s->info=w;
        s->nextarc=G->vertices[v1].firstarc;
        G->vertices[v1].firstarc=s;
    }
    return OK;
}
int DP(ALGraph G,int n,int m,int k,int *p) {
    int cost[n],d[n-1];
    int q,j;
    ArcNode *r;
    cost[n-1]=0,d[n-1]=-1;
    for(j=n-2;j>=0;j--){
        int min=INT_MAX;
        for(r=G.vertices[j].firstarc;r;r=r->nextarc){
            int v=r->adjvex;
            if (r->info+cost[v]<min){
                min=r->info+cost[v];
                q=v;
            }
        }
        cost[j]=min;
        d[j]=q;
    }
    p[0]=0;p[k-1]=n-1;
    int c=cost[0];
    for(j=1;j<=k-2;j++){
        p[j]=d[p[j-1]];
    }
    return c;

}

int main() {
    ALGraph G;
    int n,m;
    cin>>n>>m;
    G.vexnum=n;
    G.arcnum=m;
    CreateAdjlist(&G);
    int p[n];
    int c=DP(G,n,m,n,p);
  	cout<<c;
    return 0;
}
//矩阵连乘
#include <bits/stdc++.h>
using namespace std;
int Max(vector<int> &p,int n){
	int i,r;
	int m[n][n],s[n][n];
	for(i=0;i<n;i++){
		m[i][i]=0;
	}
	for(r=2;r<=n;r++){
		for(i=0;i<=n-r;i++){
			int j=i+r-1;
			m[i][j]=m[i+1][j]+p[i]*p[i+1]*p[j+1];
			s[i][j]=i;
			int k;
			for(k=i+1;k<j;k++){
				int t=m[i][k]+m[k+1][j]+p[i]*p[k+1]*p[j+1];
				if(t<m[i][j]){
					m[i][j]=t;
					s[i][j]=k;
				}
			}
		}
	}
	return m[0][n-1];
}
int main(){
	int n;
	cin>>n;
	vector<int> p(n+1);
	int i;
	for(i=0;i<=n;i++){
		cin>>p[i];
	}
	int res=Max(p,n);
	cout<<res;
	return 0;
}
//最长公共子序列
#include <bits/stdc++.h>
using namespace std;
#include <string.h>

char x[101];
char y[101];

int Max(int m, int n){
    // 使用动态规划表来存储LCS长度
    int s[m+1][n+1];
    int c[m+1][n+1];
    for(int i=0;i<=m;i++){
        c[i][0]=0;
    }
    for(int j=0;j<=n;j++){
        c[0][j]=0;
    } 
    for(int i=1;i<=m;i++){
        for(int j=1;j<=n;j++){
            if (x[i-1]==y[j-1]){
                c[i][j]=c[i-1][j-1]+1;
                s[i][j]=1;
            }
            else if(c[i-1][j]>=c[i][j-1]){
                c[i][j]=c[i-1][j];
               	s[i][j]=2;
            }
            else{
                c[i][j]=c[i][j-1];
                s[i][j]=3;
            }
        }
    }
    return c[m][n];
}

int main(){
    // 读取两个字符串
    scanf("%s",x);
    int m=strlen(x);
    scanf("%s",y);
    int n=strlen(y);
    int res=Max(m,n);
    printf("%d",res);
    return 0;
}
//水龙头问题
#include <bits/stdc++.h>
using namespace std;
bool cmp(int i, int j) {
    return i < j;
}
int main() {
    int n, m;
    cin >> n >> m;
    vector<int> T(n);
    int time = 0; //总的等待时间
    for (int i = 0; i < n; i++) {
        cin >> T[i];
    }
    sort(T.begin(),T.end(),cmp);
    vector<int> et(m,0);
    for (int i=0;i<n;i++) {
        // 找到最早空闲的水龙头
        int min=0;
        for (int j=1;j<m;j++) {
            if (et[j] < et[min]){
                min= j;
            }
        }
        // 计算等待时间并更新水龙头的结束时间
        time=time+et[min];
        et[min]+=T[i];
    }

    cout<<time;
    return 0;
}
 
//最长递增子序列
#include <bits/stdc++.h>
using namespace std;
// 即为最长递增子序列问题 
int main(){	
    int n;
    cin >> n;
    vector<int> h(n);
    vector<int> s(n, 1); // 初始化所有位置为1
    for(int i = 0; i < n; i++) cin >> h[i];
    
    for(int i = 1; i < n; i++) {
        for(int j = 0; j < i; j++) {
            if(h[i] > h[j] && s[j] + 1 > s[i]) {
                s[i] = s[j] + 1;
            }
        }
    }
    
    int maxLength = s[0];
    for(int i = 1; i < n; i++) {
        if(s[i] > maxLength) {
            maxLength = s[i];
        }
    }
    cout << maxLength;
    return 0;	
}

//0/1背包问题 
#include <bits/stdc++.h>
using namespace std;

int main(){
    int n, M;
    cin >> n >> M;
    vector<int> w(n+1);
    vector<int> p(n+1);
    for(int i = 1; i <= n; i++) cin >> w[i];
    for(int i = 1; i <= n; i++) cin >> p[i];
    vector<vector<int> > cost(n+1, vector<int>(M+1, 0));
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= M; j++){
            if (j >= w[i]){
                cost[i][j] = max(p[i] + cost[i-1][j - w[i]], cost[i-1][j]);
            }
            else{
                cost[i][j] = cost[i-1][j];
            }
        }
    }
    cout << cost[n][M];
    return 0;
}
//回溯法 
//n-皇后问题 
#include <bits/stdc++.h>
using namespace std;

int n;
vector<int> x;
bool can_place(int k){
    for(int j = 0; j < k; j++){
        if (k == j || x[k] == x[j] || abs(k - j) == abs(x[k] - x[j])){
            return false;
        }
    }
    return true;
}
void dfs(int k){
    if(k == n){
        for(int i=0;i<n;i++){
            cout<<x[i] <<" ";
        }
        cout << endl;
        return;
    }
    if(k==0){
        for(int i=0;i<n/2;i++){
            x[k] = i;
            if(can_place(k)){
                dfs(k + 1);
            }
        }
    }
    else{
        for(int i = 0; i < n; i++){
            x[k] = i;
            if(can_place(k)){
                dfs(k + 1);
            }
        }
    }
}
int main(){
    cin >> n;
    x.resize(n);
    dfs(0);
    return 0;
}
//0/1背包问题
#include <bits/stdc++.h>
using namespace std; 
struct Data{
	int x;
	int id;
	double pi,wi;
	bool operator<(const Data &d)const{
		return pi/wi>d.pi/d.wi;
	} 
}bag[100];
int x[100],ans[100];
int n,M;
double best=0.0;//最好的收益 
double bound(int k,double cp,double cw){
	double b=cp;
	double c=cw;
	for(int j=k+1;j<n;j++){
		if(c>=bag[j].wi){
			b=b+bag[j].pi;
			c=c-bag[j].wi;
		}
		else{
			b=b+(c/bag[j].wi)*bag[j].pi;
			break;
		}
	}
	return b;
}
void dfs(int k,double cp,double cw){
	if (k==n){
		if (cp>best){
			best=cp;
			for(int i=0;i<n;i++){
				ans[bag[i].id]=x[i];
			}
		}
		return ;
	}
	if(cw>=bag[k].wi){//可以放的下 
		x[k]=1;
		dfs(k+1,cp+bag[k].pi,cw-bag[k].wi);
	}
	if (bound(k,cp,cw)>best){
		x[k]=0;
		dfs(k+1,cp,cw);
	}
}
int main(){
	cin>>n>>M;
	for(int i=0;i<n;i++){
		cin>>bag[i].pi;
		bag[i].id=i;
	}
	for(int i=0;i<n;i++){
		cin>>bag[i].wi;
	}
	sort(bag,bag+n);
	dfs(0,0,M);
	cout<<best<<endl;
	for(int i=0;i<n;i++){
		cout<<ans[i]<<" ";
	}
	return 0;
}
//图的着色问题 
#include <bits/stdc++.h>
using namespace std;
int n,m,e;
int x[50]={0};//表示第k个国家涂x[k]颜色 
int g[50][50]={0}; 
bool flag = false;  
bool can_color(int k){
	for(int j=0;j<k;j++){
		if (g[k][j]==1 && x[k]==x[j]){
			return false;
		}
	}
	return true;
}
void dfs(int k){
	if (k==n){
		flag=true; 
		for(int i=0;i<n;i++){
			cout<<x[i]<<" ";
		}
		cout<<endl;
		return ;
	}
	for(int i=1;i<=m;i++){
		x[k]=i;
		if(can_color(k)){
			dfs(k+1);
		}
	}
}
int main(){
	cin>>n>>e>>m;
	for(int i=0;i<e;i++){
		int s1,s2;
		cin>>s1>>s2;
		g[s1][s2]=g[s2][s1]=1;
	}

	dfs(0);
		if (!flag){
		cout<<"No solution";
	}
	return 0;
}
