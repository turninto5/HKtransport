#include "data.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <iostream>

inline int largeDecrease(int a,int b)
{
	if (a>b)
	{
		return a - b;
	}
	return b - a;
}

Data::Data()
{
	
}
unsigned int Data::loadData(const std::string& fileName)
{
	//做一些加载数据的准备工作
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		std::cout << "无法加载文件!" << std::endl;
		return DATA_FALSE;
	}
	std::cout << "成功加载文件!" << std::endl;
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	//加载名字
	dataStr = doc["name"].GetString();

	//加载时间
	dataTime = doc["time"].GetString();

	//加载路线数据
	std::vector<std::string> temstr = { "203", "205", "207", "208", "209", "211", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16路", "17", "18", "19", "20路", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "60", "61", "62", "63", "64", "66", "67", "68", "69", "71", "72", "73", "74", "65", "70", "77", "89", "92", "106", "k1", "k2", "快3", "游1 ", "游2", "游3", "游4", "游5", "游6", "游7", "94" };
	for (const auto& numberStr:temstr)
	{
		std::vector<std::string> temVec;
		for (rapidjson::Value::ConstValueIterator iterator = doc["Specific route"][numberStr.c_str()].Begin(); iterator != doc["Specific route"][numberStr.c_str()].End(); iterator++)
		{
			temVec.emplace_back(iterator->GetString());
			if (this->busLinesAtStations.find(iterator->GetString())!=busLinesAtStations.end())
			{
				busLinesAtStations[iterator->GetString()].emplace_back(numberStr);
			}
			else
			{
				busLinesAtStations.emplace(std::pair<std::string, std::vector<std::string>>(iterator->GetString(),std::vector<std::string>(1,numberStr)));
			}
		}
		this->busRoutes.emplace(std::pair<std::string, std::vector<std::string>>(numberStr, temVec));
	}
	return DATA_OK;
}

Data::~Data() = default;

std::string Data::getDataStr()const
{
	return dataStr;
}

std::string Data::getDataTime()const
{
	return dataTime;
}

std::unordered_map<std::string, std::vector<std::string>> Data::getBusLinesAtStations()const
{
	return busLinesAtStations;
}

std::unordered_map<std::string, std::vector<std::string>> Data::getBusRoutes()const
{
	return busRoutes;
}

void Data::unloadData()
{
	delete& busLinesAtStations;
	delete& busRoutes;
}


void Data::getShortestTransfer(const std::pair<std::string,std::string>& beginToEnd, const std::string& FLAG) const
{
	if (FLAG=="-b")
	{
		int minVertex1 = positiveInfinity;
		std::string bL;
		for (const auto& beginLine : busLinesAtStations.at(beginToEnd.first))
		{
			for (const auto& endLine : busLinesAtStations.at(beginToEnd.second))
			{
				if (endLine == beginLine &&
					(largeDecrease(getLocation(busRoutes.at(beginLine), beginToEnd.first),
						getLocation(busRoutes.at(beginLine), beginToEnd.second))) < minVertex1)
				{
					bL = beginLine;
					minVertex1 = largeDecrease(getLocation(busRoutes.at(beginLine), beginToEnd.first),
						getLocation(busRoutes.at(beginLine), beginToEnd.second));
				}
			}
		}
		if (minVertex1 != positiveInfinity)
		{
			std::cout << bL << std::endl;
			std::cout << beginToEnd.first << std::endl;
			printLines(busRoutes.at(bL), getLocation<std::string>(busRoutes.at(bL), beginToEnd.first),
				getLocation<std::string>(busRoutes.at(bL), beginToEnd.second));
			return;
		}

		int minVertex2 = positiveInfinity;
		std::string bStr, eStr;
		std::pair<std::string, std::vector<std::string>> tVec;
		for (const auto& beginLine : busLinesAtStations.at(beginToEnd.first))
		{
			for (const auto& endLine : busLinesAtStations.at(beginToEnd.second))
			{
				for (const auto& temTransitStation : busLinesAtStations)
				{
					if (getLocation<std::string>(temTransitStation.second, beginLine) &&
						getLocation<std::string>(temTransitStation.second, endLine))
					{
						if ((largeDecrease(getLocation<std::string>(busRoutes.at(beginLine), beginToEnd.first),
							getLocation<std::string>(busRoutes.at(beginLine), temTransitStation.first)) +
							largeDecrease(getLocation<std::string>(busRoutes.at(endLine), temTransitStation.first),
								getLocation<std::string>(busRoutes.at(endLine), beginToEnd.second))) < minVertex2)
						{
							minVertex2 = (largeDecrease(getLocation<std::string>(busRoutes.at(beginLine), beginToEnd.first),
								getLocation<std::string>(busRoutes.at(beginLine), temTransitStation.first)) +
								largeDecrease(getLocation<std::string>(busRoutes.at(endLine), temTransitStation.first),
									getLocation<std::string>(busRoutes.at(endLine), beginToEnd.second)));
							bStr = beginLine;
							eStr = endLine;
							tVec = temTransitStation;
						}

					}
				}
			}
		}
		if (minVertex2 != positiveInfinity)
		{
			std::cout << bStr << std::endl;
			std::cout << beginToEnd.first << std::endl;
			printLines<std::string>(busRoutes.at(bStr),
				getLocation<std::string>(busRoutes.at(bStr), beginToEnd.first),
				getLocation<std::string>(busRoutes.at(bStr), tVec.first));

			std::cout << "换乘" << eStr << "公交车" << std::endl;

			printLines<std::string>(busRoutes.at(eStr),
				getLocation<std::string>(busRoutes.at(eStr), tVec.first),
				getLocation<std::string>(busRoutes.at(eStr), beginToEnd.second));
			return;
		}

		int minVertex3 = positiveInfinity;
		std::string bStr1, eStr1;
		std::pair<std::string, std::vector<std::string>> tVc2, tVc1;
		std::string tL1;

		for (const auto& beginLine : busLinesAtStations.at(beginToEnd.first))
		{
			for (const auto& endLine : busLinesAtStations.at(beginToEnd.second))
			{
				for (const auto& temTransitStation1 : busLinesAtStations)
				{
					for (const auto& temTransitStation2 : busLinesAtStations)
					{
						if (getLocation<std::string>(temTransitStation1.second, beginLine) &&
							getLocation<std::string>(temTransitStation2.second, endLine) && temTransitStation1 != temTransitStation2 &&
							temTransitStation1.first != beginToEnd.first && temTransitStation2.first != beginToEnd.second)
						{
							for (const auto& transitLine1 : temTransitStation1.second)
							{
								for (const auto& transitLine2 : temTransitStation2.second)
								{
									if (transitLine1 == transitLine2 && (largeDecrease(getLocation<std::string>(busRoutes.at(beginLine), beginToEnd.first),
										getLocation<std::string>(busRoutes.at(beginLine), temTransitStation1.first)) +
										largeDecrease(getLocation<std::string>(busRoutes.at(transitLine2), temTransitStation1.first),
											getLocation<std::string>(busRoutes.at(transitLine2), temTransitStation2.first)) +
										largeDecrease(getLocation<std::string>(busRoutes.at(endLine), temTransitStation2.first),
											getLocation<std::string>(busRoutes.at(endLine), beginToEnd.second))) < minVertex3)
									{
										minVertex3 = (largeDecrease(getLocation<std::string>(busRoutes.at(beginLine), beginToEnd.first),
											getLocation<std::string>(busRoutes.at(beginLine), temTransitStation1.first)) +
											largeDecrease(getLocation<std::string>(busRoutes.at(transitLine2), temTransitStation1.first),
												getLocation<std::string>(busRoutes.at(transitLine2), temTransitStation2.first)) +
											largeDecrease(getLocation<std::string>(busRoutes.at(endLine), temTransitStation2.first),
												getLocation<std::string>(busRoutes.at(endLine), beginToEnd.second)));
										bStr1 = beginLine;
										eStr1 = endLine;
										tVc1 = temTransitStation1;
										tVc2 = temTransitStation2;
										tL1 = transitLine1;
									}
								}
							}
						}
					}
				}
			}
		}
		if (minVertex3 != positiveInfinity)
		{
			std::cout << bStr1 << std::endl;
			std::cout << beginToEnd.first << std::endl;

			printLines<std::string>(busRoutes.at(bStr1),
				getLocation<std::string>(busRoutes.at(bStr1), beginToEnd.first),
				getLocation<std::string>(busRoutes.at(bStr1), tVc1.first));

			std::cout << "换乘" << tL1 << "路公交车" << std::endl;

			printLines<std::string>(busRoutes.at(tL1),
				getLocation<std::string>(busRoutes.at(tL1), tVc1.first),
				getLocation<std::string>(busRoutes.at(tL1), tVc2.first));

			std::cout << "换乘" << eStr1 << "路公交车" << std::endl;

			printLines<std::string>(busRoutes.at(eStr1),
				getLocation<std::string>(busRoutes.at(eStr1), tVc2.first),
				getLocation<std::string>(busRoutes.at(eStr1), beginToEnd.second));
			return;
		}
		std::cout << "需要换乘的次数过多，不建议在此地乘公交去目的地";
	}
	if (FLAG=="-c")
	{
		int minVertex1 = positiveInfinity;
		std::string bL;
		for (const auto& beginLine : busLinesAtStations.at(beginToEnd.first))
		{
			for (const auto& endLine : busLinesAtStations.at(beginToEnd.second))
			{
				if (endLine == beginLine &&
					(largeDecrease(getLocation(busRoutes.at(beginLine), beginToEnd.first),
						getLocation(busRoutes.at(beginLine), beginToEnd.second))) < minVertex1)
				{
					bL = beginLine;
					minVertex1 = largeDecrease(getLocation(busRoutes.at(beginLine), beginToEnd.first),
						getLocation(busRoutes.at(beginLine), beginToEnd.second));
				}
			}
		}

		int minVertex2 = positiveInfinity;
		std::string bStr, eStr;
		std::pair<std::string, std::vector<std::string>> tVec;
		for (const auto& beginLine : busLinesAtStations.at(beginToEnd.first))
		{
			for (const auto& endLine : busLinesAtStations.at(beginToEnd.second))
			{
				for (const auto& temTransitStation : busLinesAtStations)
				{
					if (getLocation<std::string>(temTransitStation.second, beginLine) &&
						getLocation<std::string>(temTransitStation.second, endLine))
					{
						if ((largeDecrease(getLocation<std::string>(busRoutes.at(beginLine), beginToEnd.first),
							getLocation<std::string>(busRoutes.at(beginLine), temTransitStation.first)) +
							largeDecrease(getLocation<std::string>(busRoutes.at(endLine), temTransitStation.first),
								getLocation<std::string>(busRoutes.at(endLine), beginToEnd.second))) < minVertex2)
						{
							minVertex2 = (largeDecrease(getLocation<std::string>(busRoutes.at(beginLine), beginToEnd.first),
								getLocation<std::string>(busRoutes.at(beginLine), temTransitStation.first)) +
								largeDecrease(getLocation<std::string>(busRoutes.at(endLine), temTransitStation.first),
									getLocation<std::string>(busRoutes.at(endLine), beginToEnd.second)));
							bStr = beginLine;
							eStr = endLine;
							tVec = temTransitStation;
						}

					}
				}
			}
		}

		int minVertex3 = positiveInfinity;
		std::string bStr1, eStr1;
		std::pair<std::string, std::vector<std::string>> tVc2, tVc1;
		std::string tL1;

		for (const auto& beginLine : busLinesAtStations.at(beginToEnd.first))
		{
			for (const auto& endLine : busLinesAtStations.at(beginToEnd.second))
			{
				for (const auto& temTransitStation1 : busLinesAtStations)
				{
					for (const auto& temTransitStation2 : busLinesAtStations)
					{
						if (getLocation<std::string>(temTransitStation1.second, beginLine) &&
							getLocation<std::string>(temTransitStation2.second, endLine) && temTransitStation1 != temTransitStation2 &&
							temTransitStation1.first != beginToEnd.first && temTransitStation2.first != beginToEnd.second)
						{
							for (const auto& transitLine1 : temTransitStation1.second)
							{
								for (const auto& transitLine2 : temTransitStation2.second)
								{
									if (transitLine1 == transitLine2 && (largeDecrease(getLocation<std::string>(busRoutes.at(beginLine), beginToEnd.first),
										getLocation<std::string>(busRoutes.at(beginLine), temTransitStation1.first)) +
										largeDecrease(getLocation<std::string>(busRoutes.at(transitLine2), temTransitStation1.first),
											getLocation<std::string>(busRoutes.at(transitLine2), temTransitStation2.first)) +
										largeDecrease(getLocation<std::string>(busRoutes.at(endLine), temTransitStation2.first),
											getLocation<std::string>(busRoutes.at(endLine), beginToEnd.second))) < minVertex3)
									{
										minVertex3 = (largeDecrease(getLocation<std::string>(busRoutes.at(beginLine), beginToEnd.first),
											getLocation<std::string>(busRoutes.at(beginLine), temTransitStation1.first)) +
											largeDecrease(getLocation<std::string>(busRoutes.at(transitLine2), temTransitStation1.first),
												getLocation<std::string>(busRoutes.at(transitLine2), temTransitStation2.first)) +
											largeDecrease(getLocation<std::string>(busRoutes.at(endLine), temTransitStation2.first),
												getLocation<std::string>(busRoutes.at(endLine), beginToEnd.second)));
										bStr1 = beginLine;
										eStr1 = endLine;
										tVc1 = temTransitStation1;
										tVc2 = temTransitStation2;
										tL1 = transitLine1;
									}
								}
							}
						}
					}
				}
			}
		}
		if (minVertex1<=minVertex2 && minVertex1<=minVertex3)
		{
			if (minVertex1 != positiveInfinity)
			{
				std::cout << bL << std::endl;
				std::cout << beginToEnd.first << std::endl;
				printLines(busRoutes.at(bL), getLocation<std::string>(busRoutes.at(bL), beginToEnd.first),
				getLocation<std::string>(busRoutes.at(bL), beginToEnd.second));
				return;
			}
		}
		if (minVertex2<=minVertex1 && minVertex2<=minVertex3)
		{
			if (minVertex2 != positiveInfinity)
			{
				std::cout << bStr << std::endl;
				std::cout << beginToEnd.first << std::endl;
				printLines<std::string>(busRoutes.at(bStr),
					getLocation<std::string>(busRoutes.at(bStr), beginToEnd.first),
					getLocation<std::string>(busRoutes.at(bStr), tVec.first));
	
				std::cout << "换乘" << eStr << "公交车" << std::endl;
	
				printLines<std::string>(busRoutes.at(eStr),
					getLocation<std::string>(busRoutes.at(eStr), tVec.first),
					getLocation<std::string>(busRoutes.at(eStr), beginToEnd.second));
				return;
			}
		}
		if (minVertex3 != positiveInfinity)
		{
			std::cout << bStr1 << std::endl;
			std::cout << beginToEnd.first << std::endl;
	
			printLines<std::string>(busRoutes.at(bStr1),
				getLocation<std::string>(busRoutes.at(bStr1), beginToEnd.first),
				getLocation<std::string>(busRoutes.at(bStr1), tVc1.first));
	
			std::cout << "换乘" << tL1 << "路公交车" << std::endl;
	
			printLines<std::string>(busRoutes.at(tL1),
				getLocation<std::string>(busRoutes.at(tL1), tVc1.first),
				getLocation<std::string>(busRoutes.at(tL1), tVc2.first));
	
			std::cout << "换乘" << eStr1 << "路公交车" << std::endl;
	
			printLines<std::string>(busRoutes.at(eStr1),
				getLocation<std::string>(busRoutes.at(eStr1), tVc2.first),
				getLocation<std::string>(busRoutes.at(eStr1), beginToEnd.second));
			return;
		}
		std::cout << "需要换乘的次数过多，不建议在此地乘公交去目的地";
	}
}

