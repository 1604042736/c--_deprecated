#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define F_JOIN 1
#define F_WRITE 1<<1
#define F_SKIPSPACE 1<<2
#define F_READ 1<<3

#define CHECK_FLAG(a,b) (((a)&(b))==(b))

#define BOTHPRINT(a,b,...) fprintf(a,b,__VA_ARGS__);printf(b,__VA_ARGS__);

#define PRESKIP while (ch != '#')\
				{\
				if (ch == '"')\
				{\
					ch = fgetc(preprocessor->infile);\
					while (ch != '"')ch = fgetc(preprocessor->infile);\
				}\
				else if (ch == '\'')\
				{\
					ch = fgetc(preprocessor->infile);\
					while (ch != '\'')ch = fgetc(preprocessor->infile);\
				}\
				ch = fgetc(preprocessor->infile);\
				}\
				flag ^= F_READ;

struct DefineItem
{
	char* name;
	char* val;
};

struct DefineTable
{
	struct DefineItem** defines;
	int define_num;
	int defines_size;
};

//预处理器
struct Preprocessor
{
	FILE* infile;
	FILE* outfile;
	char* infilename;
	struct DefineTable* definetable;
	struct DefineItem* cur_defineitem;
};

enum State
{
	S_START,
	S_DIR, S_DEFINE, S_UNDEF, S_INCLUDE, S_IFDEF, S_ELIF, S_ELSE, S_IFUDEF, S_ENDIF,	//预处理指令
	S_DEFINEGOTNAME,	//define得到了name之后
	S_NAME, S_STRING, S_CHAR,
	S_INCLUDEDEFAULT,	//从默认目录包含
	S_INCLUDECURRENT,	//从用户目录包含
	S_COMMENTPRE,
	S_SINGLECOMMENT,	//单行注释
	S_MULTICOMMENTEND, S_MULTICOMMENT,	//多行注释
};

struct DefineTable* definetable_new()
{
	struct DefineTable* definetable = (struct DefineTable*)malloc(sizeof(struct DefineTable));
	definetable->define_num = 0;
	definetable->defines_size = 1024;
	definetable->defines = (struct DefineItem**)malloc(sizeof(struct DefineItem*) * definetable->defines_size);
	for (int i = 0; i < definetable->defines_size; i++)definetable->defines[i] = NULL;
	return definetable;
}

void definetable_adddefine(struct DefineTable* definetable, struct DefineItem* defineitem)
{
	definetable->defines[definetable->define_num++] = defineitem;
}

void definetable_removedefine(struct DefineTable* definetable, char* name)
{
	int i = 0;
	for (; i < definetable->define_num; i++)
	{
		if (!strcmp(definetable->defines[i]->name, name))
		{
			definetable->define_num--;
			for (; i < definetable->defines_size - 1; i++)
			{
				definetable->defines[i] = definetable->defines[i + 1];
			}
			break;
		}
	}
}

void definetable_print(struct DefineTable* definetable)
{
	for (int i = 0; i < definetable->define_num; i++)
	{
		printf("%s\t%s\n", definetable->defines[i]->name, definetable->defines[i]->val);
	}
}

struct Preprocessor* preprocessor_new(char* infilename, FILE* outfile)
{
	struct Preprocessor* preprocessor = (struct Preprocessor*)malloc(sizeof(struct Preprocessor));
	if (preprocessor == NULL)
	{
		printf("预处理器分配内存失败");
		exit(1);
	}
	preprocessor->infile = fopen(infilename, "r");
	if (preprocessor->infile == NULL)
	{
		printf("Cannot open file '%s': %s", infilename,strerror(errno));
		exit(1);
	}
	preprocessor->outfile = outfile;
	preprocessor->infilename = infilename;
	return preprocessor;
}

void preprocessor_start(struct Preprocessor* preprocessor)
{
	enum State state = S_START;
	char ch = fgetc(preprocessor->infile);
	char* s = (char*)malloc(1024 * sizeof(char));
	int slen = 0;
	int flag = F_JOIN | F_WRITE;
	int condition = 0;
	while (1)
	{
		flag = F_JOIN | F_WRITE | F_READ;
		switch (state)
		{
		case S_START:
			if (ch == '#')
			{
				state = S_DIR;
				slen = 0;
				flag ^= F_WRITE;
				flag |= F_SKIPSPACE;
				flag ^= F_JOIN;
			}
			else if (isalpha(ch))
			{
				state = S_NAME;
				slen = 0;
				flag ^= F_WRITE;
			}
			else if (ch == '"')
			{
				state = S_STRING;
				flag ^= F_WRITE;
			}
			else if (ch == '\'')
			{
				state = S_CHAR;
				flag ^= F_WRITE;
			}
			else if (ch == '/')
			{
				state = S_COMMENTPRE;
				flag ^= F_WRITE;
			}
			else if (ch == EOF)
			{
				goto end;
			}
			break;
		case S_DIR:
			flag ^= F_WRITE;
			if (!(isdigit(ch) || isalpha(ch) || ch == '_'))
			{
				if (!strcmp(s, "define"))
				{
					preprocessor->cur_defineitem = (struct DefineItem*)malloc(sizeof(struct DefineItem));
					preprocessor->cur_defineitem->name = "\0";
					preprocessor->cur_defineitem->val = "\0";
					state = S_DEFINE;
				}
				else if (!strcmp(s, "undef"))
				{
					state = S_UNDEF;
				}
				else if (!strcmp(s, "include"))
				{
					state = S_INCLUDE;
				}
				else if (!strcmp(s, "ifdef"))
				{
					state = S_IFDEF;
				}
				else if (!strcmp(s, "ifudef"))
				{
					state = S_IFUDEF;
				}
				else if (!strcmp(s, "elif"))
				{
					state = S_ELIF;
				}
				else if (!strcmp(s, "else"))
				{
					state = S_ELSE;
				}
				else if (!strcmp(s, "endif"))
				{
					state = S_ENDIF;
				}
				else
				{
					state = S_START;
					break;
				}
				flag ^= F_JOIN;
				s = (char*)malloc(1024 * sizeof(char));
				slen = 0;
				flag |= F_SKIPSPACE;
			}
			break;
		case S_DEFINE:
			flag ^= F_WRITE;
			if (!(isdigit(ch) || isalpha(ch) || ch == '_'))
			{
				preprocessor->cur_defineitem->name = s;
				s = (char*)malloc(1024 * sizeof(char));
				slen = 0;
				state = S_DEFINEGOTNAME;
				flag |= F_SKIPSPACE;
				flag ^= F_JOIN;
			}
			break;
		case S_DEFINEGOTNAME:
			flag ^= F_WRITE;
			if (ch == '\n' || ch == EOF)
			{
				preprocessor->cur_defineitem->val = s;
				s = (char*)malloc(1024 * sizeof(char));
				slen = 0;
				state = S_START;
				definetable_adddefine(preprocessor->definetable, preprocessor->cur_defineitem);
				preprocessor->cur_defineitem = NULL;
			}
			break;
		case S_UNDEF:
			flag ^= F_WRITE;
			if (!(isdigit(ch) || isalpha(ch) || ch == '_'))
			{
				definetable_removedefine(preprocessor->definetable, s);
				s = (char*)malloc(1024 * sizeof(char));
				slen = 0;
				state = S_START;
			}
			break;
		case S_NAME:
			flag ^= F_WRITE;
			if (!(isdigit(ch) || isalpha(ch) || ch == '_'))
			{
				for (int i = 0; i < preprocessor->definetable->define_num; i++)
				{
					if (!strcmp(preprocessor->definetable->defines[i]->name, s))
					{
						BOTHPRINT(preprocessor->outfile, "%s", preprocessor->definetable->defines[i]->val);
						goto match;
					}
				}
				flag |= F_WRITE;
				state = S_START;
				break;
			match:
				s = (char*)malloc(1024 * sizeof(char));
				slen = 0;
				state = S_START;
				break;
			}
			break;
		case S_STRING:
			flag ^= F_WRITE;
			if (ch == '"' || ch == EOF)
			{
				flag |= F_WRITE;
				state = S_START;
			}
			break;
		case S_CHAR:
			flag ^= F_WRITE;
			if (ch == '\'' || ch == EOF)
			{
				flag |= F_WRITE;
				state = S_START;
			}
			break;
		case S_INCLUDE:
			flag ^= F_WRITE;
			if (ch == '"')
			{
				state = S_INCLUDECURRENT;
				s = (char*)malloc(1024 * sizeof(char));
				slen = 0;
				flag ^= F_JOIN;
			}
			else if (ch == '<')
			{
				state = S_INCLUDEDEFAULT;
				s = (char*)malloc(1024 * sizeof(char));
				slen = 0;
				flag ^= F_JOIN;
			}
			else if (ch == EOF)
			{
				state = S_START;
			}
			break;
		case S_INCLUDECURRENT:
			flag ^= F_WRITE;
			if (ch == EOF)
			{
				state = S_START;
			}
			else if (ch == '"')
			{
				struct Preprocessor* pre = preprocessor_new(s, preprocessor->outfile);
				pre->definetable = preprocessor->definetable;
				BOTHPRINT(preprocessor->outfile, "#enterfile \"%s\"\n", pre->infilename);
				preprocessor_start(pre);
				s = (char*)malloc(1024 * sizeof(char));
				sprintf(s, "\n#backfile \"%s\"", preprocessor->infilename);
				slen = 0;
				state = S_START;
				flag |= F_WRITE;
				flag ^= F_JOIN;
			}
			break;
		case S_INCLUDEDEFAULT:
			flag ^= F_WRITE;
			if (ch == EOF)
			{
				state = S_START;
			}
			else if (ch == '>')
			{
				char filepath[1024];
				sprintf(filepath, "include/%s", s);
				struct Preprocessor* pre = preprocessor_new(filepath, preprocessor->outfile);
				pre->definetable = preprocessor->definetable;
				BOTHPRINT(preprocessor->outfile, "\n#enterfile %s\n", pre->infilename);
				preprocessor_start(pre);
				s = (char*)malloc(1024 * sizeof(char));
				sprintf(s, "\n#backfile %s\n", preprocessor->infilename);
				slen = 0;
				state = S_START;
				flag |= F_WRITE;
				flag ^= F_JOIN;
			}
			break;
		case S_COMMENTPRE:
			if (ch == '/')
			{
				state = S_SINGLECOMMENT;
				flag ^= F_WRITE;
			}
			else if (ch == '*')
			{
				state = S_MULTICOMMENT;
				flag ^= F_WRITE;
			}
			else
			{
				state = S_START;
			}
			break;
		case S_SINGLECOMMENT:
			flag ^= F_WRITE;
			if (ch == '\n' || ch == EOF)
			{
				state = S_START;
				flag ^= F_JOIN;
				slen = 0;
			}
			break;
		case S_MULTICOMMENT:
			flag ^= F_WRITE;
			if (ch == '*')
			{
				state = S_MULTICOMMENTEND;
			}
			else if (ch == EOF)
			{
				state = S_START;
			}
			break;
		case S_MULTICOMMENTEND:
			flag ^= F_WRITE;
			if (ch == '/' || ch == EOF)
			{
				state = S_START;
			}
			else
			{
				state = S_MULTICOMMENT;
			}
			break;
		case S_IFDEF:
			flag ^= F_WRITE;
			s = (char*)malloc(1024 * sizeof(char));
			slen = 0;
			while (isdigit(ch) || isalpha(ch) || ch == '_')
			{
				s[slen++] = ch;
				s[slen] = '\0';
				ch = fgetc(preprocessor->infile);
			}
			for (int i = 0; i < preprocessor->definetable->define_num; i++)
			{
				if (!strcmp(preprocessor->definetable->defines[i]->name, s))
				{
					condition = 1;
					break;
				}
			}
			if (condition == 0)
			{
				PRESKIP;
			}
			state = S_START;
			break;
		case S_IFUDEF:
			flag ^= F_WRITE;
			s = (char*)malloc(1024 * sizeof(char));
			slen = 0;
			while (isdigit(ch) || isalpha(ch) || ch == '_')
			{
				s[slen++] = ch;
				s[slen] = '\0';
				ch = fgetc(preprocessor->infile);
			}
			condition = 1;
			for (int i = 0; i < preprocessor->definetable->define_num; i++)
			{
				if (!strcmp(preprocessor->definetable->defines[i]->name, s))
				{
					condition = 0;
					PRESKIP;
					break;
				}
			}
			state = S_START;
			break;
		case S_ELIF:
			flag ^= F_WRITE;
			if (condition == 1)
			{
				PRESKIP;
				state = S_START;
				break;
			}
			s = (char*)malloc(1024 * sizeof(char));
			slen = 0;
			while (isdigit(ch) || isalpha(ch) || ch == '_')
			{
				s[slen++] = ch;
				s[slen] = '\0';
				ch = fgetc(preprocessor->infile);
			}
			for (int i = 0; i < preprocessor->definetable->define_num; i++)
			{
				if (!strcmp(preprocessor->definetable->defines[i]->name, s))
				{
					condition = 1;
					break;
				}
			}
			if (condition == 0)
			{
				PRESKIP;
			}
			state = S_START;
			break;
		case S_ELSE:
			if (condition == 1)
			{
				flag ^= F_WRITE;
				PRESKIP;
			}
			state = S_START;
			break;
		case S_ENDIF:
			flag ^= F_WRITE;
			condition = 0;
			state = S_START;
			break;
		default:
			break;
		}
		if (CHECK_FLAG(flag, F_JOIN))
		{
			if (ch != EOF)
			{
				s[slen++] = ch;
				s[slen] = '\0';
			}
		}
		if (CHECK_FLAG(flag, F_WRITE))
		{
			BOTHPRINT(preprocessor->outfile, "%s", s);
			slen = 0;
		}
		if (!CHECK_FLAG(flag, F_READ))continue;
		ch = fgetc(preprocessor->infile);
		if (CHECK_FLAG(flag, F_SKIPSPACE))
		{
			while (isspace(ch))ch = fgetc(preprocessor->infile);
			flag ^= F_SKIPSPACE;
		}
		continue;
	end:
		break;
	}
}

int main(int argc, char* argv[])
{
#ifdef _DEBUG
	char* infilename = "test.txt";
	char* outfilename = "test.i";
#else
	if (argc <= 2)
	{
		printf("Too few args");
		exit(2);
	}
	char* infilename = argv[1];
	char* outfilename = argv[2];
#endif
	struct Preprocessor* preprocessor = preprocessor_new(infilename, fopen(outfilename, "w"));
	preprocessor->definetable = definetable_new();
	preprocessor_start(preprocessor);
	printf("\n");
	definetable_print(preprocessor->definetable);
}