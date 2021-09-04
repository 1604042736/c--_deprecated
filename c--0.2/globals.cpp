#include "header-file/globals.h"

char getCh(FILE* file)
{
	return getc(file);
}

void ungetCh(char ch,FILE* file)
{
	ungetc(ch,file);
}

void printTree(TreeNode* tree,int s)
{
	while(tree!=NULL)
	{
		for(int i=0;i<s;i++)
		{
			printf(" ");
		}
		switch(tree->kind)
		{
			case Ifk:
				printf("If\n");
				break;
			case WhileK:
				printf("While\n");
				break;
			case ForK:
				printf("For\n");
				break;
			case DoWhileK:
				printf("DoWhile\n");
				break;
			case IntK:
				printf("Int: %s\n",tree->name);
				break;
			case VoidK:
				printf("Void: %s\n",tree->name);
				break;
			case Opk:
				printf("Op: %s\n",tree->name);
				break;
			case Idk:
				printf("Id: %s\n",tree->name);
				break;
			case Numk:
				printf("Num: %s\n",tree->name);
				break;
			case AssignK:
				printf("Assign to: %s\n",tree->name);
				break;
			case ParamK:
				printf("Param\n");
				break;
			case BreaKK:
				printf("Break\n");
				break;
			case ContinueK:
				printf("Continue\n");
				break;
			case CallK:
				printf("Call: %s\n",tree->name);
				break;
			case ReturnK:
				printf("Return\n");
				break;
			case PrintK:
				printf("Print\n");
				break;
			case InputK:
				printf("Input\n");
				break;
			case ArrayNumK:
				printf("Array Num\n");
				break;
			case StructK:
				printf("Struct: %s\n",tree->name);
				break;
			case MemberK:
				printf("Member: %s\n",tree->name);
				break;
			case ArrayAssignK:
				printf("Array Assign to: %s\n",tree->name);
				break;
			case ArrayK:
				printf("Array: %s\n",tree->name);
				break;
			case NewArrayK:
				printf("New Array: %s\n",tree->name);
				break;
			case DataKindK:
				printf("Data Kind: %s\n",tree->name);
				break;
			case FuncStateK:
				printf("Func State: %s\n",tree->name);
				break;
			case FuncComeK:
				printf("Func Come: %s\n",tree->name);
				break;
			case UserVarK:
				printf("UserVar: %s\n",tree->name);
				break;
			case ConstStringK:
				printf("Const String: %s\n",tree->name);
				break;
			case StringK:
				printf("String: %s\n",tree->name);
				break;
			case PointerK:
				printf("Pointer: %s\n",tree->name);
				break;
			case PointerAssignK:
				printf("Pointer Assign: %s\n",tree->name);
				break;
			case NewPointerK:
				printf("New Pointer: %s\n",tree->name);
				break;
		}
		for(int i=0;i<MAXCHILD;i++)
		{
			printTree(tree->child[i],s+2);
		}
		tree=tree->sibling;
	}
}
