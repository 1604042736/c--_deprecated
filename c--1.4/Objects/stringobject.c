#include "stringobject.h"
#include "listobject.h"

struct Object* StringObject_New()
{
	struct StringObject* strobj = Memory_Malloc(memory,"string");
	strobj->objattr = &StringObjectAttribute;
	strobj->string = "\0";
	strobj->size = 0;
	strobj->objattr->attr = stringobjattr;
	strobj->refcount = DEFAULTREFCOUNT;
	return (struct Object*)strobj;
}

struct Object* StringObject_NewWithString(char* str)
{
	struct StringObject* strobj = (struct StringObject*)StringObject_New();
	strobj->size = strlen(str);
	strobj->string = (char*)malloc(sizeof(char) *( strobj->size+1));
	strcpy(strobj->string, str);
	strobj->string[strobj->size] = '\0';
	return (struct Object*)strobj;
}

struct Object* StringObject_NewWithChar(char ch)
{
	struct StringObject* strobj = (struct StringObject*)StringObject_New();
	strobj->size = 1;
	strobj->string = (char*)malloc(sizeof(char) * (strobj->size+1));
	strobj->string[0] = ch;
	strobj->string[1] = '\0';
	return (struct Object*)strobj;
}

void StringObject_Print(struct Object* obj)
{
	struct StringObject* strobj = (struct StringObject*)obj;
	printf("\"");
	for (int i = 0; i < strobj->size; i++)
	{
		switch (strobj->string[i])
		{
		case '\n':
			printf("\\n");
			break;
		case '\b':
			printf("\\b");
			break;
		case '\t':
			printf("\\t");
			break;
		case '\r':
			printf("\\r");
			break;
		default:
			printf("%c", strobj->string[i]);
			break;
		}
	}
	printf("\"");
}

struct Object* StringObject_Add(struct Object* self, struct Object* other)
{
	struct StringObject* selfstring = (struct StringObject*)self;
	struct StringObject* otherstring = (struct StringObject*)other;
	int size = selfstring->size + otherstring->size;
	char* result = (char*)malloc(sizeof(char) * (size+1));
	strcpy(result, selfstring->string);
	strcat(result, otherstring->string);
	result[size] = '\0';
	return StringObject_NewWithString(result);
}

struct Object* StringObject_Eq(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "string"))
	{
		return IntObject_NewWithValue(0);
	}
	struct StringObject* selfstring = (struct StringObject*)self;
	struct StringObject* otherstring = (struct StringObject*)other;
	return IntObject_NewWithValue(!strcmp(selfstring->string, otherstring->string));
}

struct ListObject* StringObject_Split(struct Object* self, struct Object* split)
{
	struct StringObject* selfstring = (struct StringObject*)self;
	struct ListObject* result = ListObject_New();
	if (split == NULL)
	{
		int i = 0;
		while (i < selfstring->size&&isWhite(selfstring->string[i]))i++;
		struct StringObject* t = StringObject_New();
		while (i < selfstring->size)
		{
			if (isWhite(selfstring->string[i]))
			{
				ListObject_InsertItem(result, result->size, t);
				while (i < selfstring->size&&isWhite(selfstring->string[i]))i++;
				t = StringObject_New();
			}
			t = StringObject_Add(t, StringObject_NewWithChar(selfstring->string[i]));
			i++;
		}
		ListObject_InsertItem(result, result->size, t);
	}
	return result;
}

int StringObject_Bool(struct Object* self)
{
	struct StringObject* selfstring = (struct StringObject*)self;
	return selfstring->size;
}

struct Object* StringObject_Add2(struct Object* arg)
{
	struct ListObject* args = (struct ListObject*)arg;
	return StringObject_Add(args->item[0], args->item[1]);
}

struct Object* StringObject_Eq2(struct Object* arg)
{
	struct ListObject* args = (struct ListObject*)arg;
	return StringObject_Eq(args->item[0], args->item[1]);
}

struct Object* StringObject_Split2(struct Object* arg)
{
	struct ListObject* args = (struct ListObject*)arg;
	return StringObject_Split(args->item[0], args->item[1]);
}

struct Object* StringObject_Format(struct Object* self, struct Object* format)
{
	struct ListObject* formatlist = (struct ListObject*)format;
	struct StringObject* result = StringObject_New();
	struct StringObject* selfstr = (struct StringObject*)self;
	int i = 0;
	while (i < selfstr->size)
	{
		if (selfstr->string[i] == '{')
		{
			i++;
			int index = selfstr->string[i] - '0';
			if (index < formatlist->size)
			{
				struct Object* obj = formatlist->item[index];
				struct Object* afterformat = Object_ToString(obj);
				if (afterformat != NULL&&CHECK(afterformat,"string"))
				{
					result = StringObject_Add(result, afterformat);
				}
			}
			i += 2;
		}
		else
		{
			result = StringObject_Add(result, StringObject_NewWithChar(selfstr->string[i]));
			i++;
		}
	}
	return result;
}

struct Object* StringObject_ToString(struct Object* self)
{
	return self;
}

struct Object* StringObject_ToString2(struct Object* arg)
{
	struct ListObject* args = (struct ListObject*)arg;
	return StringObject_ToString(args->item[0]);
}