#include "header-file/vmare.h"

vector< stack<char*> > data;
vector< stack< vector<char*> > > array;
vector< stack<TreeNode*> > function; 
stack<char*> state;
vector<char*> param;
stack< vector<int> > params;
stack< vector<int> > array_params;
stack< vector<int> > array_params_call_way;
stack< vector<int> > func_params;
vector< vector<char*> > array_param;
vector<TreeNode*> func_param;
stack<int> param_way;
stack<int> array_param_way;
stack<int> array_param_call_way;
stack<int> func_param_way;
stack< vector<int> > data_way;
stack< vector<int> > array_way;
int param_num=0;
Analyzer* anal;
TreeNode* treenode;
char* filename;

char* inttochar(int n)
{
	static char buf[1024];
	sprintf(buf,"%d\0",n);
	return buf;
}

char* get_exp(TreeNode* tree)
{
	if(tree!=NULL)
	{
		TreeNode* t;
		vector< stack<char*> > data_temp;
		vector< stack<TreeNode*> > function_temp;
		int a;
		char* one;
		char* two;
		switch(tree->kind)
		{
			case Idk:
				a=st_lookup(anal->symtab,tree->name);
				if(a!=-1)
				{
					return data[a].top();
				}
				break;
			case Numk:
				return tree->name;
			case ConstStringK:
				return tree->name;
			case PointerK:
				a=st_lookup(anal->symtab,tree->name);
				return inttochar(a);
			case Opk:
				one=copyString(get_exp(tree->child[0]));
				two=copyString(get_exp(tree->child[1]));
				if(!strcmp(tree->name,"+"))
				{
					//return add(one,two);
					return inttochar(atoi(one)+atoi(two));
				}
				else if(!strcmp(tree->name,"-"))
				{
					return inttochar(atoi(one)-atoi(two));
				}
				else if(!strcmp(tree->name,"*"))
				{
					//return mul(one,two);
					return inttochar(atoi(one)*atoi(two));
				}
				else if(!strcmp(tree->name,"/"))
				{
					//return _div(one,two);
					return inttochar(atoi(one)/atoi(two));
				}
				else if(!strcmp(tree->name,"%"))
				{
					//return mod(one,two);
					return inttochar(atoi(one)%atoi(two));
				}
				else if(!strcmp(tree->name,">"))
				{
					return inttochar((bool)(atoi(one)>atoi(two)));
				}
				else if(!strcmp(tree->name,"<"))
				{
					return inttochar((bool)(atoi(one)<atoi(two)));
				}
				else if(!strcmp(tree->name,">="))
				{
					return inttochar((bool)(atoi(one)>=atoi(two)));
				}
				else if(!strcmp(tree->name,"<="))
				{
					return inttochar((bool)(atoi(one)<=atoi(two)));
				}
				else if(!strcmp(tree->name,"&&"))
				{
					return inttochar((bool)(atoi(one)&&atoi(two)));
				}
				else if(!strcmp(tree->name,"||"))
				{
					return inttochar((bool)(atoi(one)||atoi(two)));
				}
				else if(!strcmp(tree->name,"=="))
				{
					return inttochar((bool)(atoi(one)==atoi(two)));
				}
				else if(!strcmp(tree->name,"!="))
				{
					return inttochar((bool)(atoi(one)!=atoi(two)));
				}
				break;
			case ArrayK:
				a=st_lookup(anal->symtab,tree->name);
				if(a!=-1)
				{
					int b=atoi(copyString(cGen(tree->child[0])));
					if(b<array[a].top().size())
					{
						return array[a].top()[b];
					}
					else
					{
						error((char*)"数组越界",filename,tree->lineno,tree->linepos);
					}
				}
				break;
			case CallK:
				function_temp=function;
				t=NULL;
				state.push((char*)"call");
				cGen(tree->child[0]);
				if(!state.empty()&&!strcmp(state.top(),"call"))
				{
					state.pop();
				}
				for(int i=0;i<function.size();i++)
				{
					if(function[i].top()->kind==FuncComeK&&!strcmp(function[i].top()->name,tree->name))
					{
						t=function[i].top();
						cGen(t->child[0]);
						state.push((char*)"funccome");
						cGen(t->child[1]);
						if(!state.empty()&&!strcmp(state.top(),"funccome"))
						{
							state.pop();
						}
						vector<int> pn;
						while(!param_way.empty())
						{
							pn.push_back(param_way.top());
							param_way.pop();
						}
						params.push(pn);
						pn.clear();
						while(!array_param_way.empty())
						{
							pn.push_back(array_param_way.top());
							array_param_way.pop();
						}
						array_params.push(pn);
						pn.clear();
						while(!func_param_way.empty())
						{
							pn.push_back(func_param_way.top());
							func_param_way.pop();
						}
						func_params.push(pn);
						pn.clear();
						while(!array_param_call_way.empty())
						{
							pn.push_back(array_param_call_way.top());
							array_param_call_way.pop();
						}
						array_params_call_way.push(pn);
						one=cGen(t->child[2]);
						for(int i=0;i<params.top().size();i++)
						{
							data[params.top()[i]].pop();
						}
						for(int i=0;i<array_params.top().size();i++)
						{
							vector<char*> dt=array[array_params.top()[i]].top();
							array[array_params.top()[i]].pop();
							if(!array_params_call_way.empty()&&!array_params_call_way.top().empty())
							{
								array[array_params_call_way.top()[i]].pop();
								array[array_params_call_way.top()[i]].push(dt);
							}
						}
						for(int i=0;i<func_params.top().size();i++)
						{
							function[func_params.top()[i]].pop();
						}
						if(!params.empty())
						{
							params.pop();
						}
						if(!array_params.empty())
						{
							array_params.pop();
						}
						if(!func_params.empty())
						{
							func_params.pop();
						}
						if(!array_params_call_way.empty())
						{
							array_params_call_way.pop();
						}
						if(!state.empty()&&!strcmp(state.top(),"return"))
						{
							state.pop();
						}
						break;
					}
				}
				function=function_temp;
				return one;
		}
	}
}

char* cGen(TreeNode* tree)
{
	if(tree!=NULL)
	{
		TreeNode* t;
		vector< stack<TreeNode*> > function_temp;
		vector<int> vi;
		int a;
		char* val;
		char* buf;
		switch(tree->kind)
		{
			case Ifk:
				function_temp=function;
				vi.push_back(0);
				data_way.push(vi);
				array_way.push(vi);
				if(!strcmp(copyString(get_exp(tree->child[0])),"1"))
				{
					val=copyString(cGen(tree->child[1]));
					if(!state.empty()&&!strcmp(state.top(),"return"))
					{
						return val;
					}
					else if(!state.empty()&&!strcmp(state.top(),"break"))
					{
						return val;
					}
					else if(!state.empty()&&!strcmp(state.top(),"continue"))
					{
						return val;
					}
				}
				else
				{
					if(tree->child[2]!=NULL)
					{
						val=copyString(cGen(tree->child[2]));
						if(!state.empty()&&!strcmp(state.top(),"return"))
						{
							return val;
						}
						else if(!state.empty()&&!strcmp(state.top(),"break"))
						{
							return val;
						}
						else if(!state.empty()&&!strcmp(state.top(),"continue"))
						{
							return val;
						}
					}
				}
				//data=data_temp;
				function=function_temp;
				if(!data_way.empty())
				{
					for(int i=1;i<data_way.top().size();i++)
					{
						data[data_way.top()[i]].pop();
					}
					data_way.pop();
				}
				if(!array_way.empty())
				{
					for(int i=1;i<array_way.top().size();i++)
					{
						array[array_way.top()[i]].pop();
					}
					array_way.pop();
				}
				break;
			case ForK:
				function_temp=function;
				vi.push_back(0);
				data_way.push(vi);
				array_way.push(vi);
				for(cGen(tree->child[0]);!strcmp(copyString(get_exp(tree->child[1])),"1");cGen(tree->child[2]))
				{
					val=copyString(cGen(tree->child[3]));
					if(!state.empty()&&!strcmp(state.top(),"return"))
					{
						return val;
					}
					else if(!state.empty()&&!strcmp(state.top(),"break"))
					{
						state.pop();
						break;
					}
					else if(!state.empty()&&!strcmp(state.top(),"continue"))
					{
						state.pop();
						continue;
					}
				}
				//data=data_temp;
				function=function_temp;
				if(!data_way.empty())
				{
					for(int i=1;i<data_way.top().size();i++)
					{
						data[data_way.top()[i]].pop();
					}
					data_way.pop();
				}
				if(!array_way.empty())
				{
					for(int i=1;i<array_way.top().size();i++)
					{
						array[array_way.top()[i]].pop();
					} 
					array_way.pop();
				}
				break;
			case WhileK:
				function_temp=function;
				vi.push_back(0);
				data_way.push(vi);
				array_way.push(vi);
				while(!strcmp(copyString(get_exp(tree->child[0])),"1"))
				{
					val=copyString(cGen(tree->child[1]));
					if(!state.empty()&&!strcmp(state.top(),"return"))
					{
						return val;
					}
					else if(!state.empty()&&!strcmp(state.top(),"break"))
					{
						state.pop();
						break;
					}
				}
				//data=data_temp;
				function=function_temp;
				if(!data_way.empty())
				{
					for(int i=1;i<data_way.top().size();i++)
					{
						data[data_way.top()[i]].pop();
					}
					data_way.pop();
				}
				if(!array_way.empty())
				{
					for(int i=1;i<array_way.top().size();i++)
					{
						array[array_way.top()[i]].pop();
					}
					array_way.pop();
				}
				break;
			case DoWhileK:
				function_temp=function;
				vi.push_back(0);
				data_way.push(vi);
				array_way.push(vi);
				do
				{
					val=copyString(cGen(tree->child[0]));
					if(!state.empty()&&!strcmp(state.top(),"return"))
					{
						return val;
					}
					else if(!state.empty()&&!strcmp(state.top(),"break"))
					{
						state.pop();
						break;
					}
				}while(!strcmp(copyString(get_exp(tree->child[1])),"1"));
				//data=data_temp;
				function=function_temp;
				if(!data_way.empty())
				{
					for(int i=1;i<data_way.top().size();i++)
					{
						data[data_way.top()[i]].pop();
					}
					data_way.pop();
				}
				if(!array_way.empty())
				{
					for(int i=1;i<array_way.top().size();i++)
					{
						array[array_way.top()[i]].pop();
					}
					array_way.pop();
				}
				break;
			case StringK:
			case NewPointerK:
			case IntK:
				a=st_lookup(anal->symtab,tree->name);
				if(a!=-1)
				{
					stack<char*> dt;
					dt.push((char*)"0");
					if(data.size()<=a)
					{
						for(;data.size()<=a;)
						{
							data.push_back(dt);
						}
					}
					else
					{
						data[a].push((char*)"0");
					}
					data_way.top().push_back(a);
					if(!strcmp(state.top(),"param"))
					{
						state.pop();
						return inttochar(a);
					}
				}
				break;
			case ArrayNumK:
				return get_exp(tree->child[0]);
				break;
			case NewArrayK:
				a=st_lookup(anal->symtab,tree->name);
				if(a!=-1)
				{
					stack< vector<char*> > dt;
					vector<char*> arr;
					val=copyString(cGen(tree->child[1]));
					if(val!=NULL)
					{
						int len=atoi(val);
						for(int i=0;i<len;i++)
						{
							arr.push_back((char*)"0");
						}
						dt.push(arr);
						if(array.size()<=a)
						{
							for(;array.size()<=a+1;)
							{
								array.push_back(dt);
							}
						}
						else
						{
							array[a].push(arr);
						}
						array_way.top().push_back(a);
					}
					if(!state.empty()&&!strcmp(state.top(),"param"))
					{
						state.pop();
						state.push((char*)"arrayparam");
						return inttochar(a);
					}
				}
				break;
			case FuncStateK:
				buf=tree->name;
				a=-1;
				for(int i=0;i<function.size();i++)
				{
					if(!strcmp(function[i].top()->name,buf))
					{
						a=i;
						break;
					}
				}
				if(!strcmp(state.top(),"param"))
				{
					state.pop();
					state.push((char*)"funcparam");
				}
				return inttochar(a);
			case ArrayAssignK:
				a=st_lookup(anal->symtab,tree->name);
				if(a!=-1)
				{
					int b=atoi(copyString(cGen(tree->child[0])));
					val=copyString(get_exp(tree->child[1]));
					array[a].top()[b]=copyString(val);
				}
				break;
			case PointerAssignK:
				a=atoi(get_exp(tree->child[0]));
				st_lookup_point(anal->symtab,tree->name,a);
				if(data.size()<=a)
				{
					stack<char*> dt;
					dt.push((char*)"0");
					for(;data.size()<=a;)
					{
						data.push_back(dt);
					}
				}
				break;
			case Idk:
				a=st_lookup(anal->symtab,tree->name);
				if(a!=-1)
				{
					return inttochar(a);
				}
				break;
			case InputK:
				if(tree->child[0]->kind==ArrayK)
				{
					a=st_lookup(anal->symtab,tree->child[0]->name);
					int b=atoi(copyString(cGen(tree->child[0]->child[0])));
					buf=new char[1024];
					scanf("%s",buf);
					array[a].top()[b]=copyString(buf);
				}
				else
				{
					a=atoi(cGen(tree->child[0]));
					buf=new char[1024];
					scanf("%s",buf);
					data[a].pop();
					data[a].push(copyString(buf));
				}
				break;
			case AssignK:
				a=st_lookup(anal->symtab,tree->name);
				val=copyString(get_exp(tree->child[0]));
				if(a!=-1)
				{
					data[a].pop();
					data[a].push(copyString(val));
				}
				break;
			case PrintK:
				printf("%s",get_exp(tree->child[0]));
				break;
			case ParamK:
				if(!state.empty()&&!strcmp(state.top(),"call"))
				{
					if(tree->child[0]->kind==ArrayK)
					{
						a=st_lookup(anal->symtab,tree->child[0]->name);
						array_param.insert(array_param.begin(),array[a].top());
						array_param_call_way.push(a);
					}
					else if(tree->child[0]->kind==FuncStateK)
					{
						a=atoi(cGen(tree->child[0]));
						func_param.insert(func_param.begin(),function[a].top());
					}
					else
					{
						param.insert(param.begin(),copyString(get_exp(tree->child[0])));
					}
				}
				else if(!state.empty()&&!strcmp(state.top(),"funccome"))
				{
					state.push((char*)"param");
					a=atoi(cGen(tree->child[0]));
					if(!state.empty()&&!strcmp(state.top(),"arrayparam"))
					{
						//array[a].pop();
						array[a].push(array_param[array_param.size()-2]);
						array_param_way.push(a);
						array_param.erase(array_param.begin()+(array_param.size()-2));
						state.pop();
					}
					else if(!state.empty()&&!strcmp(state.top(),"funcparam"))
					{
						if(a==-1)
						{
							stack<TreeNode*> tn;
							tn.push(func_param[func_param.size()-2]);
							tn.top()->name=tree->child[0]->name;
							function.push_back(tn);
							func_param_way.push(function.size()-1);
						}
						else
						{
							function[a].push(func_param[func_param.size()-2]);
							function[a].top()->name=tree->child[0]->name;
							func_param_way.push(a);
						}
						func_param.erase(func_param.begin()+(func_param.size()-2));
						state.pop();
					}
					else
					{
						data[a].pop();
						data[a].push(copyString(param[param.size()-2]));
						param_way.push(a);
						param.erase(param.begin()+(param.size()-2));
					}
				}
				break;
			case CallK:
				function_temp=function;
				t=NULL;
				state.push((char*)"call");
				cGen(tree->child[0]);
				if(!state.empty()&&!strcmp(state.top(),"call"))
				{
					state.pop();
				}
				for(int i=0;i<function.size();i++)
				{
					if(function[i].top()->kind==FuncComeK&&!strcmp(function[i].top()->name,tree->name))
					{
						t=function[i].top();
						cGen(t->child[0]);
						state.push((char*)"funccome");
						cGen(t->child[1]);
						if(!state.empty()&&!strcmp(state.top(),"funccome"))
						{
							state.pop();
						}
						vector<int> pn;
						while(!param_way.empty())
						{
							pn.push_back(param_way.top());
							param_way.pop();
						}
						params.push(pn);
						pn.clear();
						while(!array_param_way.empty())
						{
							pn.push_back(array_param_way.top());
							array_param_way.pop();
						}
						array_params.push(pn);
						pn.clear();
						while(!func_param_way.empty())
						{
							pn.push_back(func_param_way.top());
							func_param_way.pop();
						}
						func_params.push(pn);
						pn.clear();
						while(!array_param_call_way.empty())
						{
							pn.push_back(array_param_call_way.top());
							array_param_call_way.pop();
						}
						array_params_call_way.push(pn);
						cGen(t->child[2]);
						for(int i=0;i<params.top().size();i++)
						{
							data[params.top()[i]].pop();
						}
						for(int i=0;i<array_params.top().size();i++)
						{
							vector<char*> dt=array[array_params.top()[i]].top();
							array[array_params.top()[i]].pop();
							if(!array_params_call_way.empty()&&!array_params_call_way.top().empty())
							{
								array[array_params_call_way.top()[i]].pop();
								array[array_params_call_way.top()[i]].push(dt);
							}
						}
						for(int i=0;i<func_params.top().size();i++)
						{
							function[func_params.top()[i]].pop();
						}
						if(!params.empty())
						{
							params.pop();
						}
						if(!array_params.empty())
						{
							array_params.pop();
						}
						if(!func_params.empty())
						{
							func_params.pop();
						}
						if(!array_params_call_way.empty())
						{
							array_params_call_way.pop();
						}
						if(!state.empty()&&!strcmp(state.top(),"return"))
						{
							state.pop();
						}
						break;
					}
				}
				function=function_temp;
				break;
			case ReturnK:
				state.push((char*)"return");
				return get_exp(tree->child[0]);
			case BreaKK:
				state.push((char*)"break");
				return (char*)"break";
			case ContinueK:
				state.push((char*)"continue");
				return (char*)"continue";
		}
		return cGen(tree->sibling);
	}
}

char* run(TreeNode* tree,Analyzer* analyzer,char* name)
{
	anal=analyzer;
	TreeNode* t=tree;
	treenode=tree;
	filename=name;
	stack<char*> dt;
	vector<char*> arr;
	stack< vector<char*> > a;
	dt.push((char*)"0");
	arr.push_back((char*)"0");
	a.push(arr);
	data.push_back(dt);
	array.push_back(a);
	state.push((char*)"");
	param.push_back((char*)"0");
	array_param.push_back(arr);
	func_param.push_back(NULL);
	vector<int> vi;
	vi.push_back(0);
	data_way.push(vi);
	array_way.push(vi);
	bool bl=true;
	while(t!=NULL)
	{
		if(t->kind=FuncComeK)
		{
			stack<TreeNode*> tn;
			tn.push(t);
			function.push_back(tn);
		}
		t=t->sibling;
	}
	char* ret=(char*)"0";
	for(int i=0;i<function.size();i++)
	{
		if(function[i].top()->kind==FuncComeK&&!strcmp(function[i].top()->name,"main"))
		{
			t=function[i].top();
			cGen(t->child[0]);
			cGen(t->child[1]);
			ret=cGen(t->child[2]);
			bl=false;
			break;
		}
	}
	if(bl)
	{
		printf("找不到main函数\n");
		exit(-1);
	}
	return ret;
}
