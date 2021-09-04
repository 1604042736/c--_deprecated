#include "header-file/Preprocessor.h"

struct Define_list
{
	char* name;
	char* val;
};
vector<Define_list> definel;
vector<bool> factor;

vector<char> filei;

char pre[1024];
char temp[1024],_temp[1024];
int templen=0;

int define_find()
{
	for(int i=0;i<definel.size();i++)
	{
		if(!strcmp(temp,definel[i].name)&&strcmp(_temp,definel[i].name))
		{
			return i;
		}
	}
	return -1;
}

void pretreatment(char* filename,FILE* file)
{
	int lenpre=0;
	char t[1024];
	int lent=0;
	for(;lenpre<strlen(pre);lenpre++)
	{
		if((pre[lenpre]>='a'&&pre[lenpre]<='z')||(pre[lenpre]>='A'&&pre[lenpre]<='Z')||pre[lenpre]=='_')
		{
			t[lent++]=pre[lenpre];
			t[lent]='\0';
		}
		else
		{
			break;
		}
	}
	lenpre++;
	if(!strcmp(t,"define"))
	{
		Define_list dl;
		char* one=new char[1024];
		char* two=new char[1024];
		lent=0;
		while(pre[lenpre]==' '||pre[lenpre]=='\t')
		{
			lenpre++;
		}
		for(;lenpre<strlen(pre);lenpre++)
		{
			if((pre[lenpre]>='a'&&pre[lenpre]<='z')||(pre[lenpre]>='A'&&pre[lenpre]<='Z')||pre[lenpre]=='_')
			{
				one[lent++]=pre[lenpre];
				one[lent]='\0';
			}
			else
			{
				break;
			}
		}
		dl.name=one;
		lent=0;
		lenpre++;
		while(pre[lenpre]==' '||pre[lenpre]=='\t')
		{
			lenpre++;
		}
		for(;lenpre<strlen(pre);lenpre++)
		{
			if(pre[lenpre]!='\n')
			{
				two[lent++]=pre[lenpre];
				two[lent]='\0';
			}
			else
			{
				break;
			}
		}
		dl.val=two;
		definel.push_back(dl);
	}
	else if(!strcmp(t,"undef"))
	{
		char* one=new char[1024];
		while(pre[lenpre]==' '||pre[lenpre]=='\t')
		{
			lenpre++;
		}
		lent=0;
		for(;lenpre<strlen(pre);lenpre++)
		{
			if((pre[lenpre]>='a'&&pre[lenpre]<='z')||(pre[lenpre]>='A'&&pre[lenpre]<='Z')||pre[lenpre]=='_')
			{
				one[lent++]=pre[lenpre];
				one[lent]='\0';
			}
			else
			{
				break;
			}
		}
		for(int i=0;i<definel.size();i++)
		{
			if(!strcmp(one,definel[i].name))
			{
				definel.erase(definel.begin()+i);
				break;
			}
		}
	}
	else if(!strcmp(t,"ifdef"))
	{
		char* one=new char[1024];
		bool b=true;
		while(pre[lenpre]==' '||pre[lenpre]=='\t')
		{
			lenpre++;
		}
		lent=0;
		for(;lenpre<strlen(pre);lenpre++)
		{
			if((pre[lenpre]>='a'&&pre[lenpre]<='z')||(pre[lenpre]>='A'&&pre[lenpre]<='Z')||pre[lenpre]=='_')
			{
				one[lent++]=pre[lenpre];
				one[lent]='\0';
			}
			else
			{
				break;
			}
		}
		for(int i=0;i<definel.size();i++)
		{
			if(!strcmp(one,definel[i].name))
			{
				b=false;
			}
		}
		if(b)
		{
			char ch=getCh(file);
			while(ch!='\n')
			{
				ch=getCh(file);
			}
		}
		factor.push_back(!b);
	}
	else if(!strcmp(t,"ifndef"))
	{
		char* one=new char[1024];
		bool b=true;
		while(pre[lenpre]==' '||pre[lenpre]=='\t')
		{
			lenpre++;
		}
		lent=0;
		for(;lenpre<strlen(pre);lenpre++)
		{
			if((pre[lenpre]>='a'&&pre[lenpre]<='z')||(pre[lenpre]>='A'&&pre[lenpre]<='Z')||pre[lenpre]=='_')
			{
				one[lent++]=pre[lenpre];
				one[lent]='\0';
			}
			else
			{
				break;
			}
		}
		for(int i=0;i<definel.size();i++)
		{
			if(!strcmp(one,definel[i].name))
			{
				b=false;
			}
		}
		if(!b)
		{
			char ch=getCh(file);
			while(ch!='\n')
			{
				ch=getCh(file);
			}
		}
		factor.push_back(b);
	}
	else if(!strcmp(t,"elif"))
	{
		bool b=true;
		if(factor[factor.size()-1]==false)
		{
			char* one=new char[1024];
			while(pre[lenpre]==' '||pre[lenpre]=='\t')
			{
				lenpre++;
			}
			lent=0;
			for(;lenpre<strlen(pre);lenpre++)
			{
				if((pre[lenpre]>='a'&&pre[lenpre]<='z')||(pre[lenpre]>='A'&&pre[lenpre]<='Z')||pre[lenpre]=='_')
				{
					one[lent++]=pre[lenpre];
					one[lent]='\0';
				}
				else
				{
					break;
				}
			}
			for(int i=0;i<definel.size();i++)
			{
				if(!strcmp(one,definel[i].name))
				{
					b=false;
				}
			}
		}
		if(b)
		{
			char ch=getCh(file);
			while(ch!='\n')
			{
				ch=getCh(file);
			}
		}
		else
		{
			factor.push_back(!b);
		}
	}
	else if(!strcmp(t,"else"))
	{
		if(factor[factor.size()-1]==true)
		{
			char ch=getCh(file);
			while(ch!='\n')
			{
				ch=getCh(file);
			}
		}
	}
	else if(!strcmp(t,"endif"))
	{
		factor.clear();
	}
	else if(!strcmp(t,"include"))
	{
		lenpre--;
		while(pre[lenpre]==' '||pre[lenpre]=='\t')
		{
			lenpre++;
		}
		if(pre[lenpre]=='<')
		{
			char include[1024];
			int leninclude=0;
			lenpre++;
			while(pre[lenpre]!='>')
			{
				include[leninclude++]=pre[lenpre];
				include[leninclude]='\0';
				lenpre++;
			}
			char buf[1024];
			sprintf(buf,"include/%s",include);
			preprocessor(buf);
		}
		else if(pre[lenpre]=='"')
		{
			char include[1024];
			int leninclude=0;
			lenpre++;
			while(pre[lenpre]!='"')
			{
				include[leninclude++]=pre[lenpre];
				include[leninclude]='\0';
				lenpre++;
			}
			char* filenamei=new char[1024];
			for(int i=0;i<strlen(filename);i++)
			{
				filenamei[i]=filename[i];
				filenamei[i+1]='\0';
			}
			for(int i=strlen(filenamei);i>=0;i--)
			{
				if(filenamei[i]=='/'||filenamei[i]=='\\')
				{
					break;
				}
				filenamei[i]='\0';
			}
			char buf[1024];
			sprintf(buf,"%s%s",filenamei,include);
			preprocessor(buf);
		}
	}
	else if(!strcmp(t,"error"))
	{
		printf("遇到#error，退出编译");
		exit(0);
	} 
}

vector<char> preprocessor(char* filename)
{
	FILE* file=fopen(filename,"r");
	if(!file)
	{
		printf("%s: 找不到文件",filename);
		exit(-1);
	}
	char ch;
	ch=getCh(file);
	while(ch!=EOF)
	{
		if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||ch=='_')
		{
			temp[templen++]=ch;
			temp[templen]='\0';
		}
		else
		{
			templen=0;
			int a=define_find();
			int i=a;
			while(a!=-1)
			{
				for(int k=0;k<strlen(temp);k++)
				{
					_temp[k]=temp[k];
					_temp[k+1]='\0';
				}
				for(int k=0;k<strlen(definel[a].val);k++)
				{
					temp[k]=definel[a].val[k];
					temp[k+1]='\0';
				} 
				i=a;
				a=define_find();
			}
			if(i!=-1)
			{
				for(int k=0;k<strlen(definel[i].val);k++)
				{
					filei.push_back(definel[i].val[k]);
				}
			}
			else
			{
				for(int k=0;k<strlen(temp);k++)
				{
					filei.push_back(temp[k]);
				}
			}
			temp[templen]='\0';
			if(ch=='"')
			{
				filei.push_back(ch);
				ch=getCh(file);
				while(ch!='"')
				{
					filei.push_back(ch);
					ch=getCh(file);
				}
				filei.push_back(ch);
				ch=getCh(file);
			}
		}
		if(ch=='/')
		{
			ch=getCh(file);
			if(ch=='*')
			{
				ch=getCh(file);
				while(true)
				{
					if(ch=='*')
					{
						ch=getCh(file);
						if(ch=='/')
						{
							break;
						}
					}
					else if(ch==EOF)
					{
						printf("注释未在代码结束前结束\n");
						exit(0);
					}
					ch=getCh(file);
				}
			}
			else if(ch=='/')
			{
				ch=getCh(file);
				while(ch!='\n'&&ch!=EOF)
				{
					ch=getCh(file);
				}
			}
			else
			{
				filei.push_back('/');
				filei.push_back(ch);
			}
		}
		else if(ch=='#')
		{
			int len=0;
			ch=getCh(file);
			while(ch!='\n'&&ch!=EOF)
			{
				pre[len++]=ch;
				pre[len]='\0';
				ch=getCh(file);
			}
			pretreatment(filename,file);
			ungetCh(ch,file);
		}
		else if(!((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||ch=='_'))
		{
			filei.push_back(ch);
		}
		ch=getCh(file);
	}
	return filei;
}
