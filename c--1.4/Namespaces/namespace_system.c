#include "namespace_system.h"
#include "stringobject.h"

void Namespace_System_Init()
{
#define ADDSTRVAL(NAME,VALUE) DictObject_SetItem(namespace_system->globals,StringObject_NewWithString(NAME),StringObject_NewWithString(VALUE))
	namespace_system = NamespaceObject_NewWithName("system");
	ADDSTRVAL("version", "1.4");
#undef ADDSTRVAL
}