#define CANT_LOAD 0x12654
#define NUMBER_ERROR 0x54565
#define PARAMETER_UNFIT 0x54554
#define STATION_UNEXIST 0x68457

#include "Data.h"
#include "iostream"
#include "string"

int main(int argc,char * argv[])
{
    Data data;
    std::string fileName = "hkbus_json/hk_json.json";
    if (data.loadData(fileName)!=Data::DATA_OK)
    {
	    std::cout << "无法加载数据文件:hk_json.json,请确认数据文件是否存在于指定位置\n";
        return CANT_LOAD;
    }
    if (argc!=4)
    {
        std::cout << "参数数量不对\n";
        return NUMBER_ERROR;
    }
    std::string s1 = argv[1];
    if (s1== "-b" || s1=="-c")
    {
        std::string s2 = argv[2], s3=argv[3];
		std::vector<std::string> temVec;
		for (auto busStation : data.getBusLinesAtStations())
		{
			temVec.emplace_back(busStation.first);
		}
		if (getLocation<std::string>(temVec,s2) && getLocation<std::string>(temVec,s3))
		{
			data.getShortestTransfer(std::pair<std::string, std::string>(s2, s3),s1);
			return 0;
		}
		std::cout << "输入的站点不存在,请重新输入";
		return STATION_UNEXIST;
    }
	std::cout << "参数不符" << ";请输入以下格式:hkbus.exe -b station1 station2\n";
	return PARAMETER_UNFIT;
}