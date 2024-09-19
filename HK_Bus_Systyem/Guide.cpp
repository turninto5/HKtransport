#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include<algorithm>
#include "Editor.h"
#include "Map.h"
#include "Guide.h"

using namespace std;
typedef vector<line>::size_type line_size;
typedef vector<string>::size_type string_size;
typedef vector<int>::size_type int_size;
typedef vector<station>::size_type station_size;

Guide::Guide() {
	distance = 0;
	transfer_times = 0;
}

bool Guide::search_for_fewest_stations(string start, string end, File_Editor editor, Map map) {
	vector<station> route;
	mat adjacency = map.get_adjacency_matrix();
	station start_sta;
	station end_sta;
	if (str_trans_sta(start_sta, start, editor.get_all_stations()) && str_trans_sta(end_sta, end, editor.get_all_stations())) {
		map.Dijkstra_algorithm(adjacency, start_sta, end_sta, editor.get_all_stations(), distance, route);
		cout << start << "到" << end << "最短路径是" << distance << endl;
		return true;
	}
	else {
		cout << "输入错误，没有这样的站点名字！" << endl;
		return false;
	}
		
	
	
	//output(route);
	//查找目标路径
}

bool Guide::search_for_fewest_transfer_times(string start, string end, File_Editor editor, Map map) {
	line start_line;
	line end_line;
	station start_sta;
	station end_sta;

	int distance;
	line ob_line;//用于检测
	line ob_line1;
	line ob_line2;

	vector<bool> visit(editor.get_lines().size(), false);
	if (str_trans_sta(start_sta, start, editor.get_all_stations()) && str_trans_sta(end_sta, end, editor.get_all_stations())) {
		search_line(editor.get_lines(), start_sta, start_line);
		search_line(editor.get_lines(), end_sta, end_line);
		bool done = false;
		//线路初始化

		//第0次检测
		if (station_on_line(start_sta, end_sta, editor.get_lines(),ob_line)) {
			measure_distance(distance, start_sta, end_sta,ob_line);//在一条线上的站点需要展示出来，直线测距离函数
			cout << "直达，无需换乘" << endl;//起点终点在一条线路上，换乘0次
		}
		else {
			//终点线标记
			int_size end_index;
			search_lines_index(editor.get_lines(), end_index, end_line);
			visit[end_index] = true;


			//寻找重合点
			vector<station> crossing;
			search_crossing(end_line, crossing, editor.get_lines(), editor.get_all_stations());
			//第1次检测
			for (station_size i = 0; i < crossing.size(); i++) {
				if (station_on_line(start_sta, crossing[i], editor.get_lines(), ob_line)&& station_on_line(end_sta, crossing[i], editor.get_lines(), ob_line1)) {
					done = true;
					int overall_distance = 0;
					measure_distance(distance, start_sta, crossing[i], ob_line);
					cout << crossing[i].name << " 换乘一次，到" << ob_line1.name << "路" << endl;
					measure_distance(distance, crossing[i], end_sta, ob_line1);
					break;
					//换乘一次需要调用两次直线测距离函数
					//换乘1次
				}
			}
			line avaliable_line;
			if (!done) {
				for (station_size i = 0; i < crossing.size(); i++) {
					vector<station> crossing_1;
					if (search_avalivable_line(editor.get_lines(), crossing[i], visit, avaliable_line)) {
						search_crossing(avaliable_line, crossing_1, editor.get_lines(), editor.get_all_stations());
						//第2次检测
						for (station_size j = 0; j < crossing_1.size(); j++) {
							if (station_on_line(start_sta, crossing[i], editor.get_lines(),ob_line)&& station_on_line(crossing_1[j], crossing[i], editor.get_lines(), ob_line1)&& station_on_line(crossing_1[j], end_sta, editor.get_lines(), ob_line2)) {
								measure_distance(distance, start_sta, crossing[i], ob_line);
								cout << crossing[i].name << " 换乘一次，到" << ob_line.name << "路" << endl;
								measure_distance(distance, crossing[i], crossing_1[j], ob_line1);
								cout << crossing_1[j].name << " 换乘一次，到" << ob_line1.name << "路" << endl;
								measure_distance(distance, crossing_1[j],end_sta , ob_line2);
								done = true;
								//要调用三次直线测距离函数
								cout << crossing[i].name << crossing_1[j].name << "换乘2次" << endl;
							}
							done = true;
							cout << "开摆" << endl;
						}
					}
				}
			}//线路没找到
		}
		return true;
	}
	else {
		cout << "请输入正确的站点！"<<endl;
		return false;
	}
}

void Guide::search_line(vector<line> lines, station sta, line& lin) {
	for (line_size i = 0; i < lines.size(); i++) {
		for (string_size j = 0; j < lines[i].stations.size(); j++) {
			if (sta.name == lines[i].stations[j])
				lin = lines[i];
		}
	}
}
//找到站点对应的某一个线路

void Guide::search_crossing(line lin, vector<station>& crossing, vector<line> lines, vector<station> all_stations) {
	station cros;
	vector<string> untreated_crossing_name;
	for (line_size i = 0; i < lines.size(); i++) {
		if (lines[i].name == lin.name)
			continue;
		for (string_size j = 0; j < lines[i].stations.size(); j++) {
			for (string_size k = 0; k < lin.stations.size(); k++) {
				if (lines[i].stations[j] == lin.stations[k]) {
					untreated_crossing_name.push_back(lin.stations[k]);
				}
			}
		}
	}
	sort(untreated_crossing_name.begin(), untreated_crossing_name.end());
	untreated_crossing_name.erase(unique(untreated_crossing_name.begin(), untreated_crossing_name.end()), untreated_crossing_name.end());
	for (string_size i = 0; i < untreated_crossing_name.size(); i++) {
		str_trans_sta(cros, untreated_crossing_name[i],all_stations);
		crossing.push_back(cros);
	}
}
//寻找线路上的重合点+标记已经被检测过的线路

bool Guide::search_avalivable_line(vector<line> lines, station sta, vector<bool> visit, line& avaliable_line) {
	for (line_size i = 0; i < lines.size(); i++) {
		for (string_size j = 0; j < lines[i].stations.size(); j++) {
			if (lines[i].stations[j] == sta.name && !visit[i]) {
				avaliable_line = lines[i];
				return true;
			}
		}
	}
}
//寻找重合点上没有被标记的线路

bool Guide::station_on_line(station sta1, station sta2, vector<line> lines, line& ob_line) {
	bool s1 = false;
	bool s2 = false;
	for (line_size i = 0; i < lines.size(); i++) {
		for (string_size j = 0; j < lines[i].stations.size(); j++) {
			if (lines[i].stations[j] == sta1.name) {
				s1 = true;
			}
			else if (lines[i].stations[j] == sta2.name) {
				s2 = true;
			}
		}
		if (!(s1 && s2)) {
			s1 = false;
			s2 = false;
		}
		else {
			ob_line = lines[i];
			break;
		}
	}
	return s1 && s2;
}
//检测重合点是否在特定线路上

void Guide::search_lines_index(vector<line> lines, line_size& index, line lin) {
	for (line_size i = 0; i < lines.size(); i++) {
		if (lin.name == lines[i].name)
			index = i;
	}
}

bool Guide::str_trans_line(line& lin, string line_name, vector <line> lines) {
	bool outcome = false;
	for (line_size i = 0; i < lines.size(); i++) {
		if (lines[i].name == line_name) {
			lin.name = line_name;
			lin.stations = lines[i].stations;
			lin.index = lines[i].index;
			outcome = true;
			return outcome;
		}
	}
	if (!outcome) {
		return outcome;
	}
}

bool Guide::str_trans_sta(station&sta, string sta_name, vector<station> all_stations) {
	bool outcome = false;
	for (station_size i = 0; i < all_stations.size(); i++) {
		if (all_stations[i].name == sta_name) {
			sta.name = sta_name;
			sta.index = all_stations[i].index;
			outcome = true;
			return outcome;
		}
	}
	if (!outcome) {
		return outcome;
	}
}

void Guide::output(vector<station> route) {
	for (station_size i = 0; i < route.size(); i++)
		cout << route[i].name << endl;
}

bool Guide::demostrate_line(string line_name, File_Editor editor) {
	bool outcome = false;
	line oriented_line;
	if (str_trans_line(oriented_line, line_name, editor.get_lines())) {
		cout << line_name << "线路展示：" << endl;
		for (string_size i = 0; i < oriented_line.stations.size(); i++) {
			cout << oriented_line.stations[i] << ",";
			if (i + 1 == oriented_line.stations.size()) {
				cout << oriented_line.stations[i];
			}
		}
		cout << endl;
	}
	else {
		cout << "输入错误！" << endl;
		return false;
	}
	
	
	//输入线路名称，按顺序返回站点名称
}

void Guide::measure_distance(int& distance, station start, station end, line ob_line) {
	bool is_end = false;
	bool is_start = false;
	string_size start_index = 0;
	string_size end_index = 0;
	distance = 0;
	for (string_size i = 0; i < ob_line.stations.size(); i++) {
		if (start.name == ob_line.stations[i] ) {
			start_index = i;
			is_start = true;
		}
		else if (end.name == ob_line.stations[i]) {
			end_index = i;
			is_end = true;
		}
	}
	if (is_start && is_end) {
		distance = abs((int)start_index - (int)end_index);
		if (start_index > end_index) {
			for (string_size i = start_index; i > end_index; i--)
				cout << ob_line.stations[i] << " ";
			cout << ob_line.stations[end_index];
		}
		else {
			for (string_size i = start_index; i < end_index; i++)
				cout << ob_line.stations[i] << " ";
			cout << ob_line.stations[end_index];
		}

	}
	cout << endl;
}

