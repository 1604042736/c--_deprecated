#include "intobject.h"

struct Object* IntObject_New()
{
	struct IntObject* intobj = (struct IntObject*)malloc(sizeof(struct IntObject));
	intobj->objattr = &IntObjectAttribute;
	intobj->value = 0;
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
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	int result = selfint->value + otherint->value;
	return IntObject_NewWithValue(result);
}

int IntObject_Eq(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "int"))
	{
		return 0;
	}
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	return selfint->value == otherint->value;
}

int IntObject_toInt(struct Object* self)
{
	struct IntObject* selfint = (struct IntObject*)self;
	return selfint->value;
}

struct Object* IntObject_Sub(struct Object* self, struct Object* other)
{
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	int result = selfint->value - otherint->value;
	return IntObject_NewWithValue(result);
}

struct Object* IntObject_Mul(struct Object* self, struct Object* other)
{
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	int result = selfint->value * otherint->value;
	return IntObject_NewWithValue(result);
}

struct Object* IntObject_Div(struct Object* self, struct Object* other)
{
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	int result = selfint->value / otherint->value;
	return IntObject_NewWithValue(result);
}

struct Object* IntObject_Mod(struct Object* self, struct Object* other)
{
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	int result = selfint->value % otherint->value;
	return IntObject_NewWithValue(result);
}

struct Object* IntObject_Neq(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "int"))
	{
		return 0;
	}
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	return IntObject_NewWithValue(selfint->value != otherint->value);
}

struct Object* IntObject_Gt(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "int"))
	{
		return 0;
	}
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	return IntObject_NewWithValue(selfint->value > otherint->value);
}

struct Object* IntObject_Geq(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "int"))
	{
		return 0;
	}
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	return IntObject_NewWithValue(selfint->value >= otherint->value);
}

struct Object* IntObject_Lt(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "int"))
	{
		return 0;
	}
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	return IntObject_NewWithValue(selfint->value < otherint->value);
}

struct Object* IntObject_Leq(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "int"))
	{
		return 0;
	}
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	return IntObject_NewWithValue(selfint->value <= otherint->value);
}

int IntObject_Bool(struct Object* self)
{
	struct IntObject* selfint = (struct IntObject*)self;
	return selfint->value;
}

struct Object* IntObject_And(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "int"))
	{
		return 0;
	}
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	return IntObject_NewWithValue(selfint->value && otherint->value);
}

struct Object* IntObject_Or(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "int"))
	{
		return 0;
	}
	struct IntObject* selfint = (struct IntObject*)self;
	struct IntObject* otherint = (struct IntObject*)other;
	return IntObject_NewWithValue(selfint->value || otherint->value);
}