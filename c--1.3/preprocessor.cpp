#include "Include/preprocessor.h"

Preprocessor* preprocessor_init()
{
	Preprocessor* prep = new Preprocessor;
	prep->blockstyle = INDENT;
	return prep;
}

string preprocessor(Preprocessor* prep,string content)
{
	string result="";
	int i = 0;
	while (i < content.size())
	{
		string define_name = search_define(content[i]);
		if (content[i] == '"')	//字符串不进行任何操作
		{
			result += content[i];
			i++;
			while (i < content.size() && content[i] != '"')
			{
				result += content[i];
				i++;
			}
			result += content[i];
			i++;
		}
		else if (content[i] == '\'')	//字符串不进行任何操作
		{
			result += content[i];
			i++;
			while (i < content.size() && content[i] != '\'')
			{
				result += content[i];
				i++;
			}
			result += content[i];
			i++;
		}
		else if (content[i] == '#')
		{
			string pre_code = "";
			i++;
			while (i < content.size() && content[i]!= '\n')
			{
				pre_code += content[i];
				i++;
			}
			string preresult=dealprecode(prep, result, content,i, pre_code);
			if (preresult != "")
			{
				result += preresult;
			}
		}
		else if (define_name != "")
		{
			string name = "";
			for (int j = 0; j < define_name.size(); j++)
			{
				name += content[i];
				i++;
			}
			if (define_name == name&& !((content[i] >= 'a' && content[i] <= 'z') || (content[i] >= 'A' && content[i] <= 'Z') || content[i] == '_'))
			{
				result += define[define_name];
			}
			else
			{
				result += name;
			}
		}
		else
		{
			result += content[i];
			i++;
		}
	}
	return result;
}

string dealprecode(Preprocessor* prep,string& result, string content, int& p,string precode)
{
	int pos = 0;
	int replace = 0;
	string arg1 = getnextprediv(precode, pos);
	if (arg1 == "define")
	{
		string arg2 = getnextprediv(precode, pos);
		while (pos < precode.size() && (precode[pos] == '\n' || precode[pos] == ' ' || precode[pos] == '\t'))
		{
			pos++;
		}
		string arg3 = "";
		while (pos < precode.size() && precode[pos] != '\n')
		{
			arg3 += precode[pos];
			pos++;
		}
		define[arg2] = arg3;
	}
	else if (arg1 == "undef")
	{
		string arg2= getnextprediv(precode, pos);
		define.erase(arg2);
	}
	else if (arg1 == "include")
	{
		while (pos < precode.size() && (precode[pos] == '\n' || precode[pos] == '\t' || precode[pos] == ' '))
		{
			pos++;
		}
		string filename = "";
		if (precode[pos] == '<')	//从默认目录包含
		{
			pos++;
			while (pos < precode.size() && (precode[pos] == '\n' || precode[pos] == '\t' || precode[pos] == ' '))
			{
				pos++;
			}
			while (pos < precode.size() && precode[pos] != '\n' && precode[pos] != '>')
			{
				filename += precode[pos];
				pos++;
			}
			filename = include_path + '/' + filename;
		}
		else if (precode[pos] == '"')
		{
			pos++;
			while (pos < precode.size() && (precode[pos] == '\n' || precode[pos] == '\t' || precode[pos] == ' '))
			{
				pos++;
			}
			while (pos < precode.size() && precode[pos] != '\n' && precode[pos] != '"')
			{
				filename += precode[pos];
				pos++;
			}
		}
		if (filename != "")
		{
			int i = result.size() - 1;
			/*回到预处理指令所在行的行首*/
			while (i >= 0 && result[i] != '\n')
			{
				i--;
			}
			i++;
			/*获取预处理器前的缩进*/
			string indent = "";
			while (i < result.size() && (result[i] == ' ' || result[i] == '\t'))
			{
				indent+=result[i];
				i++;
			}
			ifstream t(filename);
			stringstream buffer;
			buffer << t.rdbuf();
			string content(buffer.str());
			/*给文件每一行加上缩进*/
			string _pre_file = preprocessor(prep,content);
			string pre_file = "";
			for (i = 0; i < _pre_file.size(); i++)
			{
				pre_file+=_pre_file[i];
				if (_pre_file[i] == '\n')
				{
					pre_file += indent;
				}
			}
			t.close();
			return pre_file;
		}
	}
	else if (arg1=="ifdef")
	{
		string arg2 = getnextprediv(precode, pos);
		if (define.find(arg2) != define.end())
		{
			defineflag = 1;
		}
		else
		{
			replace = 1;
		}
	}
	else if (arg1=="ifndef")
	{
		string arg2 = getnextprediv(precode, pos);
		if (define.find(arg2) == define.end())
		{
			defineflag = 1;
		}
		else
		{
			replace = 1;
		}
	}
	else if (arg1=="elif")
	{
		string arg2 = getnextprediv(precode, pos);
		if (!defineflag && define.find(arg2) != define.end())
		{
			defineflag = 1;
		}
		else
		{
			replace = 1;
		}
	}
	else if (arg1=="else")
	{
		if (defineflag)
		{
			replace = 1;
		}
	}
	else if (arg1=="endif")
	{
		defineflag = 0;
	}
	else if (arg1=="blockstyle")
	{
		string arg2 = getnextprediv(precode, pos);
		if (arg2=="indent")
		{
			prep->blockstyle = INDENT;
		}
		else if (arg2=="beginend")
		{
			prep->blockstyle = BEGINEND;
		}
		else if (arg2=="braces")
		{
			prep->blockstyle = BRACES;
		}
		else if (arg2 == "end")
		{
			prep->blockstyle = END;
		}
	}
	if (replace)
	{
		while (p<content.size() && content[p] != '#')
		{
			p++;
			/*判断是否是条件预处理指令*/
			if (content[p] == '#')
			{
				string pc = "";
				p++;
				while(p < content.size() && (content[p] == ' ' || content[p] == '\n' || content[p] == '\t'))
				{
					p++;
				}
				while (p<content.size() && content[p] != ' ' && content[p] != '\n' && content[p] != '\t')
				{
					pc += content[p];
					p++;
				}
				/*如果是*/
				if (pc=="ifdef" || pc=="ifndef" || pc=="elif" || pc=="else" || pc=="endif")
				{
					p--;
					for (int i = pc.size() - 1; i >= 0; i--)
					{
						p--;
					}
				}
			}
		}
	}
	return "";
}

string getnextprediv(string precode, int& pos)
{
	while (pos < precode.size() && (precode[pos] == '\n' || precode[pos] == ' ' || precode[pos] == '\t'))
	{
		pos++;
	}
	string result = "";
	result += precode[pos];
	pos++;
	while (pos < precode.size() && !(precode[pos] == '\n' || precode[pos] == ' ' || precode[pos] == '\t'||!isName(precode[pos])))
	{
		result += precode[pos];
		pos++;
	}
	return result;
}

/*查找一个字符是否在宏里出现过,
如果有,那么该字符与后面组成的字符串可能是一个宏*/
string search_define(char c)
{
	map<string, string>::iterator iter;
	iter = define.begin();
	while (iter != define.end())
	{
		if (iter->first[0] == c)return iter->first;
		iter++;
	}
	return "";
}