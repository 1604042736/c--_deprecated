#include "preprocessor.h"

struct Preprocessor* Preprocessor_New(FILE* file)
{
	struct Preprocessor* preprocessor = (struct Preprocessor*)malloc(sizeof(struct Preprocessor));
	if (preprocessor == NULL)
	{
		printf("��Ԥ�����������ڴ�ʧ��");
		exit(-1);
	}
	preprocessor->file = file;
	preprocessor->result = (struct StringObject*)StringObject_New();
	preprocessor->definedict = DictObject_New();
	preprocessor->defineflag = 0;
	preprocessor->blockstyle = INDENT;
	return preprocessor;
}

void Preprocessor_Preprocess(struct Preprocessor* preprocessor)
{
	preprocessor->ch = getc(preprocessor->file);
	while (preprocessor->ch != EOF)
	{
		if (preprocessor->ch == '#')
		{
			struct StringObject* precode = StringObject_New();
			preprocessor->ch = getc(preprocessor->file);
			while (preprocessor->ch != EOF && preprocessor->ch != '\n')
			{
				precode = StringObject_Add(precode, StringObject_NewWithChar(preprocessor->ch));
				preprocessor->ch = getc(preprocessor->file);
			}
			Preprocessor_Dealprecode(preprocessor,precode);
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
	int replace = 0;
	struct ListObject* list=StringObject_Split(precode, NULL);
	if (StringObject_Eq(list->item[0], StringObject_NewWithString("define")))	//�ٶ���,������
	{
		struct Object* name = list->item[1];
		struct Object* value = NULL;
		if (list->size >=3)
		{
			value = list->item[2];
		}
		DictObject_SetItem(preprocessor->definedict, name, value);
	}
	else if (StringObject_Eq(list->item[0], StringObject_NewWithString("ifdef")))
	{
		struct Object* name = list->item[1];
		if (DictObject_GetItem(preprocessor->definedict, name) != NULL)	//�ҵ���
		{
			preprocessor->defineflag=1;
		}
		else
		{
			replace = 1;
		}
	}
	else if (StringObject_Eq(list->item[0], StringObject_NewWithString("ifndef")))
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
	else if (StringObject_Eq(list->item[0], StringObject_NewWithString("elif")))
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
	else if (StringObject_Eq(list->item[0], StringObject_NewWithString("else")))
	{
		if (preprocessor->defineflag)
		{
			replace = 1;
		}
	}
	else if (StringObject_Eq(list->item[0], StringObject_NewWithString("endif")))
	{
		preprocessor->defineflag = 0;
	}
	else if (StringObject_Eq(list->item[0], StringObject_NewWithString("blockstyle")))
	{
		struct Object* name = list->item[1];
		if (StringObject_Eq(name, StringObject_NewWithString("braces")))
		{
			preprocessor->blockstyle = BRACES;
		}
		else if (StringObject_Eq(name, StringObject_NewWithString("indent")))
		{
			preprocessor->blockstyle = INDENT;
		}
		else if (StringObject_Eq(name, StringObject_NewWithString("beginend")))
		{
			preprocessor->blockstyle = BEGINEND;
		}
		else if (StringObject_Eq(name, StringObject_NewWithString("endname")))
		{
			preprocessor->blockstyle = ENDNAME;
		}
		else if (StringObject_Eq(name, StringObject_NewWithString("end")))
		{
			preprocessor->blockstyle = END;
		}
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
}