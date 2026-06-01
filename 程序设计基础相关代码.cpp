(1)
#include <stdio.h>
void print_value(int *ptr)
{
printf("%d",*ptr++);//printf("%d",*ptr); ptr++;
printf("%d",*ptr);
}
int main()
{
int a[ ]={1,3,5,7};
print_value(a);
return 0;
}
//输出结果：13.
(2)
 #include <string.h>
#include <stdio.h>
typedef struct student
{
	char name[10];
	long no;
	float score;
}STU;
int main()
{
	STU a={"zhangsuan",12001,95},b={"Zhangxian",12002,90},c={"Zhanghuan",12003,95};
	STU d,*p=&d;
	d = a;
	if( strcmp( a.name, b.name ) > 0)
		d=b;
	if( strcmp( c.name, d.name ) > 0)
		d=c;
	printf("%ld%s\n", d.no, p->name);//p->name?
	return 0;
}
//输出结果：12002Zhangxian
(3)
//冒泡排序
for(i=0;i<n-1;i++)
for(j=0;j<n-1-i;j++)
if(a[j]>a[j+1])
{
	t=a[j];s[j]=a[j+1];a[j+1]=t;
}
(4)
//质因子分解
#include <stdio.h>
int main()
{
    int n,i=2;
    scanf("%d",&n);
    printf("%d=1",n);
    while(n!=1)
    {
    	while(n%i==0)
    	{
    		printf("*%d",i);
    		n=n/i;
		}
		i++;
	}
	return 0;
}
//输出：18=1*2*3*3 
(5)
//素数的判定
//输出m,n之间的所有素数 
#include<stdio.h>
int prime(int a)
{
	int i;
	for(i=2;i<=a;i++)
	if(a%i==0) break;
	if(a==i)
	return 1;
	else
	return 0;
}
int main()
{
	int m,n,i;
	scanf("%d%d",&m,&n);
	for(i=m;i<=n;i++)
	if(prime(i)==1)
	printf("%d\n",i);
	return 0;
}
(6)
//桶排序!!!!!!!!!!!!!!!!!!!!!!!!!
#include<stdio.h>
int main()
{
	int a[1000]={0},i;
	char ch;
	scanf("%c",&ch);
	while(ch!='#')
	{
		if(ch>='A'&&ch<='Z')
		ch=ch+32;
		if(ch>='a'&&ch<='z')
		a[ch-'a']++;
		scanf("%c",&ch);
	}
	for(i=0;i<1000;i++)
	if(a[i]!=0)
	printf("%c:%d\n",i+'a',a[i]);
	return 0; 
}
(7)
//逆序输出字符串
#include<stdio.h>
#include<string.h>
int main()
{
	char s[100],*p,*q,t;
	int n;
	gets(s);
	n=strlen(s);
	p=s;
	q=s+n-1;
	while(p<=q)
	{
	t=*p;*p=*q;*q=t;
	p++;
	q--;	
	}
	puts(s);
	return 0;	
}
(8)
//回文的判断
int fun(char *str)
{
       int i,j=strlen(str);
       for(i=0;i<j/2;i++)
              if(str[i]!=str[j-i-1])
                     return 0;
       return 1;
}
(9)
//杨辉三角的打印、三角形和回阵的打印 、
#include<stdio.h>
int main()
{
       int i,j,m,n;
       char ch;
       scanf("%c%d",&ch,&m);
       n=(m+1)/2;
       for (i=1;i<=n;i++)
       {
              for (j=0;j<n-i;j++)
                      printf(" ");
              for (j=0;j<2*i-1;j++)
		              putchar(ch);
		      printf("\n");
       }
      return 0;
}
//输出__8
	
	  _888
	
	  88888
//九九乘法表
#include<stdio.h>
int main()
{
	int i,j,result;
	for(i=1;i<10;i++)
	{
		for(j=1;j<10;j++)
		{
			result=i*j;
			printf("%d*%d=%-3d",i,j,result);
		}
		printf("\n");
	}
	return 0;
}
//杨辉三角
#include <stdio.h>
int main()
{
    int i=0,j=0,n,a[100][100]={0};
    scanf("%d",&n);
    int k=0;
    for(i=0;i<n;i++) 
    {
        for(j=0;j<=i;j++) 
        {
            if(j==0 || j==i)  
              a[i][j]=1;
            else                 
                a[i][j]=a[i-1][j-1]+a[i-1][j];
        }
    }
    for(i=0;i<n;i++)
    {
        for(k=1;k<n-i;k++)
            printf("  ");
        for(j=0;j<=i;j++)
            printf("%-4d",a[i][j]);
        printf("\n");
    }
    return 0;
}
//菱角打印
int main()
{
	int m,n,i,j,k;
	char ch;
	scanf("%d%c",&m,&ch);
	n=m/2+1;
	for(i=1;i<=n;i++)
	{
		for(j=1;j<=n-i;j++)
		printf(" ");
		for(k=1;k<2*i;k++)
		{
			printf("%c",ch);
			if(k>1&&k<2*i-1)
			printf("\b ");
		}
		printf("\n");
	}
	for(i=n-1;i>=1;i--)
	{
		for(j=1;j<=n-i;j++)
		printf(" ");
		for(k=1;k<2*i;k++)
		{
			printf("%c",ch);
			if(k>1&&k<2*i-1)
			printf("\b ");//退格键\b 
		}
		printf("\n");
	}
	return 0;
} 
(10)
//斐波那契数列求和 
#include<stdio.h>
int main()
{
       int a,b,c,s,i,n;
       scanf("%d",&n);
       a=b=1;
       s=2;
       printf("1 1 ");
       for (i=3;i<=n;i++)
       {
              c=a+b;
              printf("%d ",c);
              s+=c;
              a=b;
              b=c;
       }
       printf("%d",s);
       return 0;
}
(11)
//求n个数的最小公倍数(重点)
#include<stdio.h>
#define N 105
int gcd(int x,int y){   //求x和y的最大公约数
if(x%y==0);
    return y;
else
    return gcd(y,x%y);
}
int lcm(int x,int y){  //求x和y的最小公倍数
  return x*y/gcd(x,y);
}
int nlcm(int r[],int n)
{
    if(n==2)
    return lcm(r[0],r[1]);
    return lcm(r[0],nlcm(r+1,n-1));
}
int main()
{
int n,i,a[N];
scanf("%d",&n);
for(i=0;i<n;i++)
scanf("%d",&a[i]);
printf("%d",nlcm(a,n));
return 0;
}
(12)
//判断点在三角形的位置
#include<stdio.h>
#include<math.h>
typedef struct point
{
   double x,y;  
} Point;
double dist(Point p1,Point p2)
{
   return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}
double area(Point p1,Point p2,Point p3)
{
   double a,b,c,s;
   a=dist(p1,p2);
   b=dist(p1,p3);
   c=dist(p2,p3);
   s=(a+b+c)/2;
   return sqrt(s*(s-a)*(s-b)*(s-c));
}
int main()
{
   Point O,A,B,C;
   double s0,s1,s2,s3;
   scanf("%lf%lf%lf%lf%lf%lf%lf%lf",&O.x,&O.y,&A.x,&A.y,&B.x,&B.y,&C.x,&C.y);
   s0=area(A,B,C);
   s1=area(A,B,O);
   s2=area(A,C,O);
   s3=area(B,C,O); 
   if (s1+s2+s3>s0)
       printf("Out");
   else if (s1*s2*s3==0)
          printf("On");
        else 
          printf("In");
   return 0;
}
(13)
//删除括号里的字符
#include<stdio.h>
int main()
{
   int prt=1;//ABCD(EFG)JK
   char ch;
   while ((ch=getchar())!='#')
       if (ch=='(') //过滤括号里的字符 
          prt=0;
       else if (ch==')')
              prt=1;//过滤括号里的字符 
           else if (prt)
                putchar(ch); 
   return 0;
}
//
#include<stdio.h>
#include<string.h>
void fun(char *s)
{
       int i, n, k; char c;
       n=0;
       for(i=0; s[i]!='\0'; i++)
              n++;
       if(n%2==0)
              k=n-1;
       else
              k=n-2;
       c=s[k];
       for(i=k-2; i>=1; i=i-2)
              s[i+2]=s[i];
       s[1]=c;
}
int main()
{
       char s[100];
       gets(s);
       fun(s);
       puts(s);
}
//十进制转N进制; 
#include<stdio.h>
void xchg(int x,int n)
{
 	if(x)
 	{
 	xchg(x/n,n);
 	printf("%d",x%n);
 	}
}

int main()
{
 	int x,n,i;
 	scanf("%d",&n);
 	for(int i=0;i<5;i++)
 	{
 	scanf("%d",&x);
 	if(x==0)
 	putchar('0');
 	else
 	xchg(x,n);
 	if(i<4)
 	putchar('\n');
	}
 return 0;
}
