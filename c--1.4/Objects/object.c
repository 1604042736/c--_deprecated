#include "object.h"
#include "cfunctionobject.h"
#include "dictobject.h"
#include "stringobject.h"
#include "exceptionobject.h"
#include "listobject.h"
#include "intobject.h"

void Object_Init()
{
#define ADD(ATTR,NAME,FUNC)	DictObject_SetItem(ATTR, StringObject_NewWithString(NAME), CFunctionObject_NewWithCFunc(NAME, FUNC));
	listobjectattr = DictObject_New();
	ADD(listobjectattr, "insert", ListObject_InsertItem2);
	ADD(listobjectattr, "remove", ListObject_ListDelItem2);

	intobjectattr = DictObject_New();
	ADD(intobjectattr, "add", IntObject_Add2);
	ADD(intobjectattr, "sub", IntObject_Sub2);
	ADD(intobjectattr, "mul", IntObject_Mul2);
	ADD(intobjectattr, "div", IntObject_Div2);
	ADD(intobjectattr, "mod", IntObject_Mod2);
	ADD(intobjectattr, "eq", IntObject_Eq2);
	ADD(intobjectattr, "neq", IntObject_Neq2);
	ADD(intobjectattr, "lt", IntObject_Lt2);
	ADD(intobjectattr, "leq", IntObject_Leq2);
	ADD(intobjectattr, "bool", IntObject_Bool2);
	ADD(intobjectattr, "and", IntObject_And2);
	ADD(intobjectattr, "or", IntObject_Or2);
	ADD(intobjectattr, "geq", IntObject_Geq2);

	stringobjattr = DictObject_New();
	ADD(stringobjattr, "add", StringObject_Add2);
	ADD(stringobjattr, "split", StringObject_Split2);
	ADD(stringobjattr, "eq", StringObject_Add2);
	ADD(stringobjattr, "tostring", StringObject_ToString2);
#undef ADD
#define NEWCONSTSTR(NAME) conststr_##NAME=StringObject_NewWithString(#NAME)
	NEWCONSTSTR(add);
	NEWCONSTSTR(and);
	NEWCONSTSTR(bool);
	NEWCONSTSTR(call);
	NEWCONSTSTR(copy);
	NEWCONSTSTR(div);
	NEWCONSTSTR(eq);
	NEWCONSTSTR(getattr);
	NEWCONSTSTR(getitem);
	NEWCONSTSTR(geq);
	NEWCONSTSTR(gt);
	NEWCONSTSTR(hash);
	NEWCONSTSTR(insertitem);
	NEWCONSTSTR(leq);
	NEWCONSTSTR(lt);
	NEWCONSTSTR(mod);
	NEWCONSTSTR(mul);
	NEWCONSTSTR(neq);
	NEWCONSTSTR(new);
	NEWCONSTSTR(or );
	NEWCONSTSTR(print);
	NEWCONSTSTR(tostring);
	NEWCONSTSTR(setattr);
	NEWCONSTSTR(setitem);
	NEWCONSTSTR(sub);
#undef NEWCONSTSTR
	char* includeenvpath = getenv(INCLUDEENVPATH);
	if (includeenvpath != NULL)
	{
		conststr_includepath = StringObject_NewWithString(includeenvpath);
	}
	else
	{
		conststr_includepath = StringObject_NewWithString(".\\.include\\");
	}
}

struct Object* Object_Add(struct Object* self, struct Object* other)
{
	if (self->objattr->attr != NULL)
	{
		GETATTR(add)
		if (func != NULL)
		{
			struct  ListObject* args = ListObject_New();
			ListObject_InsertItem(args, 0, self);
			ListObject_InsertItem(args, 0, other);
			return func->objattr->obj_call(func, args);
		}
		else
		{
			exception = ExceptionObject_New();
			exception->message = StringObject_NewWithString("不存在该运算符");
			return NULL;
		}
	}
	else
	{
		exception = ExceptionObject_New();
		exception->message = StringObject_NewWithString("不存在该运算符");
		return NULL;
	}
}

struct Object* Object_And(struct Object* self, struct Object* other)
{
	if (self->objattr->attr != NULL)
	{
		GETATTR(and)
		if (func != NULL)
		{
			struct  ListObject* args = ListObject_New();
			ListObject_InsertItem(args, 0, self);
			ListObject_InsertItem(args, 0, other);
			return func->objattr->obj_call(func, args);
		}
		else
		{
			exception = ExceptionObject_New();
			exception->message = StringObject_NewWithString("不存在该运算符");
			return NULL;
		}
	}
	else
	{
		exception = ExceptionObject_New();
		exception->message = StringObject_NewWithString("不存在该运算符");
		return NULL;
	}
}

int	Object_Bool(struct Object* self)
{
	if (self->objattr->attr != NULL)
	{
		GETATTR(bool)
		if (func != NULL)
		{
			struct  ListObject* args = ListObject_New();
			ListObject_InsertItem(args, 0, self);
			return func->objattr->obj_call(func, args);
		}
		else
		{
			exception = ExceptionObject_New();
			exception->message = StringObject_NewWithString("不存在该运算符");
			return NULL;
		}
	}
	else
	{
		exception = ExceptionObject_New();
		exception->message = StringObject_NewWithString("不存在该运算符");
		return NULL;
	}
}

struct Object* Object_Call(struct Object* self, struct Object* args)
{
	if (self->objattr->attr != NULL)
	{
		GETATTR(call)
		if (func != NULL)
		{
			return func->objattr->obj_call(func, args);
		}
		else
		{
			exception = ExceptionObject_New();
			exception->message = StringObject_NewWithString("不存在该运算符");
			return NULL;
		}
	}
	else
	{
		exception = ExceptionObject_New();
		exception->message = StringObject_NewWithString("不存在该运算符");
		return NULL;
	}
}

struct Object* Object_Div(struct Object* self, struct Object* other)
{
	if (self->objattr->attr != NULL)
	{
		GETATTR(div)
		if (func != NULL)
		{
			struct  ListObject* args = ListObject_New();
			ListObject_InsertItem(args, 0, self);
			ListObject_InsertItem(args, 0, other);
			return func->objattr->obj_call(func, args);
		}
		else
		{
			exception = ExceptionObject_New();
			exception->message = StringObject_NewWithString("不存在该运算符");
			return NULL;
		}
	}
	else
	{
		exception = ExceptionObject_New();
		exception->message = StringObject_NewWithString("不存在该运算符");
		return NULL;
	}
}
struct Object* Object_Eq(struct Object* self, struct Object* other)
{
	if (self->objattr->attr != NULL)
	{
		GETATTR(eq)
		if (func != NULL)
		{
			struct  ListObject* args = ListObject_New();
			ListObject_InsertItem(args, 0, self);
			ListObject_InsertItem(args, 0, other);
			return func->objattr->obj_call(func, args);
		}
		else
		{
			exception = ExceptionObject_New();
			exception->message = StringObject_NewWithString("不存在该运算符");
			return NULL;
		}
	}
	else
	{
		exception = ExceptionObject_New();
		exception->message = StringObject_NewWithString("不存在该运算符");
		return NULL;
	}
}

struct Object* Object_GetAttr(struct Object* self, struct Object* attr)
{
	if (self->objattr->attr != NULL)
	{
		GETATTR(getattr)
		if (func != NULL)
		{
			struct  ListObject* args = ListObject_New();
			ListObject_InsertItem(args, 0, self);
			ListObject_InsertItem(args, 0, attr);
			return func->objattr->obj_call(func, args);
		}
		else
		{
			exception = ExceptionObject_New();
			exception->message = StringObject_NewWithString("不存在该运算符");
			return NULL;
		}
	}
	else
	{
		exception = ExceptionObject_New();
		exception->message = StringObject_NewWithString("不存在该运算符");
		return NULL;
	}
}

struct Object* Object_GetItem(struct Object* self, struct Object* index)
{
	if (self->objattr->attr != NULL)
	{
		GETATTR(getitem)
		if (func != NULL)
		{
			struct  ListObject* args = ListObject_New();
			ListObject_InsertItem(args, 0, self);
			ListObject_InsertItem(args, 0, index);
			return func->objattr->obj_call(func, args);
		}
		else
		{
			exception = ExceptionObject_New();
			exception->message = StringObject_NewWithString("不存在该运算符");
			return NULL;
		}
	}
	else
	{
		exception = ExceptionObject_New();
		exception->message = StringObject_NewWithString("不存在该运算符");
		return NULL;
	}
}

struct Object* Object_Geq(struct Object* self, struct Object* other)
{
	if (self->objattr->attr != NULL)
	{
		GETATTR(geq)
		//struct Object* func = DictObject_GetItem(self->objattr->attr, StringObject_NewWithString("geq"));
		if (func != NULL)
		{
			struct  ListObject* args = ListObject_New();
			ListObject_InsertItem(args, 0, self);
			ListObject_InsertItem(args, 0, other);
			return func->objattr->obj_call(func, args);
		}
		else
		{
			exception = ExceptionObject_New();
			exception->message = StringObject_NewWithString("不存在该运算符");
			return NULL;
		}
	}
	else
	{
		exception = ExceptionObject_New();
		exception->message = StringObject_NewWithString("不存在该运算符");
		return NULL;
	}
}

struct Object* Object_Gt(struct Object* self, struct Object* other)
{
	if (self->objattr->attr != NULL)
	{
		GETATTR(gt)
		if (func != NULL)
		{
			struct  ListObject* args = ListObject_New();
			ListObject_InsertItem(args, 0, self);
			ListObject_InsertItem(args, 0, other);
			return func->objattr->obj_call(func, args);
		}
		else
		{
			exception = ExceptionObject_New();
			exception->message = StringObject_NewWithString("不存在该运算符");
			return NULL;
		}
	}
	else
	{
		exception = ExceptionObject_New();
		exception->message = StringObject_NewWithString("不存在该运算符");
		return NULL;
	}
}

void Object_InsertItem(struct Object* self, int index, struct Object* item)
{
	if (self->objattr->attr != NULL)
	{
		struct Object* func = DictObject_GetItem(self->objattr->attr, StringObject_NewWithString("insert"));
		if (func != NULL)
		{
			struct  ListObject* args = ListObject_New();
			ListObject_InsertItem(args, 0, self);
			ListObject_InsertItem(args, 0, IntObject_NewWithValue(index));
			ListObject_InsertItem(args, 0, item);
			return func->objattr->obj_call(func, args);
		}
		else
		{
			exception = ExceptionObject_New();
			exception->message = StringObject_NewWithString("不存在该运算符");
			return NULL;
		}
	}
	else
	{
		exception = ExceptionObject_New();
		exception->message = StringObject_NewWithString("不存在该运算符");
		return NULL;
	}
}

struct Object* Object_Leq(struct Object* self, struct Object* other)
{
	if (self->objattr->attr != NULL)
	{
		GETATTR(leq)
		if (func != NULL)
		{
			struct  ListObject* args = ListObject_New();
			ListObject_InsertItem(args, 0, self);
			ListObject_InsertItem(args, 0, other);
			return func->objattr->obj_call(func, args);
		}
		else
		{
			exception = ExceptionObject_New();
			exception->message = StringObject_NewWithString("不存在该运算符");
			return NULL;
		}
	}
	else
	{
		exception = ExceptionObject_New();
		exception->message = StringObject_NewWithString("不存在该运算符");
		return NULL;
	}
}

struct Object* Object_Lt(struct Object* self, struct Object* other)
{
	if (self->objattr->attr != NULL)
	{
		GETATTR(lt)
		if (func != NULL)
		{
			struct  ListObject* args = ListObject_New();
			ListObject_InsertItem(args, 0, self);
			ListObject_InsertItem(args, 0, other);
			return func->objattr->obj_call(func, args);
		}
		else
		{
			exception = ExceptionObject_New();
			exception->message = StringObject_NewWithString("不存在该运算符");
			return NULL;
		}
	}
	else
	{
		exception = ExceptionObject_New();
		exception->message = StringObject_NewWithString("不存在该运算符");
		return NULL;
	}
}

struct Object* Object_Mod(struct Object* self, struct Object* other)
{
	if (self->objattr->attr != NULL)
	{
		GETATTR(mod)
		if (func != NULL)
		{
			struct  ListObject* args = ListObject_New();
			ListObject_InsertItem(args, 0, self);
			ListObject_InsertItem(args, 0, other);
			return func->objattr->obj_call(func, args);
		}
		else
		{
			exception = ExceptionObject_New();
			exception->message = StringObject_NewWithString("不存在该运算符");
			return NULL;
		}
	}
	else
	{
		exception = ExceptionObject_New();
		exception->message = StringObject_NewWithString("不存在该运算符");
		return NULL;
	}
}

struct Object* Object_Mul(struct Object* self, struct Object* other)
{
	if (self->objattr->attr != NULL)
	{
		GETATTR(mul)
		if (func != NULL)
		{
			struct  ListObject* args = ListObject_New();
			ListObject_InsertItem(args, 0, self);
			ListObject_InsertItem(args, 0, other);
			return func->objattr->obj_call(func, args);
		}
		else
		{
			exception = ExceptionObject_New();
			exception->message = StringObject_NewWithString("不存在该运算符");
			return NULL;
		}
	}
	else
	{
		exception = ExceptionObject_New();
		exception->message = StringObject_NewWithString("不存在该运算符");
		return NULL;
	}
}

struct Object* Object_Neq(struct Object* self, struct Object* other)
{
	if (self->objattr->attr != NULL)
	{
		GETATTR(neq)
		if (func != NULL)
		{
			struct  ListObject* args = ListObject_New();
			ListObject_InsertItem(args, 0, self);
			ListObject_InsertItem(args, 0, other);
			return func->objattr->obj_call(func, args);
		}
		else
		{
			exception = ExceptionObject_New();
			exception->message = StringObject_NewWithString("不存在该运算符");
			return NULL;
		}
	}
	else
	{
		exception = ExceptionObject_New();
		exception->message = StringObject_NewWithString("不存在该运算符");
		return NULL;
	}
}

struct Object* Object_New()
{
	struct Object* obj = (struct Object*)malloc(sizeof(struct Object));	//TODO 内存管理
	obj->objattr = &ObjectObjectAttribute;
	obj->refcount = DEFAULTREFCOUNT;
	return obj;
}

struct Object* Object_Or(struct Object* self, struct Object* other)
{
	if (self->objattr->attr != NULL)
	{
		GETATTR(or)
		if (func != NULL)
		{
			struct  ListObject* args = ListObject_New();
			ListObject_InsertItem(args, 0, self);
			ListObject_InsertItem(args, 0, other);
			return func->objattr->obj_call(func, args);
		}
		else
		{
			exception = ExceptionObject_New();
			exception->message = StringObject_NewWithString("不存在该运算符");
			return NULL;
		}
	}
	else
	{
		exception = ExceptionObject_New();
		exception->message = StringObject_NewWithString("不存在该运算符");
		return NULL;
	}
}

void Object_Print(struct Object* self)
{
	if (self->objattr->obj_print != NULL)
	{
		self->objattr->obj_print(self);
	}
}

struct Object* Object_ToString(struct Object* self)
{
	GETATTR(tostring)
	if (func != NULL)
	{
		struct  ListObject* args = ListObject_New();
		ListObject_InsertItem(args, 0, self);
		return func->objattr->obj_call(func, args);
	}
	else
	{
		exception = ExceptionObject_New();
		exception->message = StringObject_NewWithString("不存在该运算符");
		return NULL;
	}
}

void Object_SetAttr(struct Object* self, struct Object* attr, struct Object* obj)
{
	GETATTR(setattr)
	if (func != NULL)
	{
		struct  ListObject* args = ListObject_New();
		ListObject_InsertItem(args, 0, self);
		ListObject_InsertItem(args, 0, attr);
		ListObject_InsertItem(args, 0, obj);
		return func->objattr->obj_call(func, args);
	}
}

void Object_SetItem(struct Object* self, struct Object* index, struct Object* obj)
{
	GETATTR(setitem)
	if (func != NULL)
	{
		struct  ListObject* args = ListObject_New();
		ListObject_InsertItem(args, 0, self);
		ListObject_InsertItem(args, 0,IntObject_NewWithValue(index));
		ListObject_InsertItem(args, 0, obj);
		return func->objattr->obj_call(func, args);
	}
}

struct Object* Object_Sub(struct Object* self, struct Object* other)
{
	if (self->objattr->attr != NULL)
	{
		GETATTR(sub)
		if (func != NULL)
		{
			struct  ListObject* args = ListObject_New();
			ListObject_InsertItem(args, 0, self);
			ListObject_InsertItem(args, 0, other);
			return func->objattr->obj_call(func, args);
		}
		else
		{
			exception = ExceptionObject_New();
			exception->message = StringObject_NewWithString("不存在该运算符");
			return NULL;
		}
	}
	else
	{
		exception = ExceptionObject_New();
		exception->message = StringObject_NewWithString("不存在该运算符");
		return NULL;
	}
}