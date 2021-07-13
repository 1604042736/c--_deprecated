#include "Include/dynstring.h"

struct DynString* dynstring_init()
{
	struct DynString* string = (struct DynString*)malloc(sizeof(struct DynString));
	if (NULL == string)
	{
		printf("�ַ�����ʼ��ʧ��");
		exit(-1);
	}
	string->str = "\0";
	return string;
}

void dynstring_add_chr(struct DynString* self, char other)
{
	/*��ԭ�ַ�������*/
	char* tmp = (char*)malloc(sizeof(char)*(strlen(self->str)));
	if (0 == tmp)
	{
		printf("tmpΪ0");
		exit(-1);
	}
	strcpy(tmp, self->str);
	self->str = (char*)malloc(sizeof(char)*(strlen(tmp)+2));	//���³�ʼ���ַ���
	if (0 == self->str)
	{
		printf("self->strΪ0");
		exit(-1);
	}
	/*��ԭ�ַ����뱻�ӵ��ַ�ƴ�ӵ��³�ʼ�����ַ�����*/
	int i;
	for (i=0; i < strlen(tmp); i++)
	{
		self->str[i] = tmp[i];
		self->str[i + 1] = '\0';
	}
	self->str[i] = other;
	self->str[i + 1] = '\0';
}