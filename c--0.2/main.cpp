#include "header-file/globals.h"
#include "header-file/Preprocessor.h"
#include "header-file/compiler.h"
#include "header-file/symbol.h"
#include "header-file/vmare.h"

void interactive()
{
	printf("\nc-- [Version 1.0.0.2]\nUsage: c-- filename[-i][-pi][-t][-pt][-a][-pa]\n");
	return;
	while(true)
	{
		ofstream fout("temp.c--");
		int large=0;
		do
		{
			cout<<"c-->\t";
			std::string str;
			getline(cin,str);
			if(str.length()!=0)
			{
				for(int i=0;i<str.length();i++)
				{
					if(str[i]=='{')
					{
						large++;
					}
					else if(str[i]=='}')
					{
						large--;
					}
				}
				fout<<str<<endl;
			}
		}while(large!=0);
		fout.close();
		vector<char> filei=preprocessor((char*)"temp.c--");
		TreeNode* syntaxtree=compiler((char*)"temp.c--",filei);
		Analyzer* analyzer=symbol_start(syntaxtree,(char*)"temp.c--");
		run(syntaxtree,analyzer,(char*)"temp.c--");
		cout<<endl;
	}
}

int main(int argc,char* argv[])
{
	if(argc==1)
	{
		interactive();
		return 0;
	}
	vector<char> filei=preprocessor(argv[1]);
	if(!strcmp(argv[argc-1],"-i"))
	{
		FILE* out=fopen(strcat(argv[1],".i"),"w");
		for(int i=0;i<filei.size();i++)
		{
			fprintf(out,"%c",filei[i]);
		}
		return 0;
	}
	else if(!strcmp(argv[argc-1],"-pi"))
	{
		for(int i=0;i<filei.size();i++)
		{
			printf("%c",filei[i]);
		}
		return 0;
	}
	TreeNode* syntaxtree=compiler(argv[1],filei);
	if(!strcmp(argv[argc-1],"-pt"))
	{
		printTree(syntaxtree,0);
		return 0;
	}
	else if(!strcmp(argv[argc-1],"-t"))
	{
		ofstream fout(strcat(argv[1],".tree"),ios::binary);
		fout.write((char*)(&syntaxtree),sizeof(syntaxtree));
		return 0;
	}
	Analyzer* analyzer=symbol_start(syntaxtree,argv[1]);
	if(!strcmp(argv[argc-1],"-pa"))
	{
		printSymTab(analyzer->symtab);
		return 0;
	}
	else if(!strcmp(argv[argc-1],"-a"))
	{
		ofstream fout(strcat(argv[1],".symbol"),ios::binary);
		fout.write((char*)(&analyzer),sizeof(analyzer));
		return 0;
	}
	return atoi(run(syntaxtree,analyzer,argv[1]));
}
