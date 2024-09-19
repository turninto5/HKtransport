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
	//所有线路
	vector<station> all_stations;
	//所有的站点
public:
	File_Editor();

	void split(vector<string>& ret, string str, char sparator);
	//字符串分割函数,i,j索引分隔符，中间的字段是要被分割的字符串 need to verify

	int get_string_length(string str);
	//计算含汉字的字符串长度need to verify

	void separate_line(string line, string& name, string& stations);
	//将获取的字符串分为线路名称，和站点名称大集合,这样或许就不用两个文件了，可以理解为初始化线路结构体

	vector<line>& get_lines();

	vector<station>& get_all_stations();

	void treat_stations(vector<string>& stations);

	void set_lines_index(line& line, vector<station> all_stations);
};

#endif _Editor_h_

