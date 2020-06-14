//真值表计算器_二叉树+递归2.cpp程序算法基本思想基于
//http://hwdong.com/ds/
//http://blog.csdn.net/patrick_lyle/article/details/63250699
/*算法原理：将命题公式拆解成可以计算的简单子问题，拆解过程由通过二叉树存储；
　然后对于每种情况一一赋值，再递归二叉树求出真值。*/
#include<bits/stdc++.h>
using namespace std;

/*本程序会用到的变量*/

struct variable{
	char name;//变元名
	int value;//变元值
};
vector<variable > vec_variable;//变元及值存储vector数组

struct Tree_Node //二叉树节点
{
	char data;
	struct Tree_Node *left, *right;
};

//枚举优先级
enum { NOT=5, AND=4, OR=3, CONT=2, DBCONT=1 };//离散数学 左孝凌 P10


/*程序所有模块函数*/

//问候语
void prologue(){
	cout<<"输入如\"((p>q)&(q>r))>(p>r)\"的命题公式，程序将会列出真值表。(二叉树+递归)"<<endl<<endl;
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

//二叉树建树
void Create_tree(string s,int x,int y,Tree_Node* &T){
    //printf("%d %d\n",x,y);
    if(!(T= new Tree_Node) ){
		throw "错误：建树";
		return;
	}
    if(y-x==1 || y-x==0){
        T->left = T->right=0;
        y-x==1?T->data =s[x]:T->data ='0';
        return ;
    }
    int c1 =-1,p=0;
    char item;
    for(int i=x; i<y; i++){
    	if(s[i]=='(') p++;
    	else if(s[i]==')') p--;
    	if(!operator_rank(s[i])) continue;
    	if(!p && (c1==-1 || get_priority(item , s[i])>0) ) {
    		c1=i;
    		item = s[i];
		}
    }
    if(c1<0){//????±í′?ê?±?ò???à¨o?à¨?eà′
		Create_tree(s,x+1,y-1,T);
		return ;
	}
    Create_tree(s,x,c1,T->left);
    Create_tree(s,c1+1,y,T->right);
    T->data =s[c1];
    return ;
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

//递归计算真值
int recursion_truth_value_calculate(Tree_Node* T){
	char item = T->data;
	if(item == '0') return -1;
	if( (item >= 'a' && item <= 'z') || (item >= 'A' && item <= 'Z') ){
		int length_vec_variable = vec_variable.size();
		for (int j = 0; j < length_vec_variable; ++j)
			if (item == vec_variable[j].name)
				return vec_variable[j].value;
	}
	return operator_calculate(recursion_truth_value_calculate(T->left),item,recursion_truth_value_calculate(T->right));
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
		//核心2 二叉树递归计算
		Tree_Node* Tree = 0;
		Create_tree(input_str,0,input_str.size(),Tree);
		//打表
		for (int i = 0; i < vec_variable.size(); ++i)
			cout<<vec_variable[i].name<<" ";
		cout<<"\t"<<input_str<<endl<<endl;

		init_variable_truth();
		Print();
		cout<<"\t "<<recursion_truth_value_calculate(Tree)<<endl;
		while(!variable_truth_end()){
			one_variable_truth();
			Print();
			cout<<"\t "<<recursion_truth_value_calculate(Tree)<<endl;
		}

	}
	return 0;
}
//p|(q&r)>!p
//((p>q)&(q>r))>(p>r)

