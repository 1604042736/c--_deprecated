#include "Include/preprocessor.h"
#include "Include/dynstring.h"
#include "Include/globals.h"

char* preprocessor(FILE* file)
{
	if (NULL == definelist)
	{
		definelist = definelist_init();
	}
	struct DynString* result = dynstring_init();
	ch = getc(file);
	while (ch != EOF)
	{
		int define_index = search_define(ch);
		if (ch == '#')
		{
			struct DynString* precode = dynstring_init();
			ch = getc(file);
			while (ch != '\n' && ch != EOF)
			{
				dynstring_add_chr(precode, ch);
				ch = getc(file);
			}
			char* preresult=dealprecode(result,file,precode);
			if (NULL != preresult)
			{
				for (int i = 0; i < strlen(preresult); i++)
				{
					dynstring_add_chr(result, preresult[i]);
				}
			}
		}
		else if (define_index != -1)	//匹配宏
		{
			struct DynString* define = dynstring_init();
			for (int i = 0; i < strlen(definelist->nowdefinelist->define_name[define_index]); i++)
			{
				dynstring_add_chr(define, ch);
				ch = getc(file);
			}
			if (!strcmp(define->str, definelist->nowdefinelist->define_name[define_index])&&!((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||ch=='_'))	//匹配上了
			{
				/*将宏的内容替换进去*/
				for (int i = 0; i < strlen(definelist->nowdefinelist->define_content[define_index]); i++)
				{
					dynstring_add_chr(result, definelist->nowdefinelist->define_content[define_index][i]);
				}
			}
			else
			{
				for (int i = 0; i < strlen(define->str); i++)
				{
					dynstring_add_chr(result, define->str[i]);
				}
			}
		}
		else
		{
			dynstring_add_chr(result, ch);
			ch = getc(file);
		}
	}
	return result->str;
}

char* dealprecode(struct DynString* code,FILE* file,struct DynString* precode)
{
	int pos = 0;
	int replace = 0;	//是否替换
	char* arg1 = getnextprediv(precode, &pos);
	if (!strcmp(arg1, "define"))
	{
		char* arg2 = getnextprediv(precode, &pos);
		struct DynString* arg3 = dynstring_init();
		while (pos < strlen(precode->str) && (precode->str[pos] == '\n' || precode->str[pos] == '\t' || precode->str[pos] == ' '))
		{
			pos++;
		}
		while (pos < strlen(precode->str) && precode->str[pos] != '\n')
		{
			dynstring_add_chr(arg3, precode->str[pos]);
			pos++;
		}
		definelist_add(arg2, arg3->str);
	}
	else if (!strcmp(arg1, "include"))
	{
		while (pos < strlen(precode->str) && (precode->str[pos] == '\n' || precode->str[pos] == '\t' || precode->str[pos] == ' '))
		{
			pos++;
		}
		char* filename=NULL;
		if (precode->str[pos] == '<')	//从默认目录包含文件
		{
			pos++;
			while (pos < strlen(precode->str) && (precode->str[pos] == '\n' || precode->str[pos] == '\t' || precode->str[pos] == ' '))
			{
				pos++;
			}
			struct DynString* file_name=dynstring_init();
			while (pos < strlen(precode->str) && precode->str[pos] != '\n' && precode->str[pos] != '>')
			{
				dynstring_add_chr(file_name, precode->str[pos]);
				pos++;
			}
			char* path=(char*)malloc(sizeof(char)*strlen(include_path));
			strcpy(path, include_path);
			strcat(path, file_name->str);
			filename = path;
		}
		else if (precode->str[pos] == '"')	//从当前目录包含
		{
			pos++;
			while (pos < strlen(precode->str) && (precode->str[pos] == '\n' || precode->str[pos] == '\t' || precode->str[pos] == ' '))
			{
				pos++;
			}
			struct DynString* file_name = dynstring_init();
			while (pos < strlen(precode->str) && precode->str[pos] != '\n' && precode->str[pos] != '"')
			{
				dynstring_add_chr(file_name, precode->str[pos]);
				pos++;
			}
			filename = file_name->str;
		}
		if (filename != NULL)
		{
			FILE* f = fopen(filename, "r");
			if (NULL == f)
			{
				printf("无法打开文件%s", filename);
				exit(-1);
			}
			int i = strlen(code->str) - 1;
			/*回到预处理指令所在行的行首*/
			while (i >= 0 && code->str[i] != '\n')
			{
				i--;
			}
			i++;
			/*获取预处理器前的缩进*/
			struct DynString* indent = dynstring_init();
			while (i < strlen(code->str) && (code->str[i] == ' ' || code->str[i] == '\t'))
			{
				dynstring_add_chr(indent, code->str[i]);
				i++;
			}
			/*给文件每一行加上缩进*/
			char* _pre_file = preprocessor(f);
			struct DynString* pre_file = dynstring_init();
			for (i = 0; i < strlen(_pre_file); i++)
			{
				dynstring_add_chr(pre_file, _pre_file[i]);
				if (_pre_file[i] == '\n')
				{
					for (int j = 0; j < strlen(indent->str); j++)
					{
						dynstring_add_chr(pre_file, indent->str[j]);
					}
				}
			}
			fclose(f);
			return pre_file->str;
		}
	}
	else if (!strcmp(arg1, "undef"))
	{
		char* arg2 = getnextprediv(precode, &pos);
		definelist_del(arg2);
	}
	else if (!strcmp(arg1, "ifdef"))
	{
		char* arg2 = getnextprediv(precode, &pos);
		if (search_define2(arg2) != -1)
		{
			defineflag = 1;
		}
		else
		{
			replace = 1;
		}
	}
	else if (!strcmp(arg1, "ifndef"))
	{
		char* arg2 = getnextprediv(precode, &pos);
		if (search_define2(arg2) == -1)
		{
			defineflag = 1;
		}
		else
		{
			replace = 1;
		}
	}
	else if (!strcmp(arg1, "elif"))
	{
		char* arg2 = getnextprediv(precode, &pos);
		if (!defineflag && search_define2(arg2) != -1)
		{
			defineflag = 1;
		}
		else
		{
			replace = 1;
		}
	}
	else if (!strcmp(arg1, "else"))
	{
		if (defineflag)
		{
			replace = 1;
		}
	}
	else if (!strcmp(arg1, "endif"))
	{
		defineflag = 0;
	}
	else if (!strcmp(arg1, "syntax"))
	{
		char* arg2 = getnextprediv(precode, &pos);
		if (!strcmp(arg2, "blockstyle"))
		{
			char* arg3 = getnextprediv(precode, &pos);
			if (!strcmp(arg3, "indent"))
			{
				blockstyle = INDENT;
			}
			else if (!strcmp(arg3, "beginend"))
			{
				blockstyle = BEGINEND;
			}
			else if (!strcmp(arg3, "braces"))
			{
				blockstyle = BRACES;
			}
		}
	}
	if (replace)
	{
		while (ch != EOF && ch != '#')
		{
			ch = getc(file);
			/*判断是否是条件预处理指令*/
			if (ch == '#')
			{
				struct DynString* pc = dynstring_init();
				ch = getc(file);
				while (ch != EOF && ch != ' ' && ch != '\n' && ch != '\t')
				{
					dynstring_add_chr(pc, ch);
					ch = getc(file);
				}
				/*如果是*/
				if (!strcmp(pc->str, "ifdef") || !strcmp(pc->str, "ifndef") || !strcmp(pc->str, "elif") || !strcmp(pc->str, "else") || !strcmp(pc->str, "endif"))
				{
					ungetc(ch, file);
					for (int i = strlen(pc->str)-1; i >=0; i--)
					{
						ch = pc->str[i];
						ungetc(ch, file);
					}
					ch = '#';
				}
			}
		}
	}
	return NULL;
}

char* getnextprediv(struct DynString* precode, int* pos)
{
	while ((*pos) < strlen(precode->str) && (precode->str[*pos] == '\n' || precode->str[*pos] == '\t' || precode->str[*pos] == ' '))
	{
		(*pos)++;
	}
	struct DynString* result = dynstring_init();
	dynstring_add_chr(result, precode->str[*pos]);
	(*pos)++;
	while ((*pos) < strlen(precode->str) && !(precode->str[*pos] == '\n' || precode->str[*pos] == '\t' || precode->str[*pos] == ' '))
	{
		dynstring_add_chr(result, precode->str[*pos]);
		(*pos)++;
	}
	return result->str;
}

/*查找一个字符是否在宏里出现过,
如果有,那么该字符与后面组成的字符串可能是一个宏*/
int search_define(char c)
{
	struct _DefineList* p = definelist->alldefinelist;
	while (p != NULL)
	{
		for (int i = 0; i < MAX_DEFINE_LEN; i++)
		{
			if (p->define_name[i]!=NULL&&p->define_name[i][0] == c)
			{
				return i;
			}
		}
		p = p->next;
	}
	return -1;
}

int search_define2(char* name)
{
	struct _DefineList* p = definelist->alldefinelist;
	while (p != NULL)
	{
		for (int i = 0; i < MAX_DEFINE_LEN; i++)
		{
			if (p->define_name[i]!=NULL&&!strcmp(p->define_name[i], name))
			{
				return i;
			}
		}
		p = p->next;
	}
	return -1;
}

struct DefineList* definelist_init()
{
	struct DefineList* dfl = (struct DefineList*)malloc(sizeof(struct DefineList));
	if (NULL == dfl)
	{
		printf("宏列表(DefineList)初始化失败");
		exit(-1);
	}
	dfl->alldefinelist = _definelist_init();
	dfl->nowdefinelist = dfl->alldefinelist;
	return dfl;
}

struct _DefineList* _definelist_init()
{
	struct _DefineList* _dfl = (struct _DefineList*)malloc(sizeof(struct _DefineList));
	if (NULL == _dfl)
	{
		printf("宏列表(_DefineList)初始化失败");
		exit(-1);
	}
	for (int i = 0; i < MAX_DEFINE_LEN; i++)
	{
		_dfl->define_name[i] = NULL;
		_dfl->define_content[i] = NULL;
	}
	_dfl->define_len = 0;
	_dfl->next = NULL;
	return _dfl;
}

/*添加一个宏*/
void definelist_add(char* name, char* content)
{
	int pos = definelist->nowdefinelist->define_len;
	definelist->nowdefinelist->define_name[pos] = name;
	definelist->nowdefinelist->define_content[pos] = content;
	definelist_findempty();
}

/*找到宏列表中空的位置*/
void definelist_findempty()
{
	struct _DefineList* p = definelist->alldefinelist;
	while (p != NULL)
	{
		for (int i = 0; i < MAX_DEFINE_LEN; i++)
		{
			if (p->define_name[i] == NULL && p->define_content[i] == NULL)
			{
				p->define_len = i;
				definelist->nowdefinelist = p;
				return;
			}
		}
		if (p->next == NULL)
		{
			break;
		}
		p = p->next;
	}
	/*找不到就再加入一个*/
	struct _DefineList* newdfl = _definelist_init();
	definelist->nowdefinelist = newdfl;
	p->next = newdfl;
}

void definelist_del(char* name)
{
	struct _DefineList* p = definelist->alldefinelist;
	while (p != NULL)
	{
		for (int i = 0; i < MAX_DEFINE_LEN; i++)
		{
			if (p->define_name[i] != NULL && !strcmp(p->define_name[i] , name))
			{
				p->define_name[i] = NULL;
				p->define_content[i] = NULL;
				definelist_findempty();
				return;
			}
		}
		p = p->next;
	}
}