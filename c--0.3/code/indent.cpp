#include<fstream>
using namespace std;

FILE* file;
int indent=0;

int main(int argc,char* argv[])
{
	file=fopen(argv[1],"r");
	if(!file)
	{
		printf("%s: 找不到文件\n",argv[1]);
		return 1;
	}
	char ch=getc(file);
	for(int i=0;i<indent;i++)
	{
		printf("\t");
	}
	while(ch!=EOF)
	{
		while(ch=='\t'||ch==' ')
		{
			ch=getc(file);
			if(ch!='\t'&&ch!=' ')
			{
				break;
			}
		}
		if(ch=='}')
		{
			indent--;
			for(int i=0;i<indent;i++)
			{
				printf("\t");
			}
		}
		printf("%c",ch);
		if(ch=='\n')
		{
			ch=getc(file);
			if(ch!=EOF&&ch!='}')
			{
				for(int i=0;i<indent;i++)
				{
					printf("\t");
				}
			}
			ungetc(ch,file);
		}
		else if(ch=='{')
		{
			indent++;
		}
		else if(ch=='"')
		{
			ch=getc(file);
			while(ch!='"'&&ch!=EOF)
			{
				printf("%c",ch);
				ch=getc(file);
			}
			ungetc(ch,file);
		}
		else if(ch=='\'')
		{
			ch=getc(file);
			while(ch!='\''&&ch!=EOF)
			{
				printf("%c",ch);
				ch=getc(file);
			}
			ungetc(ch,file);
		}
		ch=getc(file);
	}
}
