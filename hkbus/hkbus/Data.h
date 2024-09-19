#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

//�����������������ز���ȡ��֯����
class Data
{
public:
	//����
	explicit Data();

	//����
	virtual ~Data();

	//���������Ƿ�ɹ��ı�ʶ��
	const static unsigned int DATA_OK = 1;
	const static unsigned int DATA_FALSE = 0;

	//���������
	const static int positiveInfinity = 999;

    //�Ӹ�����*.json�ļ���������
	unsigned int loadData(const std::string&);

	//ж������
	void unloadData();

	//��ȡ˽������
	std::string getDataStr()const;
	std::string getDataTime()const;
	std::unordered_map<std::string, std::vector<std::string>> getBusRoutes()const;
	std::unordered_map<std::string, std::vector<std::string>> getBusLinesAtStations()const;

    //�õ����ٵĻ���·��
	void getShortestTransfer(const std::pair<std::string, std::string>&, const std::string&) const;
private:

	//��������
	std::string dataStr;

	//����ʱ��
	std::string dataTime;

	//����·����Ϣ
	std::unordered_map<std::string, std::vector<std::string>> busRoutes;

	//վ�㹫����·��Ϣ
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