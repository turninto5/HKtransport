#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include<algorithm>
#include "Editor.h"
using namespace std;

typedef vector<string>::size_type vec_sz;
typedef string::size_type string_size;
typedef vector<line>::size_type line_size;
typedef vector<station>::size_type station_size;

File_Editor::File_Editor() {
	line single_line;
	bus_lines.open("BusLines.txt", ios::in);//初始化线路文件 
	if (!bus_lines.is_open()) {
		cout << "文件读取失败" << endl;
	}
	//文件读取可能存在异常 need to modify
	string line;
	string line_name;
	string line_stations;
	int index = 0;
	

	vector<string> stations_name;
	station sta;

	while (getline(bus_lines, line)) {//获取一行
		separate_line(line, line_name, line_stations);//拆分
		single_line.name = line_name;//初始化线路名称																																																																																																																																																																									
		vector <string> line_stations_vec;
		split(line_stations_vec, line_stations, ',');
		single_line.stations = line_stations_vec;

		vector<int> index_for_line(line_stations_vec.size(),0);
		single_line.index = index_for_line;
		
		lines.push_back(single_line);

		for (vec_sz i = 0; i < line_stations_vec.size(); i++)
			stations_name.push_back(line_stations_vec[i]);
	}
	treat_stations(stations_name);
	for (vec_sz i = 0; i < stations_name.size(); i++) {
		sta.name = stations_name[i];
		sta.index = i;
		all_stations.push_back(sta);
	}

	for (line_size i = 0; i < lines.size(); i++) {
		set_lines_index(lines[i], all_stations);
	}
}

void File_Editor::split(vector<string>& stations, string station_str, char separator) {
	string_size i = 0;
	string_size j = 0;
	while (i < station_str.size()) {
		while (j < station_str.size()) {
			if (station_str[j] == separator) {
				stations.push_back(station_str.substr(i, j - i));
				j++;
				i = j;

			}
			else
				j++;
		}
	}
}

void File_Editor::separate_line(string line, string& name, string& stations) {
	typedef string::size_type string_size;
	string_size i = 0;
	while (i != line.size()) {
		if (line[i + 1] == ':') {
			name = line.substr(0, i + 1);
			stations = line.substr(i + 2, line.size() - i);//冒号之后是站点大集合，取出来赋给站点大集合,我还不要冒号
		}
		i++;
	}//找到冒号，然后把冒号前面的线路名称截掉赋值给线路名称

}

int File_Editor::get_string_length(string str) {
	int num = 0;
	int length = str.length();
	int i = 0;
	while (i < length)
	{
		//如果是汉字则移到下一个
		if (str[i] & 0x80)
		{
			i++;
		}
		num++;
		i++;
	}
	return length;
}

vector<line>& File_Editor::get_lines() {
	return lines;
}

vector<station>& File_Editor::get_all_stations() {
	return all_stations;
}

void File_Editor::treat_stations(vector<string>& stations) {
	sort(stations.begin(), stations.end());
	stations.erase(unique(stations.begin(), stations.end()), stations.end());
}

void File_Editor::set_lines_index(line& line, vector<station> all_stations) {
	for (vec_sz i = 0; i < line.stations.size(); i++) {
		for (station_size j = 0; j < all_stations.size(); j++) {
			if (all_stations[j].name == line.stations[i]) {
				line.index[i] = all_stations[j].index;
			}
		}
	}
}