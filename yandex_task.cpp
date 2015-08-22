#include "stdafx.h"
#include <string>
#include <vector>

using namespace std;

enum MyFunction {VALUE, PLUS, MINUS, MULTIPLICATION, DIVISION};

unsigned long int GetPriority(MyFunction func)
{
	if (func == MyFunction::PLUS) return 0;
	else if (func == MyFunction::MINUS) return 0;
	else if (func == MyFunction::MULTIPLICATION) return 1;
	else if (func == MyFunction::DIVISION) return 1;
	return 0;
}

MyFunction GetFunction(char c)
{
	if (c == '+') return MyFunction::PLUS;
	else if (c == '-') return MyFunction::MINUS;
	else if (c == '*') return MyFunction::MULTIPLICATION;
	else if (c == '/') return MyFunction::DIVISION;
	return MyFunction::VALUE;
}

class MyNode
{
public:
	MyFunction func;
	long int priority = -1;
	MyNode* c1;
	MyNode* c2;
	MyNode* parent;
	double v;

	MyNode()
	{
		c1 = nullptr;
		c2 = nullptr;
		parent = nullptr;
	}

	~MyNode()
	{
		if (c1 != nullptr) delete c1;
		if (c2 != nullptr) delete c2;
	}

	string GetText()
	{
		if (func == MyFunction::VALUE)
		{
			return std::to_string(v);
		}

		string s1;
		string s2;
		if (c1 != nullptr)
		{
			s1 = c1->GetText();
		}

		if (c2 != nullptr)
		{
			s2 = c2->GetText();
		}

		string tfunc;
		if (func == MyFunction::PLUS) tfunc = "+";
		else if (func == MyFunction::MINUS) tfunc = "-";
		else if (func == MyFunction::MULTIPLICATION) tfunc = "*";
		else if (func == MyFunction::DIVISION) tfunc = "/";

		return "(" + s1 + tfunc + s2 + ")";
	}

	double Calc()
	{
		if (func == MyFunction::VALUE)
		{
			return v;
		}

		double v1 = 0;
		double v2 = 0;

		if (c1 != nullptr)
		{
			v1 = c1->Calc();			
		}

		if (c2 != nullptr)
		{
			v2 = c2->Calc();
		}
		
		if (func == MyFunction::PLUS) return v1 + v2;
		else if (func == MyFunction::MINUS) return v1 - v2;
		else if (func == MyFunction::MULTIPLICATION) return v1 * v2;
		else if (func == MyFunction::DIVISION) return v1 / v2;

		throw string("error");
	}
};

class MyTree
{
public:
	MyNode* root;

	MyTree(string& s);

	double Calc()
	{
		return root->Calc();
	}

	string GetText()
	{
		return root->GetText();
	}

	~MyTree()
	{
		delete root;
	}

};

int _tmain(int argc, _TCHAR* argv[])
{
	string str = "5+3*2+4*1*6*7+8+9+10";
	//string str = "(((1+2)*(3+4))+(6-5)*7+8)";
	MyTree t(str);
	double r = t.Calc();
	string res = t.GetText();
	printf("\n%f", r);
	printf("\n%s", res.c_str());
	
	return 0;
}

MyNode* Modify(MyNode* node, double v, MyFunction _func, unsigned long int priority)
{
	if (node->parent != nullptr && priority <= node->parent->priority)
	{
		while (node->parent != nullptr && priority <= node->parent->priority)
			node = node->parent;

		MyNode* np = new MyNode();
		MyNode* nc = new MyNode();

		np->priority = priority;
		np->func = _func;
		np->c1 = node;
		np->c2 = nc;

		nc->parent = np;
		nc->v = v;
		nc->func = MyFunction::VALUE;

		if (node->parent != nullptr)
		{
			node->parent->c2 = np;
			np->parent = node->parent;
		}

		node->parent = np;
		
		return nc;
	}
	else
	{
		node->func = _func;
		node->priority = priority;

		MyNode* nc1 = new MyNode();
		nc1->v = node->v;
		nc1->func = MyFunction::VALUE;
		nc1->parent = node;
		node->c1 = nc1;

		MyNode* nc2 = new MyNode();
		nc2->v = v;
		nc2->func = MyFunction::VALUE;
		nc2->parent = node;
		node->c2 = nc2;

		return nc2;
	}

	return node;
}

MyTree::MyTree(string& s)
{
	MyNode* node = new MyNode();
	
	s += "e";
	string t = "";
	long int depth = 0;
	MyFunction _func = MyFunction::PLUS;
	unsigned long int priority = 0;
	bool first = true;

	for (int i = 0; i < s.length(); i++)
	{
		char c = s[i];

		if (c == '+' || c == '-' || c == '*' || c == '/' || c == 'e')
		{
			double v = atof(t.c_str());
			t = "";

			if (first)
			{
				node->func = MyFunction::VALUE;
				node->v = v;
				first = false;
			}
			else
			{
				node = Modify(node, v, _func, priority);
			}

			if (c == 'e') break;

			_func = GetFunction(c);
			priority = depth * 2 + GetPriority(_func);
		}
		else if (c == '(')
			depth++;
		else if (c == ')')
			depth--;
		else
			t += c;
	}

	root = node;
	while (root->parent != nullptr)
		root = root->parent;
}
