#pragma once

#include <string>
#include <memory>
#include <functional>
#include <opencv2\opencv.hpp>

#define TG_EXPORT __declspec(dllexport)

struct SearchResult
{
	// cv::Mat image;
	std::string path; // 图片路径

	// 识别结果ID
	std::vector<int> id;
	// 识别结果距离
	std::vector<float> dis;
};

struct FaceData
{
	std::string path;
	cv::Rect roi;
};

// 回调函数获取识别结果
using Callback = std::function<void(SearchResult&)>;

enum Mode
{
	CPU,
	GPU,
};

struct SearchConfig
{
	Mode mode = CPU; // Caffe的模式（CPU|GPU）
	float threshold = 0.25; // 计算特征之间欧氏距离之后，通过阈值判断是否是同一个脸
	float pad = 0.5785; // 对齐的时候，pad的比例
	int size = 100; // 对齐的时候，图片大小
	int batch = 100;
	int max_ret = 30; // 返回最多max_ret个识别结果
	std::string data_folder; // 数据文件夹
	std::string tmp_folder = "Tmp"; // 临时文件夹
	std::string ret_folder = "Ret"; // 识别结果保存
	std::string unknown_folder = "UnK";
	std::string db_folder = "Database"; // 数据库图片文件夹
	std::string db_name = "online.database"; // 数据库名称
	Callback callback; // 用于显示的回调函数
};


// 这边
class TG_EXPORT FaceSearcher
{
public:
	// Generate Database
	// 生成并保存数据库
	virtual void GenerateDatabase(const std::string path) = 0;
	
	virtual void LoadDatabase(const std::string data_file) = 0;
	virtual void SaveDatabase(const std::string data_file) = 0;
	virtual FaceData GetData(int id) = 0;

	virtual void Set(SearchConfig config) = 0;

	virtual void Search(std::vector<std::string> image_files) = 0;
	static std::shared_ptr<FaceSearcher> Create();
};
