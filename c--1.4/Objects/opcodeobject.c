#include "opcodeobject.h"

struct Object* OpCodeObject_New()
{
	struct OpCodeObject* opcodeobj = (struct OpCodeObject*)malloc(sizeof(struct OpCodeObject));
	opcodeobj->objattr = &OpCodeObjectAttribute;
	opcodeobj->oparg = 0;
	return (struct Object*)opcodeobj;
}

struct Object* OpCodeObject_NewWithOpCodeAndLine(unsigned short op, short oparg, int lineno, int linepos)
{
	struct OpCodeObject* opcodeobj = OpCodeObject_New();
	opcodeobj->op = op;
	opcodeobj->oparg = oparg;
	opcodeobj->opcode = (op << 8) + oparg;
	opcodeobj->lineno = lineno;
	opcodeobj->linepos = linepos;
	return (struct Object*)opcodeobj;
}

void OpCodeObject_Print(struct Object* self)
{
	struct OpCodeObject* selfopcode = (struct OpCodeObject*)self;
	printf("%d\t%d\t%d\t%d", selfopcode->op, selfopcode->oparg, selfopcode->lineno, selfopcode->linepos);
}