#include "object.h"

struct Object* Object_Add(struct Object* self, struct Object* other)
{
	if (self->objattr->obj_add != NULL)
	{
		return self->objattr->obj_add(self, other);
	}
}

struct Object* Object_And(struct Object* self, struct Object* other)
{
	if (self->objattr->obj_and != NULL)
	{
		return self->objattr->obj_and(self, other);
	}
}

int	Object_Bool(struct Object* self)
{
	if (self->objattr->obj_bool != NULL)
	{
		return self->objattr->obj_bool(self);
	}
}

struct Object* Object_Call(struct Object* self, struct Object* args)
{
	if (self->objattr->obj_call != NULL)
	{
		return self->objattr->obj_call(self, args);
	}
}

struct Object* Object_Div(struct Object* self, struct Object* other)
{
	if (self->objattr->obj_div != NULL)
	{
		return self->objattr->obj_div(self, other);
	}
}
struct Object* Object_Eq(struct Object* self, struct Object* other)
{
	if (self->objattr->obj_eq != NULL)
	{
		return self->objattr->obj_eq(self, other);
	}
}

struct Object* Object_GetAttr(struct Object* self, struct Object* attr)
{
	if (self->objattr->obj_getattr != NULL)
	{
		return self->objattr->obj_getattr(self, attr);
	}
}

struct Object* Object_GetItem(struct Object* self, struct Object* index)
{
	if (self->objattr->obj_getitem != NULL)
	{
		return self->objattr->obj_getitem(self, index);
	}
}

struct Object* Object_Geq(struct Object* self, struct Object* other)
{
	if (self->objattr->obj_geq != NULL)
	{
		return self->objattr->obj_geq(self, other);
	}
}

struct Object* Object_Gt(struct Object* self, struct Object* other)
{
	if (self->objattr->obj_gt != NULL)
	{
		return self->objattr->obj_gt(self, other);
	}
}

void Object_InsertItem(struct Object* self, int index, struct Object* item)
{
	if (self->objattr->obj_insertitem != NULL)
	{
		self->objattr->obj_insertitem(self, index,item);
	}
}

struct Object* Object_Leq(struct Object* self, struct Object* other)
{
	if (self->objattr->obj_leq != NULL)
	{
		return self->objattr->obj_leq(self, other);
	}
}

struct Object* Object_Lt(struct Object* self, struct Object* other)
{
	if (self->objattr->obj_lt != NULL)
	{
		return self->objattr->obj_lt(self, other);
	}
}

struct Object* Object_Mod(struct Object* self, struct Object* other)
{
	if (self->objattr->obj_mod != NULL)
	{
		return self->objattr->obj_mod(self, other);
	}
}

struct Object* Object_Mul(struct Object* self, struct Object* other)
{
	if (self->objattr->obj_mul != NULL)
	{
		return self->objattr->obj_mul(self, other);
	}
}

struct Object* Object_Neq(struct Object* self, struct Object* other)
{
	if (self->objattr->obj_neq != NULL)
	{
		return self->objattr->obj_neq(self, other);
	}
}

struct Object* Object_New()
{
	struct Object* obj = (struct Object*)malloc(sizeof(struct Object));	//TODO ÄÚ´æ¹ÜÀí
	obj->objattr = &ObjectObjectAttribute;
	return obj;
}

struct Object* Object_Or(struct Object* self, struct Object* other)
{
	if (self->objattr->obj_or != NULL)
	{
		return self->objattr->obj_or(self, other);
	}
}

void Object_Print(struct Object* self)
{
	if (self->objattr->obj_print != NULL)
	{
		self->objattr->obj_print(self);
	}
}

void Object_SetAttr(struct Object* self, struct Object* attr, struct Object* obj)
{
	if (self->objattr->obj_setattr != NULL)
	{
		return self->objattr->obj_setattr(self, attr,obj);
	}
}

void Object_SetItem(struct Object* self, struct Object* index, struct Object* obj)
{
	if (self->objattr->obj_setitem != NULL)
	{
		return self->objattr->obj_setitem(self, index,obj);
	}
}

struct Object* Object_Sub(struct Object* self, struct Object* other)
{
	if (self->objattr->obj_sub != NULL)
	{
		return self->objattr->obj_sub(self, other);
	}
}