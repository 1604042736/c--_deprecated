#include "preprocessor.h"

struct Preprocessor* Preprocessor_New(FILE* file, char* filename)
{
	struct Preprocessor* preprocessor = (struct Preprocessor*)malloc(sizeof(struct Preprocessor));
	if (preprocessor == NULL)
	{
		printf("对预处理器分配内存失败");
		exit(-1);
	}
	preprocessor->file = file;
	preprocessor->filename = StringObject_NewWithString(filename);
	preprocessor->result = (struct StringObject*)StringObject_New();
	preprocessor->definedict = DictObject_New();
	preprocessor->defineflag = 0;
	return preprocessor;
}

void Preprocessor_Preprocess(struct Preprocessor* preprocessor)
{
	preprocessor->ch = getc(preprocessor->file);
	while (preprocessor->ch != EOF)
	{
		if (preprocessor->ch == '\'')
		{
			preprocessor->result = StringObject_Add(preprocessor->result, StringObject_NewWithChar(preprocessor->ch));
			preprocessor->ch = getc(preprocessor->file);
			while (preprocessor->ch != EOF && preprocessor->ch != '\'')
			{
				preprocessor->result = StringObject_Add(preprocessor->result, StringObject_NewWithChar(preprocessor->ch));
				preprocessor->ch = getc(preprocessor->file);
			}
			preprocessor->result = StringObject_Add(preprocessor->result, StringObject_NewWithChar(preprocessor->ch));
			preprocessor->ch = getc(preprocessor->file);
		}
		else if (preprocessor->ch == '"')
		{
			preprocessor->result = StringObject_Add(preprocessor->result, StringObject_NewWithChar(preprocessor->ch));
			preprocessor->ch = getc(preprocessor->file);
			while (preprocessor->ch != EOF && preprocessor->ch != '"')
			{
				preprocessor->result = StringObject_Add(preprocessor->result, StringObject_NewWithChar(preprocessor->ch));
				preprocessor->ch = getc(preprocessor->file);
			}
			preprocessor->result = StringObject_Add(preprocessor->result, StringObject_NewWithChar(preprocessor->ch));
			preprocessor->ch = getc(preprocessor->file);
		}
		else if (preprocessor->ch == '#')
		{
			struct StringObject* precode = StringObject_New();
			preprocessor->ch = getc(preprocessor->file);
			while (preprocessor->ch != EOF && preprocessor->ch != '\n')
			{
				precode = StringObject_Add(precode, StringObject_NewWithChar(preprocessor->ch));
				preprocessor->ch = getc(preprocessor->file);
			}
			struct StringObject* preresult=Preprocessor_Dealprecode(preprocessor,precode);
			if (preresult != NULL)
			{
				preprocessor->result = StringObject_Add(preprocessor->result, preresult);
			}
		}
		else if (isName(preprocessor->ch))
		{
			struct StringObject* name = StringObject_New();
			while (preprocessor->ch != EOF && isName(preprocessor->ch))
			{
				name = StringObject_Add(name, StringObject_NewWithChar(preprocessor->ch));
				preprocessor->ch = getc(preprocessor->file);
			}
			struct Object* value = DictObject_GetItem(preprocessor->definedict, name);
			if (value != NULL)
			{
				preprocessor->result = StringObject_Add(preprocessor->result, value);
			}
			else
			{
				preprocessor->result = StringObject_Add(preprocessor->result, name);
			}
		}
		else
		{
			preprocessor->result = StringObject_Add(preprocessor->result, StringObject_NewWithChar(preprocessor->ch));
			preprocessor->ch = getc(preprocessor->file);
		}
	}
}

struct StringObject* Preprocessor_Dealprecode(struct Preprocessor* preprocessor,struct StringObject* precode)
{
#define CHECKINS(NAME) IntObject_toInt(StringObject_Eq(list->item[0], StringObject_NewWithString(NAME)))
	int replace = 0;
	struct ListObject* list=StringObject_Split(precode, NULL);
	if (CHECKINS("define"))	//速度慢,但方便
	{
		struct Object* name = list->item[1];
		struct Object* value = NULL;
		if (list->size >=3)
		{
			value = list->item[2];
		}
		DictObject_SetItem(preprocessor->definedict, name, value);
	}
	else if (CHECKINS("undef"))
	{
		struct Object* name = list->item[1];
		DictObject_DelItem(preprocessor->definedict, name);
	}
	else if (CHECKINS("ifdef"))
	{
		struct Object* name = list->item[1];
		if (DictObject_GetItem(preprocessor->definedict, name) != NULL)	//找到了
		{
			preprocessor->defineflag=1;
		}
		else
		{
			replace = 1;
		}
	}
	else if (CHECKINS("ifndef"))
	{
		struct Object* name = list->item[1];
		if (DictObject_GetItem(preprocessor->definedict, name) == NULL)
		{
			preprocessor->defineflag = 1;
		}
		else
		{
			replace = 1;
		}
	}
	else if (CHECKINS("elif"))
	{
		struct Object* name = list->item[1];
		if (!preprocessor->defineflag&&DictObject_GetItem(preprocessor->definedict, name) != NULL)
		{
			preprocessor->defineflag = 1;
		}
		else
		{
			replace = 1;
		}
	}
	else if (CHECKINS("else"))
	{
		if (preprocessor->defineflag)
		{
			replace = 1;
		}
	}
	else if (CHECKINS("endif"))
	{
		preprocessor->defineflag = 0;
	}
	else if (CHECKINS("include"))
	{
		struct Object* arg = list->item[1];
		char* filename = ((struct StringObject*)arg)->string;
		FILE* f = fopen(filename, "r");
		if (f == NULL)
		{
			arg = StringObject_Add(conststr_includepath, arg);
			char* filename = ((struct StringObject*)arg)->string;
			f = fopen(filename, "r");
		}
		struct Preprocessor* pre = Preprocessor_New(f,filename);
		Preprocessor_Preprocess(pre);

		int i = preprocessor->result->size - 1;
		/*回到预处理指令所在行的行首*/
		while (i >= 0 && preprocessor->result->string[i] != '\n')
		{
			i--;
		}
		i++;
		/*获取预处理器前的缩进*/
		struct StringObject* indent = StringObject_New();
		while (i < preprocessor->result->size && (preprocessor->result->string[i] == ' ' || preprocessor->result->string[i] == '\t'))
		{
			indent = StringObject_Add(indent, StringObject_NewWithChar(preprocessor->result->string[i]));
			i++;
		}
		struct StringObject* pre_file = StringObject_New();
		for (i = 0; i < pre->result->size; i++)
		{
			pre_file = StringObject_Add(pre_file, StringObject_NewWithChar(pre->result->string[i]));
			if (pre->result->string[i] == '\n')
			{
				pre_file = StringObject_Add(pre_file, indent);
			}
		}
		fclose(f);
		return pre_file;
	}
	if (replace)
	{
		int level = 0;
		while (preprocessor->ch != EOF && preprocessor->ch != '#')
		{
			preprocessor->ch = getc(preprocessor->file);
			if (preprocessor->ch == '#')
			{
				struct StringObject* pc = StringObject_New();
				preprocessor->ch = getc(preprocessor->file);
				while (preprocessor->ch != EOF && isWhite(preprocessor->ch))preprocessor->ch = getc(preprocessor->file);
				while (preprocessor->ch != EOF && !isWhite(preprocessor->ch))
				{
					pc = StringObject_Add(pc, StringObject_NewWithChar(preprocessor->ch));
					preprocessor->ch = getc(preprocessor->file);
				}
				if (StringObject_Eq(pc, StringObject_NewWithString("ifdef")) ||
					StringObject_Eq(pc, StringObject_NewWithString("ifndef")))
				{
					level++;
				}
				else if (StringObject_Eq(pc, StringObject_NewWithString("endif")))
				{
					level--;
				}
				else if (!level&&(StringObject_Eq(pc, StringObject_NewWithString("elif")) ||
						StringObject_Eq(pc, StringObject_NewWithString("else"))))
				{
					ungetc(preprocessor->ch, preprocessor->file);
					for (int i = pc->size - 1; i >= 0; i--)
					{
						preprocessor->ch = pc->string[i];
						ungetc(preprocessor->ch, preprocessor->file);
					}
					preprocessor->ch = '#';
				}
			}
		}
	}
	return NULL;
#undef CHECKINS
}