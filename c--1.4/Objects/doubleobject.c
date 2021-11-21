#include "doubleobject.h"

struct Object* DoubleObject_New()
{
	struct DoubleObject* doubleobj = (struct DoubleObject*)malloc(sizeof(struct DoubleObject));
	doubleobj->objattr = &DoubleObjectAttribute;
	doubleobj->value = 0;
	return (struct Object*)doubleobj;
}

struct Object* DoubleObject_NewWithValue(double val)
{
	struct DoubleObject* doubleobj = (struct DoubleObject*)DoubleObject_New();
	doubleobj->value = val;
	return (struct Object*)doubleobj;
}

void DoubleObject_Print(struct Object* obj)
{
	struct DoubleObject* doubleobj = (struct DoubleObject*)obj;
	printf("%f", doubleobj->value);
}

struct Object* DoubleObject_Add(struct Object* self, struct Object* other)
{
	struct DoubleObject* selfdouble = (struct DoubleObject*)self;
	struct DoubleObject* otherdouble = (struct DoubleObject*)other;
	double result = selfdouble->value + otherdouble->value;
	return DoubleObject_NewWithValue(result);
}

int DoubleObject_Eq(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "double"))
	{
		return 0;
	}
	struct DoubleObject* selfint = (struct DoubleObject*)self;
	struct DoubleObject* otherint = (struct DoubleObject*)other;
	return selfint->value == otherint->value;
}

struct Object* DoubleObject_Sub(struct Object* self, struct Object* other)
{
	struct DoubleObject* selfint = (struct DoubleObject*)self;
	struct DoubleObject* otherint = (struct DoubleObject*)other;
	double result = selfint->value - otherint->value;
	return DoubleObject_NewWithValue(result);
}

struct Object* DoubleObject_Mul(struct Object* self, struct Object* other)
{
	struct DoubleObject* selfint = (struct DoubleObject*)self;
	struct DoubleObject* otherint = (struct DoubleObject*)other;
	double result = selfint->value * otherint->value;
	return DoubleObject_NewWithValue(result);
}

struct Object* DoubleObject_Div(struct Object* self, struct Object* other)
{
	struct DoubleObject* selfint = (struct DoubleObject*)self;
	struct DoubleObject* otherint = (struct DoubleObject*)other;
	double result = selfint->value / otherint->value;
	return DoubleObject_NewWithValue(result);
}

struct Object* DoubleObject_Neq(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "double"))
	{
		return 0;
	}
	struct DoubleObject* selfint = (struct DoubleObject*)self;
	struct DoubleObject* otherint = (struct DoubleObject*)other;
	return DoubleObject_NewWithValue(selfint->value != otherint->value);
}

struct Object* DoubleObject_Gt(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "double"))
	{
		return 0;
	}
	struct DoubleObject* selfint = (struct DoubleObject*)self;
	struct DoubleObject* otherint = (struct DoubleObject*)other;
	return DoubleObject_NewWithValue(selfint->value > otherint->value);
}

struct Object* DoubleObject_Geq(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "double"))
	{
		return 0;
	}
	struct DoubleObject* selfint = (struct DoubleObject*)self;
	struct DoubleObject* otherint = (struct DoubleObject*)other;
	return DoubleObject_NewWithValue(selfint->value >= otherint->value);
}

struct Object* DoubleObject_Lt(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "double"))
	{
		return 0;
	}
	struct DoubleObject* selfint = (struct DoubleObject*)self;
	struct DoubleObject* otherint = (struct DoubleObject*)other;
	return DoubleObject_NewWithValue(selfint->value < otherint->value);
}

struct Object* DoubleObject_Leq(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "double"))
	{
		return 0;
	}
	struct DoubleObject* selfint = (struct DoubleObject*)self;
	struct DoubleObject* otherint = (struct DoubleObject*)other;
	return DoubleObject_NewWithValue(selfint->value <= otherint->value);
}

int DoubleObject_Bool(struct Object* self)
{
	struct DoubleObject* selfint = (struct DoubleObject*)self;
	return selfint->value;
}

struct Object* DoubleObject_And(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "double"))
	{
		return 0;
	}
	struct DoubleObject* selfint = (struct DoubleObject*)self;
	struct DoubleObject* otherint = (struct DoubleObject*)other;
	return DoubleObject_NewWithValue(selfint->value && otherint->value);
}

struct Object* DoubleObject_Or(struct Object* self, struct Object* other)
{
	if (!CHECK(other, "double"))
	{
		return 0;
	}
	struct DoubleObject* selfint = (struct DoubleObject*)self;
	struct DoubleObject* otherint = (struct DoubleObject*)other;
	return DoubleObject_NewWithValue(selfint->value || otherint->value);
}