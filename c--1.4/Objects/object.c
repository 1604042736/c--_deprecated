#include "object.h"
#include "cfunctionobject.h"
#include "dictobject.h"
#include "stringobject.h"
#include "exceptionobject.h"

struct Object* Object_Add(struct Object* self, struct Object* other)
{
	if (self->objattr->attr != NULL)
	{
		GETATTR("add")
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
		struct Object* func = DictObject_GetItem(self->objattr->attr, StringObject_NewWithString("and"));
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
		struct Object* func = DictObject_GetItem(self->objattr->attr, StringObject_NewWithString("bool"));
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
		struct Object* func = DictObject_GetItem(self->objattr->attr, StringObject_NewWithString("call"));
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
		struct Object* func = DictObject_GetItem(self->objattr->attr, StringObject_NewWithString("div"));
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
		GETATTR("eq")
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
		struct Object* func = DictObject_GetItem(self->objattr->attr, StringObject_NewWithString("getattr"));
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
		struct Object* func = DictObject_GetItem(self->objattr->attr, StringObject_NewWithString("getitem"));
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
		GETATTR("geq")
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
		struct Object* func = DictObject_GetItem(self->objattr->attr, StringObject_NewWithString("gt"));
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
		struct Object* func = DictObject_GetItem(self->objattr->attr, StringObject_NewWithString("leq"));
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
		GETATTR("lt")
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
		struct Object* func = DictObject_GetItem(self->objattr->attr, StringObject_NewWithString("mod"));
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
		struct Object* func = DictObject_GetItem(self->objattr->attr, StringObject_NewWithString("mul"));
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
		struct Object* func = DictObject_GetItem(self->objattr->attr, StringObject_NewWithString("neq"));
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
		GETATTR("or")
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
	struct Object* func = DictObject_GetItem(self->objattr->attr, StringObject_NewWithString("tostring"));
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
	struct Object* func = DictObject_GetItem(self->objattr->attr, StringObject_NewWithString("setattr"));
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
	struct Object* func = DictObject_GetItem(self->objattr->attr, StringObject_NewWithString("setitem"));
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
		GETATTR("sub")
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