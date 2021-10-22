#include "object.h"

ObjectAttr* ObjectAttr_New(string name)
{
	ObjectAttr* objattr = new ObjectAttr;
	objattr->obj_name = name;
	objattr->obj_print = Object_Print;
	objattr->obj_additem = Object_AddItem;
	objattr->obj_finditem = Object_FindItem;
	objattr->obj_getitem = Object_GetItem;
	objattr->obj_eq = Object_Eq;
	objattr->obj_dictadditem = Object_DictAddItem;
	objattr->obj_add = Object_Add;
	objattr->obj_sub = Object_Sub;
	objattr->obj_mul = Object_Mul;
	objattr->obj_div = Object_Div;
	objattr->obj_mod = Object_Mod;
	objattr->obj_insertitem = Object_InsertItem;
	objattr->obj_bool = Object_Bool;
	objattr->obj_copy = Object_Copy;
	objattr->obj_or = Object_Or;
	objattr->obj_setitem = Object_SetItem;
	objattr->obj_gt = Object_Gt;
	objattr->obj_geq = Object_Geq;
	objattr->obj_lt = Object_Lt;
	objattr->obj_leq = Object_Leq;
	objattr->obj_neq = Object_Neq;
	objattr->obj_and = Object_And;
	return objattr;
}

void Object_Print(Object*)
{

}

void Object_AddItem(Object*, Object*)
{

}

int Object_FindItem(Object*, Object*)
{
	return -1;
}

bool Object_Eq(Object*, Object*)
{
	return false;
}

Object* Object_GetItem(Object*, Object*)
{
	return NULL;
}

void Object_DictAddItem(Object*, Object*, Object*)
{

}

Object* Object_Add(Object*, Object*)
{
	return NULL;
}

Object* Object_Sub(Object*, Object*)
{
	return NULL;
}

Object* Object_Mul(Object*, Object*)
{
	return NULL;
}

Object* Object_Div(Object*, Object*)
{
	return NULL;
}

Object* Object_Mod(Object*, Object*)
{
	return NULL;
}

void Object_InsertItem(Object*, Object*, Object*)
{

}

bool Object_Bool(Object*)
{
	return false;
}

Object* Object_Copy(Object*)
{
	return NULL;
}

bool Object_Or(Object*, Object*)
{
	return false;
}

void Object_SetItem(Object*, Object*,Object*)
{

}

bool Object_Gt(Object*, Object*)
{
	return false;
}

bool Object_Geq(Object*, Object*)
{
	return false;
}

bool Object_Lt(Object*, Object*)
{
	return false;
}

bool Object_Leq(Object*, Object*)
{
	return false;
}

bool Object_Neq(Object*, Object*)
{
	return false;
}

bool Object_And(Object*, Object*)
{
	return false;
}

void print(Object* obj)
{
	if(obj!=NULL)
		obj->objattr->obj_print(obj);
}