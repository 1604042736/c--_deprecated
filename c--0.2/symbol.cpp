#include "header-file/symbol.h"

char* file;
int arrangement=0;
stack<int> scope;
stack<char*> stae;

void st_init(Symtab* symtab) 
{
	if (symtab == NULL) 
	{
		return;
	}
	for (int i = 0; i < SIZE; i++) 
	{
		symtab->hashTable[i] = NULL;
	}
}

static int hash(char* key) 
{
	int temp = 0;
	int i = 0;
	while (key[i] != '\0') 
	{
		temp = ((temp << SHIFT) + key[i]) % SIZE;
		++i;
	}
	return temp;
}

void st_insert(Symtab* symtab, char* name, int lineno, int loc) 
{
	int h = hash(name);
	BucketList l = symtab->hashTable[h];
	while (l != NULL && strcmp(name, l->name) != 0) 
	{
		l = l->next;
	}
	if (l == NULL) 
	{
		l = (BucketList)malloc(sizeof(struct BucketListRec));
		l->name = name;
		l->lines = (LineList)malloc(sizeof(struct LineListRec));
		l->lines->lineno = lineno;
		l->memloc = loc;
		l->sc=scope.top();
		l->lines->next = NULL;
		l->next = symtab->hashTable[h];
		symtab->hashTable[h] = l;
	}
	else 
	{
		LineList t = l->lines;
		while (t->next != NULL) 
		{
			t = t->next;
		}
		t->next = (LineList)malloc(sizeof(struct LineListRec));
		t->next->lineno = lineno;
		t->next->next = NULL;
	}
}

int st_lookup(Symtab* symtab, char* name) 
{
	int h = hash(name);
	BucketList l = symtab->hashTable[h];
	while (l != NULL && strcmp(name, l->name) != 0) 
	{
		l = l->next;
	}
	if (l == NULL) 
	{
		return -1;
	}
	else if(l->sc!=scope.top()&&!strcmp(stae.top(),"new"))
	{
		stae.pop();
		return -1;
	}
	else
	{
		return l->memloc;
	}
}

void st_lookup_point(Symtab* symtab, char* name,int way) 
{
	int h = hash(name);
	BucketList l = symtab->hashTable[h];
	while (l != NULL && strcmp(name, l->name) != 0) 
	{
		l = l->next;
	}
	if(l!=NULL)
	{
		l->memloc=way;
	}
}

void printSymTab(Symtab* symtab) 
{
	int i;
	printf("Variable Name  Location  Line Numbers\n\n");
	for (i = 0; i < SIZE; ++i) 
	{
		if (symtab->hashTable[i] != NULL) 
		{
			BucketList l = symtab->hashTable[i];
			while (l != NULL) 
			{
				LineList t = l->lines;
				printf("%-14s ", l->name);
				printf("%-8d  ", l->memloc);
				while (t != NULL)
				{
					printf("%4d ", t->lineno);
					t = t->next;
				}
				printf("\n");
				l = l->next;
			}
		}
	}
}

void analyzer_init(Analyzer* analyzer, Symtab* symtab) 
{
	analyzer->symtab = symtab;
	analyzer->location = 0;
	analyzer->error = false;
}

static void traverse(Analyzer* analyzer,TreeNode* t, void(*preProc)(Analyzer*,TreeNode*), void(*postProc)(Analyzer*,TreeNode*)) 
{
	bool bl=false;
	if (t != NULL) 
	{
		preProc(analyzer,t);
		if(t->kind==Ifk||t->kind==WhileK||t->kind==ForK||t->kind==DoWhileK||t->kind==FuncComeK)
		{
			scope.push(t->lineno);
			arrangement++;
			bl=true;
		}
		
		for (int i = 0; i < MAXCHILD; i++) 
		{
			traverse(analyzer,t->child[i], preProc, postProc);
		}
		
		if(bl)
		{
			scope.pop();
			arrangement--;
		}
		
		postProc(analyzer,t);
		traverse(analyzer,t->sibling, preProc, postProc);
	}
}

static void nullProc(Analyzer* analyzer, TreeNode* t) 
{
	//if (t == NULL) {
	//	return;
	//}
	//else {
	//	return;
	//}
	return;
}

static void insertNode(Analyzer* analyzer,TreeNode* t) 
{
	switch (t->kind) 
	{
		case FuncStateK:
			if (st_lookup(analyzer->symtab, t->name) == -1)
			{
				st_insert(analyzer->symtab,t->name, t->lineno, analyzer->location++);
			}
			else
			{
				st_insert(analyzer->symtab, t->name, t->lineno, 0);
			}
			break;
		case StringK:
		case NewPointerK:
		case NewArrayK:
		case IntK:
		case VoidK:
			stae.push((char*)"new");
			if(st_lookup(analyzer->symtab, t->name) != -1)
			{
				error((char*)"重定义: %s",file,t->lineno,t->linepos,t->name);
			}
			else
			{
				st_insert(analyzer->symtab,t->name, t->lineno, analyzer->location++);
			}
			if(!strcmp(stae.top(),"new"))
			{
				stae.pop();
			}
			break;
		case FuncComeK:
			if (st_lookup(analyzer->symtab, t->name) == -1)
			{
				st_insert(analyzer->symtab,t->name, t->lineno, analyzer->location++);
			}
			else
			{
				st_insert(analyzer->symtab, t->name, t->lineno, 0);
			}
			break;
		case CallK:
		case AssignK:
		case Idk:
			if (st_lookup(analyzer->symtab, t->name) == -1)
			{
				error((char*)"未声明的变量: %s",file,t->lineno,t->linepos,t->name);
			}
			else
			{
				st_insert(analyzer->symtab, t->name, t->lineno, 0);
			}
			break;
		case Ifk:
		case WhileK:
		case DoWhileK:
		case ForK:
			if(arrangement==0)
			{
				error((char*)"不能在外面使用控制语句",file,t->lineno,t->linepos);
			}
			break;
	}
}

void buildSymtab(Analyzer* analyzer, TreeNode* syntaxTree)
{
	traverse(analyzer,syntaxTree, insertNode, nullProc);
}

Analyzer* symbol_start(TreeNode* syntaxTree,char* filename)
{
	file=filename;
	stae.push((char*)"");
	scope.push(1);
	Symtab* symtab = (Symtab*)malloc(sizeof(Symtab));
	st_init(symtab);
	Analyzer* analyzer = (Analyzer*)malloc(sizeof(Analyzer));
	analyzer_init(analyzer, symtab);
	buildSymtab(analyzer, syntaxTree);
	return analyzer; 
}
