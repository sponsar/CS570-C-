#include<vector>
#include<iostream>
#include<stack>
#include<string>
#include<fstream>
using namespace std;
//#define OP 0
//#define NUM 1

/*
This project is created by Xinyao Huang, and it includes all the extra credit.
*/

class Symbol//其实就等于Node
{
protected:
	//int kind;//OP or NUM
public:
	Symbol()
	{

	}
	virtual ~Symbol()
	{

	}
	/*
	int whatkind()
	{
		return kind;
	}
	*/
};
class Operand :public Symbol //represents the number
{
private:
	double value;
public:
	Operand(double value)//要不要创建move构造函数
	{
		//kind = NUM;
		this->value = value;
	}
	~Operand()
	{

	}
	//重载运算符
	double getvalue() const
	{
		return value;
	}
	Operand& operator=(const Operand& a1)
	{
		if (this != &a1)
		{
			this->value = a1.value;
		}
		return *this;
	}
	friend Operand operator +(const Operand& a1, const Operand& a2)
	{
		return Operand(a1.value + a2.value);
	}
	friend Operand operator -(const Operand& a1, const Operand& a2)
	{
		return Operand(a1.value - a2.value);
	}
	friend Operand operator *(const Operand& a1, const Operand& a2)
	{
		return Operand(a1.value * a2.value);
	}
	friend Operand operator /(const Operand& a1, const Operand& a2)
	{
		if (a2.value == 0)
			throw logic_error("can not divide by zero!");
		return Operand(a1.value / a2.value);
	}
	friend Operand operator %(const Operand& a1, const Operand& a2)
	{
		if (a2.value == 0)
			throw logic_error("can not modulo by zero!");
		return Operand(fmod(a1.value, a2.value));
	}
	friend Operand pow(const Operand& a1, const Operand& a2)
	{
		return Operand(pow(a1.value, a2.value));
	}
};
class Operator :public Symbol//base class of operator
{
protected:
	string symbol;
public:
	Operator()
	{
		//kind = OP;
	}
	~Operator()
	{

	}//删不删
	string getter()
	{
		return symbol;
	}
	virtual Operand compute(const Operand& a1, const Operand& a2) = 0;//这个函数是不是可有可无  多态性????
};
class Add :public Operator
{
public:
	Add() //:Operator()  kind initialized
	{
		symbol = "+";
	}
	Operand compute(const Operand& a1, const Operand& a2)
	{
		return a1 + a2;
	}
};
class Minus :public Operator
{
public:
	Minus()
	{
		symbol = "-";
	}
	Operand compute(const Operand& a1, const Operand& a2)
	{
		return a1 - a2;
	}
};
class Multiply :public Operator
{
public:
	Multiply()
	{
		symbol = "*";
	}
	Operand compute(const Operand& a1, const Operand& a2)
	{
		return a1 * a2;
	}
};
class Divide :public Operator
{
public:
	Divide()
	{
		symbol = "/";
	}
	Operand compute(const Operand& a1, const Operand& a2)
	{
		return a1 / a2;
	}
};
class Modulo :public Operator
{
public:
	Modulo()
	{
		symbol = "%";
	}
	Operand compute(const Operand& a1, const Operand& a2)
	{
		return a1 % a2;
	}
};
class Pow :public Operator
{
public:
	Pow()
	{
		symbol = "pow";
	}
	Operand compute(const Operand& a1, const Operand& a2)
	{
		return pow(a1, a2);
	}
};


double data_processing(const vector<Symbol*>& vect)//use stack
{
	Symbol* ptr;
	stack<Operand> stack;
	for (unsigned int i = 0; i < vect.size(); i++)
	{
		ptr = vect[i];
		Operand* ptr_operand = dynamic_cast<Operand*>(ptr);
		if (ptr_operand == nullptr)//operator
		{
				Operator* ptr_operator = dynamic_cast<Operator*>(ptr);
				//判断stack里面有没有两个Operands
				if (stack.size() < 2)
					throw logic_error("SYNTAX ERROR");
				
				//正常情况
				Operand operand_1 = stack.top();
				stack.pop();
				Operand operand_2 = stack.top();
				stack.pop();
				Operand operand_new = ptr_operator->compute(operand_2, operand_1);//也会throw
				stack.push(operand_new);
		}
		else//operand
			stack.push(*ptr_operand);//往stack里面打
			
	}
	if (stack.size() != 1)//如果stack还剩下多个operand，就throw "SYNTAX ERROR"
		throw logic_error("SYNTAX ERROR");
	else
		return stack.top().getvalue();//stack最后一个数要不要处理
	
}



Symbol* creatOperand(string value)
{
	double val;
	val = stod(value);//将str变为数字,还可以用istringstream>>
	return new Operand(val);
}
Symbol* creatOperator(char a)
{
	switch (a)
	{
	case '+':return new Add(); break;
	case '-':return new Minus(); break;
	case '*':return new Multiply(); break;
	case '/':return new Divide(); break;
	default:return new Modulo(); //'%'
	}
}
Symbol* creatOperator(string sym)
{
	return new Pow();
}


void Solution(string RPN)//这里是cout一行
{
	vector<Symbol*> vect;
	string num_str = "";
	string pow_str = "";
	double value = 0;
	bool pow_count = false;
	bool num_count = false;
	Symbol* ptr = nullptr;
	for (unsigned int i = 0; i < RPN.length(); i++)
	{
		if ((RPN[i] >= '0' && RPN[i] <= '9') || RPN[i] == '.')//0-9  ????????????要考虑0的特殊情况还有.的特殊情况
		{
			if (pow_count)
			{
				cout << "SYNTAX ERROR" << endl;
				return;
			}
			num_count = true;
			num_str += RPN[i];
		}
		else//不等于数字的情况
		{
			if (!pow_count&&num_count)//将前面的数字打包，然后push进stack
			{
				if (num_str == ".")
				{
					cout << "SYNTAX ERROR" << endl;
					return;
				}
				ptr = creatOperand(num_str);//??????????最后记得要delete
				//ptr = new Operand(stod(num_str));
				num_str = "";
				num_count = false;
				vect.push_back(ptr);//打入vector
			}
			
			if (RPN[i] == '+' || RPN[i] == '-' || RPN[i] == '*' || RPN[i] == '/' || RPN[i] == '%' || RPN[i] == 32)//????????????????????????????????空格
			{
				if (RPN[i] == 32)
				{
					continue;
				}
				if (pow_count)
				{
					cout << "SYNTAX ERROR" << endl;
					return;
				}
				ptr = creatOperator(RPN[i]);
				vect.push_back(ptr);//打入vector
			}
			else//字符凑齐pow
			{
				pow_count = true;
				pow_str += RPN[i];
				if (pow_str == "pow")
				{
					ptr = creatOperator(pow_str);
					vect.push_back(ptr);//打入vector
					pow_count = false;
					pow_str = "";
					
				}
				else if (i == RPN.length() - 1)
				{
					cout << "SYNTAX ERROR" << endl;
					return;
				}
			}
		}
	}
	if (num_count)
	{
		ptr = creatOperand(num_str);
		num_str = "";
		num_count = false;
		vect.push_back(ptr);//打入vector
	}
	try
	{
		cout << data_processing(vect) << endl;//要不要把RPN也输出来
	}
	catch (exception& ex)//cout << logic.what() << endl;
	{
		cout << ex.what() << endl;
	}
	//delete掉所有动态内存
	for (unsigned int i = 0; i < vect.size(); i++)
		delete vect[i];
}

int main()
{
	cout << "Please input the location of your file: ";
	string location;
	cin >> location;
	
	ifstream infile(location);//这里要判断一下打不开的情况
	if (!infile)
	{
		cerr << "No file match!" << endl;
		exit(1);
	}
	string RPN;//each line

	while (getline(infile, RPN))
	{
		if (RPN.length() > 0)
			Solution(RPN);
	}
	infile.close();
	return 0;
	
}


