#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

//创建数据类用来加载并获取组织数据
class Data
{
public:
	//构造
	explicit Data();

	//析构
	virtual ~Data();

	//加载数据是否成功的标识符
	const static unsigned int DATA_OK = 1;
	const static unsigned int DATA_FALSE = 0;

	//相对正无穷
	const static int positiveInfinity = 999;

    //从给定的*.json文件加载数据
	unsigned int loadData(const std::string&);

	//卸载数据
	void unloadData();

	//获取私有数据
	std::string getDataStr()const;
	std::string getDataTime()const;
	std::unordered_map<std::string, std::vector<std::string>> getBusRoutes()const;
	std::unordered_map<std::string, std::vector<std::string>> getBusLinesAtStations()const;

    //得到最少的换成路线
	void getShortestTransfer(const std::pair<std::string, std::string>&, const std::string&) const;
private:

	//数据名称
	std::string dataStr;

	//数据时间
	std::string dataTime;

	//公交路线信息
	std::unordered_map<std::string, std::vector<std::string>> busRoutes;

	//站点公交线路信息
	std::unordered_map<std::string, std::vector<std::string>> busLinesAtStations;
};

template<typename T>
int getLocation(std::vector<T> vec, T t)
{
	for (int i = 0; i <vec.size() ; ++i)
	{
		if (vec[i]==t)
		{
			return i;
		}
	}
	return Data::DATA_FALSE;
}

template<typename T>
void printLines(std::vector<T> vec, int begin, int end)
{
	if (begin<end)
	{
		for (int i = begin+1; i <= end; ++i)
		{
			std::cout<< vec[i];
			if (i!=end)
			{
				std::cout << '\n';
			}
		}
	}
	else
	{
		for (int i = begin-1; i >=end; --i)
		{
			std::cout << vec[i] ;
			if (i != end)
			{
				std::cout << '\n';
			}
		}
	}
}