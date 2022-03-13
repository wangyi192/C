#pragma warning(disable:6385)
#include<iostream>
#include"标头.h"
#include<vector>
using namespace std;
vector<string> PP;
vector<string> QQ;

string new_end = "";	//用来存储更新后的终结符
string new_nend = "";	//用来存储更新后的非终结符
struct FA {
	int statenum, letternum;//状态数量，字符数量
	string nends, ends, start;//非终结符，终结符，初态
	char* state = new char[statenum];//状态数组（下标与状态转换矩阵下标对应）
	char* letter = new char[letternum];//字符数组
	char** stateTrans = new_2D(statenum, letternum);//状态转换矩阵
	FA(int a, int b) :statenum(a), letternum(b) {}
};
FA dfa(7, 2);
//获取状态对应的下标（从0开始）
int getstatenum(char S) {
	for (int i = 0; i < dfa.statenum; i++) {
		if (dfa.state[i] == S)return i;
	}
	return -1;
}
//获取字符对应的下标（从0开始）
int getletternum( char c) {
	for (int i = 0; i < dfa.letternum; i++) {
		if (dfa.letter[i] == c)return i;
	}
	return -1;
}
//判断str中是否存在字符s
bool Find(char s, string str) {
	for (int i = 0; i < (int)str.length(); i++) {
		if (str[i] == s)return true;
	}
	return false;
}
//判断两个字符串是否有重合部分并返回第一个一个重合字符
char superposition(string str1, string str2) {
	for (int i = 0; i < (int)str1.length(); i++) {
		if (Find(str1[i], str2))return str1[i];
	}
	return NULL;
}
//判断状态S属于PP中哪个集合并返回该集合
string Belong_which(char S, vector<string> LL) {
	string T;
	for (vector<string>::iterator it = LL.begin(); it != LL.end(); it++) {
		T = *it;
		if (Find(S, T))
			return T;
	}
	return "";
}
//按照字符a划分PP
void spolit( char a) {
	string S = "";
	//定义动态数组s用来存储划分后的状态集
	string* s = new string[(int)PP.size()];
	//定义集合k，用来存储没有对应字符下一个状态的状态集
	string k = "",m="";
	string* l = new string[(int)PP.size()];
	//初始化s和l
	for (int i = 0; i < (int)PP.size(); i++) {
		l[i] = "";
		s[i] = "";
	}
	//遍历所有状态集，用S来存储所有状态
	for (vector<string>::iterator it = PP.begin(); it != PP.end(); it++) {
		string str = *it;
		S = S + *it;
	}
	//遍历S，用字符a来划分，划分后将PP中原有的状态集erase，将划分后的状态集s放进PP；
	int j;
	for (int i = 0; i < (int)S.length(); i++) {
		for ( j = 0; j < (int)PP.size(); j++) {
			//判断S[i]通过字符a后进入的状态属于PP的哪个集合,将其放入对应的是s[i]
			if ((Belong_which(dfa.stateTrans[getstatenum(S[i])][getletternum(a)], PP) == PP[j])&& (Belong_which(S[i], PP) == PP[j])) {
				s[j] = s[j] + S[i];
			}
			else if ((Belong_which(dfa.stateTrans[getstatenum(S[i])][getletternum(a)], PP) == PP[j]) && !(Belong_which(S[i], PP) == PP[j])) {
				m = m + S[i];
			}
		}
		//当S[i]经过a后没有下一个状态集时，将其放入k中
		if (dfa.stateTrans[getstatenum(S[i])][getletternum(a)] == 'np') {
			k = k + S[i];
		}
	}
	int i;
	for (int o = 0; o < m.length(); o++) {
		for (i = 0; i < (int)PP.size(); i++) {
			if ((Belong_which(m[o], PP) == PP[i]))
				l[i] = l[i] + m[o];
		}
	}
	//更新PP
	//清空旧的PP
	PP.clear();
	//将划分好的s[i]装进PP中
	for (int p = 0; p < j; p++) {
		if (s[p] != "")
			PP.push_back(s[p]);
	}
	for (int o = 0; o < i; o++) {
		if (l[o] != "")
			PP.push_back(l[o]);
	}
	//将k放入PP中
	if(k!="")
	PP.push_back(k);
	delete[]s;
}
//用来返回存有该集合的代表符号
//(str是要查找代表状态的集合，S是合并后所有状态的集合，只要查找str中包含S中哪一个状态就行了)
char containerID(string str, string S) {
	return superposition(str, S);
}
//得到PP合并后的状态集
string merge() {
	string S = "";
	for (vector<string>::iterator it = PP.begin(); it != PP.end(); it++) {
		string s = *it;
		char a = superposition(s, dfa.start);
		char b = superposition(s, dfa.ends);
		//如果该状态集合里有初态，就用初态来当代表状态
		if (a != NULL) {
			S = S + a;
			new_nend = new_nend + a;	//更新非终结符
		}
		//如果该集合中有终态，则用查找到的第一个终态来当代表状态                                       
		else if (b != NULL) {
			S = S + b;
			new_end = new_end + b;	//顺便把终结符更新
		}
		//如果该集合既没有终态也没有初态，就取第一个状态作为代表状态
		else {
			S = S + s[0];
			new_nend = new_nend + s[0];	//更新非终结符
		}
	}
	return S;
}
//更新dfa
void update() {
	string S = merge();//PP合并后的状态集合
	//**得到更新后状态转换矩阵
	//动态分配一个二维数组用来存储更新后的状态转换矩阵
	char new_stateTrans[100][100];// = new_2D(S.length(), dfa.letternum);
	for (int i = 0; i < dfa.letternum; i++) {	//分别对应每个字符（即列）
		for (int j = 0; j < (int)S.length(); j++) {	//分别对应每个状态（即行）
			//在原状态转换矩阵中，该状态经过该字符后到达的状态
			char c = dfa.stateTrans[getstatenum( S[j])][getletternum( dfa.letter[i])];
			//当该状态经过该字符存在下一个状态时
			if (c != 'np') {
				//得到该状态在PP中哪一个状态集合
				string s = Belong_which(c, PP);
				//得到该状态集合的代表状态
				char k = containerID(s, S);
				//该状态经过该字符就会到达该代表状态
				new_stateTrans[j][i] = k;	//得到新的状态转换矩阵
			}
			//否则将其设为np
			else new_stateTrans[j][i] = c;
		}
	}
	//**

	//**更新dfa中所有的信息
	//初态、字符数和字符集不用变
	new_delete(dfa.stateTrans,dfa.statenum,dfa.letternum);
	dfa.statenum = S.length();//把dfa中的状态数更新
	//更新状态集
	for (int i = 0; i < (int)S.length(); i++)
		dfa.state[i] = S[i];
	//更新状态转换矩阵
	cout << "状态转换矩阵为：" << endl;
	cout << "		" << dfa.letter[0] << "		" << dfa.letter[1] << endl;
	for (int i = 0; i < dfa.statenum; i++) {
		cout << dfa.state[i] << "		";
		for (int j = 0; j < dfa.letternum; j++) {
			cout << new_stateTrans[i][j]<<"		";
		}
		cout << endl;
	}
	//更新终结符
	dfa.ends = new_end;
	//更新非终结符
	dfa.nends = new_nend;
	//**
}

//整体划分,合并，将dfa更新成最小dfa
void SPOLIT(FA &dfa) {
	
	//PP.push_back(dfa.start);
	PP.push_back(dfa.ends);
	PP.push_back(dfa.nends);
	QQ.push_back(dfa.ends);
	QQ.push_back(dfa.nends);
	//遍历每个字符，划分PP
	for (int i = 0; i < dfa.letternum; i++) {
		spolit(dfa.letter[i]);
	}
	//更新
	update();
}
int main() {
	dfa.ends = "CDEF";
	dfa.letter[0] = 'a';
	dfa.letter[1] = 'b';
	dfa.nends = "SAB";
	dfa.start = "S";
	dfa.state[0] = 'S';
	dfa.state[1] = 'A';
	dfa.state[2] = 'B';
	dfa.state[3] = 'C';
	dfa.state[4] = 'D';
	dfa.state[5] = 'E';
	dfa.state[6] = 'F';
	dfa.stateTrans[0][0] = 'A';
	dfa.stateTrans[0][1] = 'B';
	dfa.stateTrans[1][0] = 'C';
	dfa.stateTrans[1][1] = 'B';
	dfa.stateTrans[2][0] = 'A';
	dfa.stateTrans[2][1] = 'D';
	dfa.stateTrans[3][0] = 'C';
	dfa.stateTrans[3][1] = 'E';
	dfa.stateTrans[4][0] = 'F';
	dfa.stateTrans[4][1] = 'D';
	dfa.stateTrans[5][0] = 'F';
	dfa.stateTrans[5][1] = 'D';
	dfa.stateTrans[6][0] = 'C';
	dfa.stateTrans[6][1] = 'E';
	SPOLIT(dfa);
	cout << "最小化后的DFA的初态为：" << dfa.start << endl << "终态为：" << dfa.ends << endl << "非终结符为：" << dfa.nends <<endl<< "字符为：" << dfa.letter[0] << " " << dfa.letter[1] << endl;
	delete[]dfa.state;
	delete[]dfa.letter;
	return 1;
}
