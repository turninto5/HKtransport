#include "Editor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "Map.h"
#include"Guide.h"

using namespace std;

typedef string::size_type string_size;

int main(){
	Map map = Map(File_Editor());
	Guide guide = Guide();
	string line_name;
	string start;
	string end;
	string parameter;

	string input;
	cout << "海口过时公交导航" << endl;
	while (true) {
		cout << "请输入指令：";
		getline(cin,input,'\n');
		if (input == "help") {
			cout << "没写帮助文档，爱咋咋"<<endl;
		}
		else if (input == "exit") {
			exit(0);
		}
		else {
			if (input[0] == '-') {
				vector<string>parameters;
				input = input + " ";
				File_Editor().split(parameters, input, ' ');
				parameter = parameters[0];
				start = parameters[1];
				end = parameters[2];

				if (parameter == "-b") {
					if (!guide.search_for_fewest_stations(start, end, File_Editor(), map))
						continue;
				}
				else if (parameter == "-c") {
					if (!guide.search_for_fewest_transfer_times(start, end, File_Editor(), map))
						continue;
				}
				else {
					cout << "输入错误，请重新输入";
					continue;
				}
			}
			else {
				if (!guide.demostrate_line(input, File_Editor())) {
					continue;
				}
			}
		}
	}
}