//真值表计算器_栈+后缀1.cpp程序算法基本思想基于
//http://hwdong.com/ds/
//http://blog.csdn.net/iSpeller/article/details/8009628?locationNum=2&fps=1
/*算法原理：将中序表达式转为后序表达式，使得公式易于处理；
　然后对于每种情况一一赋值，再利用后序表达式求出真值。*/
#include<bits/stdc++.h>
using namespace std;

/*本程序会用到的变元*/

struct variable{
	char name;//变元名
	int value;//变元值
};
vector<variable > vec_variable;//变元及值存储vector数组


//枚举优先级
enum { NOT=5, AND=4, OR=3, CONT=2, DBCONT=1 };//离散数学 左孝凌 P10

/*程序所有模块函数*/

//问候语
void prologue(){
	cout<<"输入如\"((p>q)&(q>r))>(p>r)\"的命题公式，程序将会列出真值表。（栈+后缀）"<<endl<<endl;
	cout<<"其中：\"&\"表示合取；\"|\"表示析取；\">\"表示条件；\"!\"表示否定；\"/\"表示双条件。"<<endl;
	cout<<"注意：变元必须是英文字母（大写小写均可），命题公式只能包含英文圆括号！！！"<<endl;
	cout<<"exit 退出。"<<endl<<endl;
}

//判断是否是合法联结词，如果是输出联结词的优先级
int operator_rank(char c){
	switch(c){
		case '!': return NOT;
		case '&': return AND;
		case '|': return OR;
		case '>': return CONT;
		case '/': return DBCONT;
		default: return false;
	}
}

//获得优先级
int get_priority(char a,char b){
	int x=operator_rank(a),y=operator_rank(b);
	if(x > y) return 1;
	else if(x == y) return 0;
	else if(x < y) return -1;
}

//输入检查 输出错误类型 包括 1.exit，2.左右括号匹配，3.变元、联结词合法性，4.联结词使用基本恰当性
bool input_check(string input_str){
	if(input_str == "exit") {cout<<"很高兴与你相遇，再见。"<<endl;exit(0);}
	if(input_str == "continue") return false;
	int left=0,right=0,length=input_str.size();
	for (int i = 0; i < length; ++i){
		char item = input_str[i];char item1 = input_str[i+1];
		if(item == '(') left++;
		else if(item == ')') right++;
		//变元
		else if((item >= 'a' && item <= 'z') || (item >= 'A' && item <= 'Z')){
			if(i != length-1){
				if((item1 >= 'a' && item1 <= 'z') || (item1 >= 'A' && item1 <= 'Z')){
					cout<<"emmmmm，不支持两个及以上的英文字母代表一个变元"<<endl;
					return false;
				}
			}
		}
		//联结词
		else if(operator_rank(item)){
			if(item == '!') {
				if(! ( (item1 >= 'a' && item1 <= 'z') || (item1 >= 'A' && item1 <= 'Z') )){
					cout<<"错误：请检查\"!\""<<endl;
					return false;
				}
			}
			//双目运算符
			else {
				if(i==0){
					cout<<"错误："<<'"'<<item<<'"'<<"是双目运算符，不能放在第一位。"<<endl;
					return false;
				}
				if( !( (item1 >= 'a' && item1 <= 'z') || (item1 >= 'A' && item1 <= 'Z') ) )
					if(item1 != '(' && item1 != '!'){
						cout<<"错误：请检查"<<'"'<<item<<'"'<<"。"<<endl;
						return false;
					}
			}
		}
		//非法输入
		else {
			cout<<"错误：非法输入"<<'"'<<item<<'"'<<"。"<<endl;
			return false;
		}
	}
	//括号匹配检查
	if(left != right){
		cout<<"错误：括号不匹配，"<<"左括号："<<left<<";"<<"右括号："<<right<<"。"<<endl;
		return false;
	}
	return true;
}

//存储变元
void store_variable(string input_str){
	bool flag_letter[2][26];//标记变元字母是否出现过
	memset(flag_letter,0,sizeof(flag_letter));
	vec_variable.clear();
	int length = input_str.size();
	for (int i = 0; i < length; ++i){
		if(input_str[i] == ' ') continue;
		if(input_str[i] >= 'a' && input_str[i] <= 'z' && !flag_letter[0][input_str[i]-'a']){
			vec_variable.push_back((variable){input_str[i]});
			flag_letter[0][input_str[i]-'a']=1;
		}
		else if(input_str[i] >= 'A' && input_str[i] <= 'Z' && !flag_letter[1][input_str[i]-'A']){
			vec_variable.push_back((variable){input_str[i]});
			flag_letter[1][input_str[i]-'A']=1;
		}
	}
}

//输入命题公式字符串并将变元存储进 vec_variable;这个过程不赋值避免混乱
bool get_input(string &input_){
	cout<<" 命题公式： ";
	cin>>input_;
	if(!input_check(input_)) return false;
	store_variable(input_);
	return true;
}

//将 input_str 中序表达式转为后序表达式
string make_postfix_expression(string input_str){
	string postfix_expression;//后序表达式
	postfix_expression.clear();
	stack<char> operator_char_Stack;
	int length = input_str.size();
	for (int i = 0; i < length; ++i){
		char item = input_str[i];
		if(item == ' ') continue;
		//变元直接加入后缀表达式
		if((item >= 'a' && item <= 'z') || (item >= 'A' && item <= 'Z')) postfix_expression += item;
		//左括号直接进入操作符栈
		else if(item == '(') operator_char_Stack.push(item);
		//右括号 则把操作符栈中的元素加入后缀表达式 ， 直到遇到 （  并删掉 （ ；
		else if(item == ')'){
			while(operator_char_Stack.top()!='('){
				postfix_expression += operator_char_Stack.top();
				operator_char_Stack.pop();
			}
			operator_char_Stack.pop();//并删掉 （
		}
		//操作符
		else {
			if ('!' == item) postfix_expression += '0';//通过加入一个不存在的变元，把单目运算符 '!' 当做双目运算符处理
			if(operator_char_Stack.size()==0||operator_char_Stack.top()=='('||get_priority(item,operator_char_Stack.top())>0)
				operator_char_Stack.push(item);
			else{
				while(!(operator_char_Stack.size()==0||get_priority(item,operator_char_Stack.top())>0)){
					postfix_expression += operator_char_Stack.top();
					operator_char_Stack.pop();
				}
				operator_char_Stack.push(item);
			}
		}
	}
	//若操作符栈内仍有元素，把他们依次弹出并放到后序表达式序列尾部
	while(!operator_char_Stack.empty()){
		postfix_expression += operator_char_Stack.top();
		operator_char_Stack.pop();
	}
	return postfix_expression;
	//cout<<"postfix_expression：  "<<postfix_expression<<endl;
}

//一个操作符的计算
bool operator_calculate(int bool1,char character,int bool2){
	if(character == '!') return bool2 ? false : true;
	switch (character){
		case '&': return bool1 && bool2;
		case '|': return bool1 || bool2;
		case '>': return (bool1 && !bool2) ? false : true;
		case '/': return (bool1 == bool2) ? true : false;
		default : cout<<"出现了一个未知的错误：input_check()函数有问题"<<endl;exit(1);
	}
}

//后缀表达式计算并打印真值 默认此时 vec_variable 中的变元均赋值
void postfix_truth_value_calculate(string postfix_expression){
	/*赋值*/
	int length_postfix = postfix_expression.size(),length_vec_variable = vec_variable.size();
	string postfix_value = postfix_expression;
	for (int i = 0; i < length_postfix; ++i){
		char item = postfix_value[i];
		if(operator_rank(item)) continue;
		//是变元 赋值 0 1
		for (int j = 0; j < length_vec_variable; ++j)
			if (item == vec_variable[j].name){
				postfix_value[i] = vec_variable[j].value;
				break;
			}
	}
	/*计算*/
	stack<int > truth;
	while( !truth.empty() )//清栈
      truth.pop();
	for (int i = 0; i < length_postfix; ++i){
		char item = postfix_value[i];
		if(!operator_rank(item)) truth.push(item);
		else {
			int bool1 = truth.top(); truth.pop();
			int bool2 = truth.top(); truth.pop();
			truth.push(operator_calculate(bool2,item,bool1));
		}
	}
	/*打印*/
	if(truth.size()!=1) {
		cout<<"出现了一个未知的错误：truth.size()!=1"<<endl;

		cout<<"truth.size(): "<<truth.size()<<endl;
		int i=0;
		while(!truth.empty()){
			cout<<"i:"<<i++<<"///";
			cout<<truth.top()<<endl;
			truth.pop();
		}
		cout<<endl;
		exit(1);
	}
	cout<<"\t "<<truth.top()<<endl;
}

//打印输出每种情况每个变元的赋值
void Print(){
	/*输出*/
	for (int i = 0; i < vec_variable.size(); ++i)
			cout<<vec_variable[i].value<<" ";
}

//初始变元赋值全假
void init_variable_truth(){
	for (int i = 0; i < vec_variable.size(); ++i)
		vec_variable[i].value = 0;
}

//变元值是否全为真 ： 全为真则结束本次计算
bool variable_truth_end(){
	for (int i = 0; i < vec_variable.size(); ++i)
		if(vec_variable[i].value != 1) return false;
	return true;
}

//切换变元值 即所有之中的一种变元的赋值情况
void one_variable_truth(){
	//将所有变元的值的组合当成一个二进制数，每次加1即可
	int length_vec_variable = vec_variable.size();
	for (int i = 0;i < length_vec_variable; ++i){
		int item = vec_variable[i].value;
		if(item) vec_variable[i].value =0;
		else {
			vec_variable[i].value =1;
			break;
		}
	}
}


/*主函数*/
int main()
{
	prologue();
	while(1){
		string input_str;//输入的命题公式字符串
		if(!get_input(input_str)) continue;
		//核心1 栈转后缀表达式
		string postfix_expression= make_postfix_expression(input_str);//后序表达式字符串
		//打表
		for (int i = 0; i < vec_variable.size(); ++i)
			cout<<vec_variable[i].name<<" ";
		cout<<"\t"<<input_str<<endl<<endl;

		init_variable_truth();
		Print();
		postfix_truth_value_calculate(postfix_expression);
		while(!variable_truth_end()){
			one_variable_truth();
			Print();
			postfix_truth_value_calculate(postfix_expression);
		}
	}
	return 0;
}
//p|(q&r)>!p
//((p>q)&(q>r))>(p>r)

