#pragma warning(disable:6385)
#include<iostream>
#include"��ͷ.h"
#include<vector>
using namespace std;
vector<string> PP;
vector<string> QQ;

string new_end = "";	//�����洢���º���ս��
string new_nend = "";	//�����洢���º�ķ��ս��
struct FA {
	int statenum, letternum;//״̬�������ַ�����
	string nends, ends, start;//���ս�����ս������̬
	char* state = new char[statenum];//״̬���飨�±���״̬ת�������±��Ӧ��
	char* letter = new char[letternum];//�ַ�����
	char** stateTrans = new_2D(statenum, letternum);//״̬ת������
	FA(int a, int b) :statenum(a), letternum(b) {}
};
FA dfa(7, 2);
//��ȡ״̬��Ӧ���±꣨��0��ʼ��
int getstatenum(char S) {
	for (int i = 0; i < dfa.statenum; i++) {
		if (dfa.state[i] == S)return i;
	}
	return -1;
}
//��ȡ�ַ���Ӧ���±꣨��0��ʼ��
int getletternum( char c) {
	for (int i = 0; i < dfa.letternum; i++) {
		if (dfa.letter[i] == c)return i;
	}
	return -1;
}
//�ж�str���Ƿ�����ַ�s
bool Find(char s, string str) {
	for (int i = 0; i < (int)str.length(); i++) {
		if (str[i] == s)return true;
	}
	return false;
}
//�ж������ַ����Ƿ����غϲ��ֲ����ص�һ��һ���غ��ַ�
char superposition(string str1, string str2) {
	for (int i = 0; i < (int)str1.length(); i++) {
		if (Find(str1[i], str2))return str1[i];
	}
	return NULL;
}
//�ж�״̬S����PP���ĸ����ϲ����ظü���
string Belong_which(char S, vector<string> LL) {
	string T;
	for (vector<string>::iterator it = LL.begin(); it != LL.end(); it++) {
		T = *it;
		if (Find(S, T))
			return T;
	}
	return "";
}
//�����ַ�a����PP
void spolit( char a) {
	string S = "";
	//���嶯̬����s�����洢���ֺ��״̬��
	string* s = new string[(int)PP.size()];
	//���弯��k�������洢û�ж�Ӧ�ַ���һ��״̬��״̬��
	string k = "",m="";
	string* l = new string[(int)PP.size()];
	//��ʼ��s��l
	for (int i = 0; i < (int)PP.size(); i++) {
		l[i] = "";
		s[i] = "";
	}
	//��������״̬������S���洢����״̬
	for (vector<string>::iterator it = PP.begin(); it != PP.end(); it++) {
		string str = *it;
		S = S + *it;
	}
	//����S�����ַ�a�����֣����ֺ�PP��ԭ�е�״̬��erase�������ֺ��״̬��s�Ž�PP��
	int j;
	for (int i = 0; i < (int)S.length(); i++) {
		for ( j = 0; j < (int)PP.size(); j++) {
			//�ж�S[i]ͨ���ַ�a������״̬����PP���ĸ�����,��������Ӧ����s[i]
			if ((Belong_which(dfa.stateTrans[getstatenum(S[i])][getletternum(a)], PP) == PP[j])&& (Belong_which(S[i], PP) == PP[j])) {
				s[j] = s[j] + S[i];
			}
			else if ((Belong_which(dfa.stateTrans[getstatenum(S[i])][getletternum(a)], PP) == PP[j]) && !(Belong_which(S[i], PP) == PP[j])) {
				m = m + S[i];
			}
		}
		//��S[i]����a��û����һ��״̬��ʱ���������k��
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
	//����PP
	//��վɵ�PP
	PP.clear();
	//�����ֺõ�s[i]װ��PP��
	for (int p = 0; p < j; p++) {
		if (s[p] != "")
			PP.push_back(s[p]);
	}
	for (int o = 0; o < i; o++) {
		if (l[o] != "")
			PP.push_back(l[o]);
	}
	//��k����PP��
	if(k!="")
	PP.push_back(k);
	delete[]s;
}
//�������ش��иü��ϵĴ������
//(str��Ҫ���Ҵ���״̬�ļ��ϣ�S�Ǻϲ�������״̬�ļ��ϣ�ֻҪ����str�а���S����һ��״̬������)
char containerID(string str, string S) {
	return superposition(str, S);
}
//�õ�PP�ϲ����״̬��
string merge() {
	string S = "";
	for (vector<string>::iterator it = PP.begin(); it != PP.end(); it++) {
		string s = *it;
		char a = superposition(s, dfa.start);
		char b = superposition(s, dfa.ends);
		//�����״̬�������г�̬�����ó�̬��������״̬
		if (a != NULL) {
			S = S + a;
			new_nend = new_nend + a;	//���·��ս��
		}
		//����ü���������̬�����ò��ҵ��ĵ�һ����̬��������״̬                                       
		else if (b != NULL) {
			S = S + b;
			new_end = new_end + b;	//˳����ս������
		}
		//����ü��ϼ�û����̬Ҳû�г�̬����ȡ��һ��״̬��Ϊ����״̬
		else {
			S = S + s[0];
			new_nend = new_nend + s[0];	//���·��ս��
		}
	}
	return S;
}
//����dfa
void update() {
	string S = merge();//PP�ϲ����״̬����
	//**�õ����º�״̬ת������
	//��̬����һ����ά���������洢���º��״̬ת������
	char new_stateTrans[100][100];// = new_2D(S.length(), dfa.letternum);
	for (int i = 0; i < dfa.letternum; i++) {	//�ֱ��Ӧÿ���ַ������У�
		for (int j = 0; j < (int)S.length(); j++) {	//�ֱ��Ӧÿ��״̬�����У�
			//��ԭ״̬ת�������У���״̬�������ַ��󵽴��״̬
			char c = dfa.stateTrans[getstatenum( S[j])][getletternum( dfa.letter[i])];
			//����״̬�������ַ�������һ��״̬ʱ
			if (c != 'np') {
				//�õ���״̬��PP����һ��״̬����
				string s = Belong_which(c, PP);
				//�õ���״̬���ϵĴ���״̬
				char k = containerID(s, S);
				//��״̬�������ַ��ͻᵽ��ô���״̬
				new_stateTrans[j][i] = k;	//�õ��µ�״̬ת������
			}
			//��������Ϊnp
			else new_stateTrans[j][i] = c;
		}
	}
	//**

	//**����dfa�����е���Ϣ
	//��̬���ַ������ַ������ñ�
	new_delete(dfa.stateTrans,dfa.statenum,dfa.letternum);
	dfa.statenum = S.length();//��dfa�е�״̬������
	//����״̬��
	for (int i = 0; i < (int)S.length(); i++)
		dfa.state[i] = S[i];
	//����״̬ת������
	cout << "״̬ת������Ϊ��" << endl;
	cout << "		" << dfa.letter[0] << "		" << dfa.letter[1] << endl;
	for (int i = 0; i < dfa.statenum; i++) {
		cout << dfa.state[i] << "		";
		for (int j = 0; j < dfa.letternum; j++) {
			cout << new_stateTrans[i][j]<<"		";
		}
		cout << endl;
	}
	//�����ս��
	dfa.ends = new_end;
	//���·��ս��
	dfa.nends = new_nend;
	//**
}

//���廮��,�ϲ�����dfa���³���Сdfa
void SPOLIT(FA &dfa) {
	
	//PP.push_back(dfa.start);
	PP.push_back(dfa.ends);
	PP.push_back(dfa.nends);
	QQ.push_back(dfa.ends);
	QQ.push_back(dfa.nends);
	//����ÿ���ַ�������PP
	for (int i = 0; i < dfa.letternum; i++) {
		spolit(dfa.letter[i]);
	}
	//����
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
	cout << "��С�����DFA�ĳ�̬Ϊ��" << dfa.start << endl << "��̬Ϊ��" << dfa.ends << endl << "���ս��Ϊ��" << dfa.nends <<endl<< "�ַ�Ϊ��" << dfa.letter[0] << " " << dfa.letter[1] << endl;
	delete[]dfa.state;
	delete[]dfa.letter;
	return 1;
}
