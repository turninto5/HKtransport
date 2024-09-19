#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "Editor.h"
#include "Map.h"
#define INF 1000
using namespace std;
typedef vector<string>::size_type vec_sz;
typedef vector<int>::size_type int_size;
typedef vector<station>::size_type station_size;
typedef vector<line>::size_type line_size;

Map::Map(File_Editor editor) {
	vector<int>element(editor.get_all_stations().size(), INF);
	adjacency_matrix.element = element;
	for (station_size i = 0; i < editor.get_all_stations().size(); i++)
		adjacency_matrix.matrix.push_back(element);
	//ȫ��Ϊ���ɴ�

	for (line_size i = 0; i < editor.get_lines().size(); i++) {
		for (vec_sz j = 0; j + 1< editor.get_lines()[i].stations.size(); j++) {
			adjacency_matrix.matrix[editor.get_lines()[i].index[j]][editor.get_lines()[i].index[j + 1]] = 1;
			adjacency_matrix.matrix[editor.get_lines()[i].index[j + 1 ]][editor.get_lines()[i].index[j]] = 1;
		}
	}
}//ȫ����ɳ�ʼ��

void Map::Dijkstra_algorithm(mat adjacency_matrix, station start, station end, vector<station> all_stations, int& min_dist, vector<station>& min_station) {
	int_size length = adjacency_matrix.matrix.size();
	vector<station> route;
	vector <bool> visit(length, false);//�������нڵ㻹��û�з��ʹ�
	int_size start_index = 0;
	//���������Ϊ���ʹ�

	vector<int> distance = adjacency_matrix.matrix[start.index];
	visit[start.index] = true;
	

	for (int_size i = 0; i < length; i++) {//�������·����ѭ��
		int_size middle = 0; //�м�ڵ������
		int min_distance = INF;//�洢��Χ�ڵ㵽������̾���

		for (station_size j = 0; j < length; j++) {//Ѱ��û�����ʵĽڵ��о����������Ľڵ���
			if (!visit[j] && min_distance > distance[j]) {//������־����һ��Ľڵ㣬������̾��룬�ͽڵ�ڵ���
				min_distance = distance[j];//������̾���
				middle = j;
			}
		}
		for (int_size j = 0; j < length; j++) {
			if (!visit[j] && distance[j] > distance[middle] + adjacency_matrix.matrix[middle][j]) {
				//�ҵ�ͨ�������㵫�Ǿ�����̵�·��
				distance[j] = distance[middle] + adjacency_matrix.matrix[middle][j];
				
			}
		}
		//route.push_back(all_stations[middle]);
		visit[middle] = true;
	}
	min_dist = distance[end.index];

}

mat& Map::get_adjacency_matrix() {
	return adjacency_matrix;
}