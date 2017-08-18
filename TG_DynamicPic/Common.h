#pragma once

struct CtrlInfo {
	int pID;
	int index_x;	//x轴序号	
	int index_y;

	int space_x;	//x轴间隔
	int space_y;
};

//struct SearchResult
//{
//	//图片路径
//	std::string path;
//
//	//识别结果ID
//	std::vector<int> id;
//
//	//识别结果距离
//	std::vector<float> dis;
//};