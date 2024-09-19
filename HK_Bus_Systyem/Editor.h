#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include<algorithm>
using namespace std;
typedef vector<string>::size_type vec_sz;
#define _Editor_h_
#ifdef _Editor_h_

struct line {
	string name;
	vector<string> stations;
	vector<int> index;
};

struct station {
	string name;
	vec_sz index;
};

class File_Editor {
private:
	ifstream bus_lines;
	vector<line> lines;
	//������·
	vector<station> all_stations;
	//���е�վ��
public:
	File_Editor();

	void split(vector<string>& ret, string str, char sparator);
	//�ַ����ָ��,i,j�����ָ������м���ֶ���Ҫ���ָ���ַ��� need to verify

	int get_string_length(string str);
	//���㺬���ֵ��ַ�������need to verify

	void separate_line(string line, string& name, string& stations);
	//����ȡ���ַ�����Ϊ��·���ƣ���վ�����ƴ󼯺�,��������Ͳ��������ļ��ˣ��������Ϊ��ʼ����·�ṹ��

	vector<line>& get_lines();

	vector<station>& get_all_stations();

	void treat_stations(vector<string>& stations);

	void set_lines_index(line& line, vector<station> all_stations);
};

#endif _Editor_h_

