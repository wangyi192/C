#pragma once
#include<iostream>
using namespace std;
char** new_2D(int row, int col) {
	char** c = new char* [row];
	for (int i = 0; i < row; i++) {
		c[i] = new char[col];
	}
	return c;
}
void new_delete(char** c,int row,int col){
	for (int i = 0; i < row; i++) {
		delete[]c[i];
	}
	delete[]c;
}