#include "header-file/high.h"

char* add(char* a1,char* b1)
{
	int lena=strlen(a1),lenb=strlen(b1),lenc=0;
	int *a=new int[lena+1],b[lenb+1],c[(lena>lenb?lena:lenb)+2],i,x;
	memset(a,0,sizeof(a));
	memset(b,0,sizeof(b));
	memset(c,0,sizeof(c));
	for(i=0;i<=lena-1;i++)
	  a[lena-i]=a1[i]-48;
	for(i=0;i<=lenb-1;i++)
	  b[lenb-i]=b1[i]-48;
	lenc=1;
	x=0;
	while(lenc<=lena||lenc<=lenb)
	{
		c[lenc]=a[lenc]+b[lenc]+x;
		x=c[lenc]/10;
		c[lenc]%=10;
		lenc++;
	}
	c[lenc]=x;
	if(c[lenc]==0)
	  lenc--;
	char* s=new char[lenc+1];
	int lens=0;
	for(i=lenc;i>=1;i--)
	{
		s[lens++]=c[i]+'0';
		s[lens]='\0';
	}
	return s;
}

char* sub(char* n1,char* n2)
{
	
}

char* mul(char* a1,char* b1)
{
	int lena=strlen(a1),lenb=strlen(b1);
	int *a=new int[lena+1],*b=new int[lenb+1],*c=new int[lena+lenb+1],lenc=0,i,j,x;
	memset(a,0,sizeof(a));
	memset(b,0,sizeof(b));
	memset(c,0,sizeof(c));
	for(i=0;i<=lena-1;i++)
	  a[lena-i]=a1[i]-48;
	for(i=0;i<=lenb-1;i++)
	  b[lenb-i]=b1[i]-48;
	for(i=1;i<=lena;i++)
	{
		x=0;
		for(j=1;j<=lenb;j++)
		{
			c[i+j-1]=a[i]*b[j]+x+c[i+j-1];
			x=c[i+j-1]/10;
			c[i+j-1]%=10;
		}
		c[i+lenb]=x;
	}
	lenc=lena+lenb;
	while(c[lenc]==0&&lenc>1)
	  lenc--;
	char* s=new char[lenc+1];
	int lens=0;
	for(i=lenc;i>=1;i--)
	{
		s[lens++]=c[i]+'0';
		s[lens]='\0';
	}
	return s;
}

void numcpy(int p[],int q[],int det)
{
	for(int i=1;i<=p[0];i++)
	  q[i+det-1]=p[i];
	q[0]=p[0]+det-1;
}

int compare(int a[],int b[])
{
	int i;
	if(a[0]>b[0])
	  return 1;
	if(a[0]<b[0])
	  return -1;
	for(i=a[0];i>0;i--)
	{
		if(a[i]>b[i])
		  return 1;
		if(a[i]<b[i])
		  return -1;
	}
	return 0;
}

void jiana(int a[],int b[])
{
	int flag,i;
	flag=compare(a,b);
	if(flag==0)
	{
		a[0]=0;
		return;
	}
	if(flag==1)
	{
		for(i=1;i<=a[0];i++)
		{
			if(a[i]<b[i])
			{
				a[i+1]--;
				a[i]+=10;
			}
			a[i]-=b[i];
		}
		while(a[0]>0&&a[a[0]]==0)
		  a[0]--;
		return;
	}
}

void chugao(int a[],int b[],int c[])
{
	int i,tmp[101];
	c[0]=a[0]-b[0]+1;
	for(i=c[0];i>0;i--)
	{
		memset(tmp,0,sizeof(tmp));
		numcpy(b,tmp,i);
		while(compare(a,tmp)>=0)
		{
			c[i]++;
			jiana(a,tmp);
		}
	}
	while(c[0]>0&&c[c[0]]==0)
	  c[0]--;
	return ;
}

void init(int a[],char* s)
{
	int i;
	a[0]=strlen(s);
	for(i=1;i<=a[0];i++)
	  a[i]=s[a[0]-i]-'0';
}

char* _div(char* one,char* two)
{
	int *a=new int[strlen(one)+1],*b=new int[strlen(two)+1],c[strlen(one)+strlen(two)],d,i;
	memset(a,0,sizeof(a));
	memset(b,0,sizeof(b));
	memset(c,0,sizeof(c));
	init(a,one);
	init(b,two);
	chugao(a,b,c);
	char* s=new char[c[0]+1];
	int lens=0;
	if(c[0]==0)
	{
		s[lens++]='0';
		s[lens]='\0';
		return s;
	}
	for(i=c[0];i>0;i--)
	{
		s[lens++]=c[i]+'0';
		s[lens]='\0';
	}
	return s;
}

char* mod(char* one,char* two)
{
	int *a=new int[strlen(one)+1],*b=new int[strlen(two)+1],c[strlen(one)+strlen(two)],d,i;
	memset(a,0,sizeof(a));
	memset(b,0,sizeof(b));
	memset(c,0,sizeof(c));
	init(a,one);
	init(b,two);
	chugao(a,b,c);
	char* s=new char[a[0]+1];
	int lens=0;
	if(a[0]==0)
	{
		s[lens++]='0';
		s[lens]='\0';
		return s;
	}
	for(i=a[0];i>0;i--)
	{
		s[lens++]=a[i]+'0';
		s[lens]='\0';
	}
	return s;
}
