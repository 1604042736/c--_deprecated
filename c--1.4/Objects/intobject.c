#include "intobject.h"
#include "listobject.h"

struct Object* IntObject_New()
{
	struct IntObject* intobj = Memory_Malloc(memory, "int");
	intobj->objattr = &IntObjectAttribute;
	intobj->value = 0;
	intobj->objattr->attr = intobjectattr;
	intobj->refcount = DEFAULTREFCOUNT;
	return (struct Object*)intobj;
}

struct Object* IntObject_NewWithValue(int val)
{
	struct IntObject* intobj = (struct IntObject*)IntObject_New();
	intobj->value = val;
	return (struct Object*)intobj;
}

void IntObject_Print(struct Object* obj)
{
	struct IntObject* intobj = (struct IntObject*)obj;
	printf("%d", intobj->value);
}

struct Object* IntObject_Add(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "int"))
	{
		return NULL;
	}
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	int result = selfint->value + otherint->value;
	return IntObject_NewWithValue(result);
}

struct Object* IntObject_Eq(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "int"))
	{
		return NULL;
	}
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	return IntObject_NewWithValue(selfint->value == otherint->value);
}

int IntObject_toInt(struct Object* self)
{
	struct IntObject* selfint = (struct IntObject*)self;
	return selfint->value;
}

struct Object* IntObject_Sub(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "int"))
	{
		return NULL;
	}
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	int result = selfint->value - otherint->value;
	return IntObject_NewWithValue(result);
}

struct Object* IntObject_Mul(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "int"))
	{
		return NULL;
	}
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	int result = selfint->value * otherint->value;
	return IntObject_NewWithValue(result);
}

struct Object* IntObject_Div(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "int"))
	{
		return NULL;
	}
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	int result = selfint->value / otherint->value;
	return IntObject_NewWithValue(result);
}

struct Object* IntObject_Mod(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "int"))
	{
		return NULL;
	}
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	int result = selfint->value % otherint->value;
	return IntObject_NewWithValue(result);
}

struct Object* IntObject_Neq(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "int"))
	{
		return NULL;
	}
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	return IntObject_NewWithValue(selfint->value != otherint->value);
}

struct Object* IntObject_Gt(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "int"))
	{
		return NULL;
	}
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	return IntObject_NewWithValue(selfint->value > otherint->value);
}

struct Object* IntObject_Geq(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "int"))
	{
		return NULL;
	}
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	return IntObject_NewWithValue(selfint->value >= otherint->value);
}

struct Object* IntObject_Lt(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "int"))
	{
		return NULL;
	}
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	return IntObject_NewWithValue(selfint->value < otherint->value);
}

struct Object* IntObject_Leq(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "int"))
	{
		return NULL;
	}
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	return IntObject_NewWithValue(selfint->value <= otherint->value);
}

int IntObject_Bool(struct Object* self)
{
	if (self == NULL)
	{
		return NULL;
	}
	struct IntObject* selfint = (struct IntObject*)self;
	return selfint->value;
}

struct Object* IntObject_And(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "int"))
	{
		return NULL;
	}
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	return IntObject_NewWithValue(selfint->value && otherint->value);
}

struct Object* IntObject_Or(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "int"))
	{
		return NULL;
	}
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	return IntObject_NewWithValue(selfint->value || otherint->value);
}

struct Object* IntObject_Copy(struct Object* self)
{
	struct IntObject* selfint = (struct IntObject*)self;
	return IntObject_NewWithValue(selfint->value);
}

struct Object* IntObject_Add2(struct Object* arg)
{
	struct ListObject* args = (struct ListObject*)arg;
	return IntObject_Add(args->item[0], args->item[1]);
}

struct Object* IntObject_Eq2(struct Object* arg)
{
	struct ListObject* args = (struct ListObject*)arg;
	return IntObject_Eq(args->item[0], args->item[1]);
}

struct Object* IntObject_Sub2(struct Object* arg)
{
	struct ListObject* args = (struct ListObject*)arg;
	return IntObject_Sub(args->item[0], args->item[1]);
}

struct Object* IntObject_Mul2(struct Object* arg)
{
	struct ListObject* args = (struct ListObject*)arg;
	return IntObject_Mul(args->item[0], args->item[1]);
}

struct Object* IntObject_Div2(struct Object* arg)
{
	struct ListObject* args = (struct ListObject*)arg;
	return IntObject_Div(args->item[0], args->item[1]);
}

struct Object* IntObject_Mod2(struct Object* arg)
{
	struct ListObject* args = (struct ListObject*)arg;
	return IntObject_Mod(args->item[0], args->item[1]);
}

struct Object* IntObject_Neq2(struct Object* arg)
{
	struct ListObject* args = (struct ListObject*)arg;
	return IntObject_Neq(args->item[0], args->item[1]);
}

struct Object* IntObject_Gt2(struct Object* arg)
{
	struct ListObject* args = (struct ListObject*)arg;
	return IntObject_Gt(args->item[0], args->item[1]);
}

struct Object* IntObject_Geq2(struct Object* arg)
{
	struct ListObject* args = (struct ListObject*)arg;
	return IntObject_Geq(args->item[0], args->item[1]);
}

struct Object* IntObject_Lt2(struct Object* arg)
{
	struct ListObject* args = (struct ListObject*)arg;
	return IntObject_Lt(args->item[0], args->item[1]);
}

struct Object* IntObject_Leq2(struct Object* arg)
{
	struct ListObject* args = (struct ListObject*)arg;
	return IntObject_Leq(args->item[0], args->item[1]);
}

struct Object* IntObject_Bool2(struct Object* arg)
{
	struct ListObject* args = (struct ListObject*)arg;
	return IntObject_Bool(args->item[0]);
}

struct Object* IntObject_And2(struct Object* arg)
{
	struct ListObject* args = (struct ListObject*)arg;
	return IntObject_And(args->item[0], args->item[1]);
}

struct Object* IntObject_Or2(struct Object* arg)
{
	struct ListObject* args = (struct ListObject*)arg;
	return IntObject_Or(args->item[0], args->item[1]);
}