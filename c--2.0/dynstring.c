#include "Include/dynstring.h"

struct DynString* dynstring_init()
{
	struct DynString* string = (struct DynString*)malloc(sizeof(struct DynString));
	if (NULL == string)
	{
		printf("字符串初始化失败");
		exit(-1);
	}
	string->str = "\0";
	return string;
}

void dynstring_add_chr(struct DynString* self, char other)
{
	/*将原字符串保存*/
	char* tmp = (char*)malloc(sizeof(char)*(strlen(self->str)));
	if (0 == tmp)
	{
		printf("tmp为0");
		exit(-1);
	}
	strcpy(tmp, self->str);
	self->str = (char*)malloc(sizeof(char)*(strlen(tmp)+2));	//重新初始化字符串
	if (0 == self->str)
	{
		printf("self->str为0");
		exit(-1);
	}
	/*将原字符串与被加的字符拼接到新初始化的字符串中*/
	int i;
	for (i=0; i < strlen(tmp); i++)
	{
		self->str[i] = tmp[i];
		self->str[i + 1] = '\0';
	}
	self->str[i] = other;
	self->str[i + 1] = '\0';
}