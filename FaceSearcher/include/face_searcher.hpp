#pragma once

#include <string>
#include <memory>
#include <functional>
#include <opencv2\opencv.hpp>

#define TG_EXPORT __declspec(dllexport)

struct SearchResult
{
	// cv::Mat image;
	std::string path; // ͼƬ·��

	// ʶ����ID
	std::vector<int> id;
	// ʶ��������
	std::vector<float> dis;
};

struct FaceData
{
	std::string path;
	cv::Rect roi;
};

// �ص�������ȡʶ����
using Callback = std::function<void(SearchResult&)>;

enum Mode
{
	CPU,
	GPU,
};

struct SearchConfig
{
	Mode mode = CPU; // Caffe��ģʽ��CPU|GPU��
	float threshold = 0.25; // ��������֮��ŷ�Ͼ���֮��ͨ����ֵ�ж��Ƿ���ͬһ����
	float pad = 0.5785; // �����ʱ��pad�ı���
	int size = 100; // �����ʱ��ͼƬ��С
	int batch = 100;
	int max_ret = 30; // �������max_ret��ʶ����
	std::string data_folder; // �����ļ���
	std::string tmp_folder = "Tmp"; // ��ʱ�ļ���
	std::string ret_folder = "Ret"; // ʶ��������
	std::string unknown_folder = "UnK";
	std::string db_folder = "Database"; // ���ݿ�ͼƬ�ļ���
	std::string db_name = "online.database"; // ���ݿ�����
	Callback callback; // ������ʾ�Ļص�����
};


// ���
class TG_EXPORT FaceSearcher
{
public:
	// Generate Database
	// ���ɲ��������ݿ�
	virtual void GenerateDatabase(const std::string path) = 0;
	
	virtual void LoadDatabase(const std::string data_file) = 0;
	virtual void SaveDatabase(const std::string data_file) = 0;
	virtual FaceData GetData(int id) = 0;

	virtual void Set(SearchConfig config) = 0;

	virtual void Search(std::vector<std::string> image_files) = 0;
	static std::shared_ptr<FaceSearcher> Create();
};
