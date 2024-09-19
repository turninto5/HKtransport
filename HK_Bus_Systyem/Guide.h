#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include<algorithm>
#include "Editor.h"
#include "Map.h"
#include "Guide.h"

using namespace std;
typedef vector<line>::size_type line_size;
typedef vector<string>::size_type string_size;
typedef vector<int>::size_type int_size;
#define _Guide_h_
#ifdef _Guide_h_

class Guide {
private:
	int transfer_times;
	int distance;
public:
	Guide();

	bool search_for_fewest_stations(string start, string end, File_Editor editor, Map map);

	bool search_for_fewest_transfer_times(string start, string end, File_Editor editor, Map map);

	/*void demostrate_line(string line_name, vector <line> lines);*/

	bool str_trans_line(line& lin, string line_name, vector <line> lines);

	bool str_trans_sta(station& sta, string sta_name, vector<station> all_stations);

	void output(vector<station> route);

	void search_line(vector<line> lines, station sta, line& lin);

	void search_crossing(line lin, vector<station>& crossing, vector<line> lines, vector<station> all_stations);

	bool search_avalivable_line(vector<line> lines, station sta, vector<bool> visit, line& avaliable_line);

	bool station_on_line(station sta1, station sta2, vector<line> lines, line& ob_line);

	void search_lines_index(vector<line> lines, line_size& index, line lin);
	
	bool demostrate_line(string line_name, File_Editor editor);

	void measure_distance(int& distance, station start, station end, line ob_line);

};
#endif 