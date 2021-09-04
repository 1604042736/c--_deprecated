#include<fstream>
#include<windows.h>
#include<iostream>
using namespace std;

#define KEYLEN 13 

FILE* file;
int token,lineno=1;
char ch;

std::string key[]={"int","void","struct","print","input","if","for","do","while","break","continue","else","return","string"};

void ColorChoose(const char* color)
{
	if(color=="sky-blue")
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_GREEN|FOREGROUND_BLUE);
	}
	else if(color=="green")
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_GREEN);
	}
	else if(color=="yellow")
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN);
	}
	else if(color=="red")
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED);
	}
	else if(color=="purple")
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_BLUE);
	}
	else if(color=="white")
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN);
	}
	else if(color=="dark-blue")
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_BLUE);
	}
}

bool keys(std::string str)
{
	for(int i=0;i<KEYLEN;i++)
	{
		if(str==key[i])
		{
			return true;
		}
	}
	return false;
}

int gettoken()
{
	if(ch=='\n'||ch=='\t'||ch==' ')
	{
		ColorChoose("white");
		printf("%c",ch);
		if(ch=='\n')
		{
			ch=getc(file);
			if(ch!=EOF)
			{
				lineno++;
				printf("%d\t|",lineno);
			}
			ungetc(ch,file);
		}
	}
	else if(ch=='{'||ch=='}'||ch=='['||ch==']'||ch=='('||ch==')'||ch=='+'||ch=='-'||ch=='*'||ch=='%'||ch==','||ch==';'||ch==':')
	{
		ColorChoose("red");
		printf("%c",ch);
	}
	else if(isdigit(ch))
	{
		ColorChoose("yellow");
		printf("%c",ch);
	}
	else if(ch=='#')
	{
		ColorChoose("green");
		printf("%c",ch);
		ch=getc(file);
		while(ch!='\n'&&ch!=EOF)
		{
			printf("%c",ch);
			ch=getc(file);
		}
		ungetc(ch,file);
	}
	else if(ch=='/')
	{
		ch=getc(file);
		if(ch=='/')
		{
			ch=getc(file);
			ColorChoose("dark-blue");
			printf("//%c",ch);
			ch=getc(file);
			while(ch!='\n'&&ch!=EOF)
			{
				printf("%c",ch);
				ch=getc(file);
			}
			ungetc(ch,file);
		}
		else if(ch=='*')
		{
			ColorChoose("dark-blue");
			printf("/*",ch);
			ch=getc(file);
			while(true)
			{
				if(ch=='*')
				{
					ch=getc(file);
					if(ch=='/')
					{
						printf("*/");
						break;
					}
					printf("*");
				}
				else if(ch==EOF)
				{
					break;
				}
				printf("%c",ch);
				if(ch=='\n')
				{
					ch=getc(file);
					if(ch!=EOF)
					{
						ColorChoose("white");
						lineno++;
						printf("%d\t|",lineno);
						ColorChoose("dark-blue");
					}
					ungetc(ch,file);
				}
				ch=getc(file);
			}
		}
		else
		{
			ColorChoose("red");
			printf("/");
			ungetc(ch,file);
		}
	}
	else if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||ch=='_')
	{
		std::string str;
		str+=ch;
		ch=getc(file);
		while((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||ch=='_')
		{
			str+=ch;
			ch=getc(file);
		}
		if(keys(str))
		{
			ColorChoose("sky-blue");
			cout<<str;
		}
		else
		{
			ColorChoose("white");
			cout<<str;
		}
		ungetc(ch,file);
	}
	else if(ch=='=')
	{
		ColorChoose("red");
		ch=getc(file);
		if(ch=='=')
		{
			printf("==");
		}
		else
		{
			ungetc(ch,file);
			printf("=");
		}
	}
	else if(ch=='!')
	{
		ColorChoose("red");
		ch=getc(file);
		if(ch=='=')
		{
			printf("!=");
		}
		else
		{
			ungetc(ch,file);
			printf("!");
		}
	}
	else if(ch=='>')
	{
		ColorChoose("red");
		ch=getc(file);
		if(ch=='=')
		{
			printf(">=");
		}
		else
		{
			ungetc(ch,file);
			printf(">");
		}
	}
	else if(ch=='<')
	{
		ColorChoose("red");
		ch=getc(file);
		if(ch=='=')
		{
			printf("<=");
		}
		else
		{
			ungetc(ch,file);
			printf("<");
		}
	}
	else if(ch=='|')
	{
		ColorChoose("red");
		ch=getc(file);
		if(ch=='|')
		{
			printf("||");
		}
		else
		{
			ungetc(ch,file);
			printf("|");
		}
	}
	else if(ch=='&')
	{
		ColorChoose("red");
		ch=getc(file);
		if(ch=='&')
		{
			printf("&&");
		}
		else
		{
			ungetc(ch,file);
			printf("&");
		}
	}
	else if(ch=='"')
	{
		ColorChoose("purple");
		printf("\"");
		ch=getc(file);
		while(ch!='"'&&ch!=EOF)
		{
			if(ch=='\\')
			{
				ch=getc(file);
				printf("\\%c",ch);
				ch=getc(file);
			}
			else
			{
				printf("%c",ch);
				ch=getc(file);
			}
		}
		printf("\"");
	}
	else if(ch=='\'')
	{
		ColorChoose("purple");
		printf("'");
		ch=getc(file);
		while(ch!='\''&&ch!=EOF)
		{
			if(ch=='\\')
			{
				ch=getc(file);
				printf("\\%c",ch);
				ch=getc(file);
			}
			else
			{
				printf("%c",ch);
				ch=getc(file);
			}
		}
		printf("'");
	}
	ch=getc(file);
	return ch;
}

int main(int argc,char* argv[])
{
	file=fopen(argv[1],"r");
	if(!file)
	{
		printf("%s: 找不到文件\n",argv[1]);
		return 1;
	}
	ch=getc(file);
	printf("%d\t|",lineno);
	token=gettoken();
	while(token!=EOF)
	{
		token=gettoken();
	}
}
