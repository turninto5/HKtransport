#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "Editor.h"

using namespace std;
typedef vector<string>::size_type vec_sz;
typedef vector<int>::size_type int_size;

struct mat {
	vector<int> element;
	vector <vector<int>> matrix;
};

#define _Map_h_
#ifdef _Map_h_
class Map {
private:
	mat adjacency_matrix;

public:
	Map(File_Editor editor);

	void Dijkstra_algorithm(mat adjacency_matrix, station start, station end, vector<station> all_stations, int& min_dist, vector<station>& min_station);

	mat& get_adjacency_matrix();
};
#endif _Map_h_