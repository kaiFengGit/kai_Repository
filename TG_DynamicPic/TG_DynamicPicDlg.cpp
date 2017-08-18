
// TG_DynamicPicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TG_DynamicPic.h"
#include "TG_DynamicPicDlg.h"
#include "afxdialogex.h"
#include "utils.hpp"

#include <sstream>

//#include <opencv2/opencv.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTG_DynamicPicDlg *pCTG_DynamicPicDlg;
void CallBackFunction(SearchResult &result);
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTG_DynamicPicDlg dialog



CTG_DynamicPicDlg::CTG_DynamicPicDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TG_DYNAMICPIC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTG_DynamicPicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTG_DynamicPicDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_32773, &CTG_DynamicPicDlg::OnBegin)
	ON_COMMAND(ID_Stop, &CTG_DynamicPicDlg::OnStop)
	ON_COMMAND(ID_UP, &CTG_DynamicPicDlg::OnUp)
	ON_COMMAND(ID_DOWN, &CTG_DynamicPicDlg::OnDown)
	ON_COMMAND(ID_Test, &CTG_DynamicPicDlg::OnTest)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CTG_DynamicPicDlg message handlers

BOOL CTG_DynamicPicDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	BindOpencvWindowWithPicControl();
	GetStatusControlHandle();

	for (auto ID : m_StatusID)
		GetDlgItem(ID)->SetWindowText(_T(""));

	pCTG_DynamicPicDlg = this;

	CMenu* submenu = GetMenu();
	submenu->EnableMenuItem(ID_Stop, MF_DISABLED);
	submenu->EnableMenuItem(ID_UP, MF_DISABLED);
	submenu->EnableMenuItem(ID_DOWN, MF_DISABLED);
	m_ShowIndex = 0;
	m_TotalCaptureNum = 0;
	
	blackTopicFont.CreateFont(25, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("微软雅黑"));

	blackStatusFont.CreateFont(20, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("微软雅黑"));

	GetDlgItem(IDC_CAPTURE)->SetFont(&blackTopicFont);
	GetDlgItem(IDC_CAPTURE)->SetWindowText(_T("摄像头\n采集人脸"));
	
	GetDlgItem(IDC_RES)->SetFont(&blackTopicFont);
	GetDlgItem(IDC_RES)->SetWindowText(_T("识别结果"));

	UpDateStatus();

	SetTimer(1000, 1000, NULL);

	m_capFPS = m_resFPS = 0;
	m_bRecord = false;

	//在现实之前 清空所有图像
	ClearPic_Text();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTG_DynamicPicDlg::GetStatusControlHandle()
{
	int StatusID[] = {
			IDC_T1,
			IDC_T2,
			IDC_T3,
			IDC_T4,
			IDC_T5,
			IDC_T6,
			IDC_T7,
			IDC_T8,
			IDC_T9,
			IDC_T10,
			IDC_T11,
			IDC_T12,
			IDC_T13,
			IDC_T14,
			IDC_T15,
			IDC_T16,
			IDC_T17,
			IDC_T18,
			IDC_T19,
			IDC_T20,
			IDC_T21,
	};

	m_StatusID.assign(begin(StatusID), end(StatusID));
}

void CTG_DynamicPicDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTG_DynamicPicDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTG_DynamicPicDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTG_DynamicPicDlg::BindWIndow(int ID)
{
	//绑定opencv窗口和pic窗口
	string windowName(int2str(ID));

	cv::namedWindow(windowName.c_str(), cv::WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle(windowName.c_str());
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(ID)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	
	CRect rect;
	GetDlgItem(ID)->GetWindowRect(&rect);
	cv::Mat mat(cv::Size(rect.Width(), rect.Height()), CV_8UC3, cv::Scalar(200, 200, 200));
	cv::imshow(windowName.c_str(), mat);
}


void CTG_DynamicPicDlg::BindOpencvWindowWithPicControl()
{
	std::stringstream sst;
	
	struct PicInfo
	{
		int id;
		TYPE type;
	} picInfo[] = 
	{
		//DataBasePic
		IDC_DATABASE_1_1, DataBasePic,
		IDC_DATABASE_1_2, DataBasePic,
		IDC_DATABASE_1_3, DataBasePic,
		IDC_DATABASE_1_4, DataBasePic,
		IDC_DATABASE_1_5, DataBasePic,
		IDC_DATABASE_1_6, DataBasePic,
		IDC_DATABASE_1_7, DataBasePic,
		IDC_DATABASE_2_1, DataBasePic,
		IDC_DATABASE_2_2, DataBasePic,
		IDC_DATABASE_2_3, DataBasePic,
		IDC_DATABASE_2_4, DataBasePic,
		IDC_DATABASE_2_5, DataBasePic,
		IDC_DATABASE_2_6, DataBasePic,
		IDC_DATABASE_2_7, DataBasePic,
		IDC_DATABASE_3_1, DataBasePic,
		IDC_DATABASE_3_2, DataBasePic,
		IDC_DATABASE_3_3, DataBasePic,
		IDC_DATABASE_3_4, DataBasePic,
		IDC_DATABASE_3_5, DataBasePic,
		IDC_DATABASE_3_6, DataBasePic,
		IDC_DATABASE_3_7, DataBasePic,
		//CapturePic
		IDC_CAPTURE_P1, CapturePic,
		IDC_CAPTURE_P2, CapturePic,
		IDC_CAPTURE_P3, CapturePic,
		IDC_CAPTURE_P4, CapturePic,
		IDC_CAPTURE_P5, CapturePic,
		IDC_CAPTURE_P6, CapturePic,
		IDC_CAPTURE_P7, CapturePic,
		IDC_CAPTURE_P8, CapturePic,
		//ResultPic
		IDC_RES_PIC_1, ResultPic,
		IDC_RES_PIC_2, ResultPic,
		IDC_RES_PIC_3, ResultPic,
	};


	for (int i = 0; i < sizeof(picInfo) / sizeof(PicInfo); i++)
	{
		vector<string> &vec = picInfo[i].type == DataBasePic ? m_DataBaseNamedWindow : picInfo[i].type == ResultPic ?
																								m_ResultNamedWindow : m_CaptureNamedWindow;
		
		BindWIndow(picInfo[i].id);
		vec.push_back(int2str(picInfo[i].id));
	}
}

void CTG_DynamicPicDlg::OnBegin()
{
	m_bRecord = false;
	stringstream ss;
	// TODO: Add your command handler code here
	m_ShowIndex = 0;
	CMenu* submenu = GetMenu();
	submenu->EnableMenuItem(ID_32773, MF_DISABLED);
	submenu->EnableMenuItem(ID_Stop, MF_ENABLED);
	submenu->EnableMenuItem(ID_UP, MF_DISABLED);
	submenu->EnableMenuItem(ID_DOWN, MF_DISABLED);
	SendMessage(WM_NCPAINT, 0, 0);

	// 配置文件路径
	char data_buff[64]{ 0 };
	GetPrivateProfileString("FaceSearch", "DataPath", "upload_face", data_buff, 64, "./config.ini");

	char threshold_buff[64]{ 0 };
	GetPrivateProfileString("FaceSearch", "Threshold", "0.4", threshold_buff, 64, "./config.ini");

	ss << std::string(threshold_buff);

	//注册dll回调
	SearchConfig config;
	config.mode = CPU;
	config.callback = CallBackFunction;
	ss >> config.threshold;
	config.data_folder = std::string(data_buff);  //

	//Start(config);
}


void CTG_DynamicPicDlg::OnStop()
{
	m_bRecord = true;
	// TODO: Add your command handler code here
	//Stop();
	CMenu* submenu = GetMenu();
	submenu->EnableMenuItem(ID_32773, MF_ENABLED);
	submenu->EnableMenuItem(ID_Stop, MF_DISABLED);
	submenu->EnableMenuItem(ID_UP, MF_ENABLED);
	submenu->EnableMenuItem(ID_DOWN, MF_ENABLED);
	SendMessage(WM_NCPAINT, 0, 0);
}

void CTG_DynamicPicDlg::ChangeIndex(int num)
{
	if (m_Result.size() < 3)
		return;

	m_ShowIndex += num;

	//不论步长为多少均适用
	if (m_ShowIndex < 0)
		m_ShowIndex = 0;

	if (m_ShowIndex > int(m_Result.size() - 3))
		m_ShowIndex = m_Result.size() - 3;

}

void CTG_DynamicPicDlg::OnUp()
{
	// TODO: Add your command handler code here
	ChangeIndex(1);
	UpDateShowData();
}


void CTG_DynamicPicDlg::OnDown()
{
	// TODO: Add your command handler code here
	ChangeIndex(-1);
	UpDateShowData();
}
std::string OrgPath = "C:\\Users\\管理员\\Desktop\\test\\Jennifer_Lawrence_";
void CTG_DynamicPicDlg::OnTest()
{
	// TODO: Add your command handler code here
	static int index = 0;
	index++;
	
	SearchResult searchResult;
	CStringA str;
	str.Format("%s%04d.jpg", OrgPath.c_str(), index);

	searchResult.path = str.GetBuffer();//应该是绝对路径
	srand((int)time(NULL));
	for (int i = 0; i < index-2; ++i)
	{
		int id = rand() % 30;
		searchResult.id.push_back(id+1);
		searchResult.dis.push_back(id);
	}
	
	CallBackFunction(searchResult);
}

//回调函数
void CallBackFunction(SearchResult &result)
{
	//通过识别结果ID的size进行判断是否在库内查找成功
	pCTG_DynamicPicDlg->m_TotalCaptureNum++;
	pCTG_DynamicPicDlg->m_CapturePicPath.push_front(result.path);

	//成功从数据库中完成识别
	if (result.id.size() > 0)
	{
		pCTG_DynamicPicDlg->m_Result.push_front(result);
	}

	pCTG_DynamicPicDlg->UpDateShowData();

}

void CTG_DynamicPicDlg::ClearPic_Text()
{
	//清除所有图像
	cv::Mat mat(300, 300, CV_8UC3, cv::Scalar(200, 200, 200));

	vector<string> clearWindow;
	clearWindow.insert(clearWindow.end(), m_DataBaseNamedWindow.begin(), m_DataBaseNamedWindow.end());
	clearWindow.insert(clearWindow.end(), m_ResultNamedWindow.begin(), m_ResultNamedWindow.end());
	clearWindow.insert(clearWindow.end(), m_CaptureNamedWindow.begin(), m_CaptureNamedWindow.end());

	for (auto window : clearWindow)
		cv::imshow(window, mat);

	//清除所有status
	//vector<int> m_StatusID;
	for (auto id : m_StatusID)
		GetDlgItem(id)->SetWindowText(_T(""));
}

void CTG_DynamicPicDlg::UpDateShowData()
{
	//显示所有图像
	ShowPic();

	//状态栏
	UpDateStatus();
}

void CTG_DynamicPicDlg::UpDateStatus()
{
	CString mfcStr;
	mfcStr.Format(_T("总采集数：%d\n总识别数：%d"), m_TotalCaptureNum, m_Result.size());
	
	GetDlgItem(IDC_STATUS1)->SetFont(&blackStatusFont);
	GetDlgItem(IDC_STATUS1)->SetWindowText(mfcStr);

	mfcStr.Format(_T("当前位置：%d"), m_Result.size() - m_ShowIndex);
	GetDlgItem(IDC_STATUS3)->SetFont(&blackStatusFont);
	GetDlgItem(IDC_STATUS3)->SetWindowText(mfcStr);

	mfcStr.Format(_T("采集FPS：%d\n识别FPS：%d"), m_capFPS, m_resFPS);
	GetDlgItem(IDC_STATUS2)->SetFont(&blackStatusFont);
	GetDlgItem(IDC_STATUS2)->SetWindowText(mfcStr);
}

//显示container 中图像 到窗口 namedWindow 中
template <typename T>    
void ShowImg(T container, cv::Size sizeRect, vector<string> &namedWindow)  //maxSize 最大容积 5个窗口就是5
{
	cv::Mat mat;
	auto itrContainer = begin(container);
	for (size_t i = 0; i < namedWindow.size(); ++i)
	{
		if (i < container.size())
		{
			mat = cv::imread(*(itrContainer++));
			if (mat.empty())
			{
				MessageBox(NULL, _T("读取图片错误"), _T("读取图片错误"), MB_OK);
				return;
			}

			cv::resize(mat, mat, sizeRect);
			cv::imshow(namedWindow[i], mat);
		}
		else
		{
			mat = cv::Mat(sizeRect, CV_8UC3, cv::Scalar(200, 200, 200));
			cv::imshow(namedWindow[i], mat);
		}
	}
}

void CTG_DynamicPicDlg::ShowText(vector<int> vecID, vector<float> dis)
{
	auto itrID = vecID.begin();
	auto itrDis = dis.begin();
	CString mfcStr;
	
	size_t maxShowSize = min(vecID.size(), dis.size());
	for (size_t i = 0; i < vecID.size(); ++i)
	{
		if (i < maxShowSize)
		{
			mfcStr.Format(_T("序号:%d\r\n距离:%.2f"), i + 1,*(itrDis + i));
			GetDlgItem(*(itrID + i))->SetWindowText(mfcStr);
		}
		else
		{
			GetDlgItem(*(itrID + i))->SetWindowText(_T(""));
		}
	}
}

//void CTG_DynamicPicDlg::ShowPic()
//{
//	//auto searcher = FaceSearcher::Create();
//	CRect rect, rect1;
//
//	//------显示底部 采集到的人脸pic----------------------------------------
//	GetDlgItem(IDC_CAPTURE_P1)->GetWindowRect(&rect);
//	ShowImg(m_CapturePicPath, cv::Size(rect.Width(), rect.Height()), m_CaptureNamedWindow);
//
//	//------显示 识别到的人脸pic & 数据库人脸----------------------------------------
//	//------显示 识别到的人脸pic & 数据库人脸----------------------------------------
//	//将数据库显示图片框分成三行 显示状态栏窗口也分成三行
//	vector<vector<string>> vecDataBaseWindow(3);  //3rows
//	vector<vector<int>> vecStatusID(3);
//	for (int i = 0; i < vecDataBaseWindow.size(); ++i)
//	{
//		for (int j = 0; j < 7; ++j)
//		{
//			vecDataBaseWindow[i].push_back(m_DataBaseNamedWindow.at(i * 7 + j));
//			vecStatusID[i].push_back(m_StatusID.at(i * 7 + j));
//		}
//	}
//
//	//显示数据库人脸
//	GetDlgItem(IDC_DATABASE_1_1)->GetWindowRect(&rect);
//	vector<string> vecResult;
//	int indexWindow = 0;
//	CStringA mfcStr;
//	for (auto res : m_Result)
//	{
//		vector<string> vecDataBase;
//		if (indexWindow >= 3)
//			break;
//		//利用图像id进行获取图像
//		//for (auto id : res.id)
//		//	vecDataBase.push_back(searcher->GetData(id).path);
//		for (auto id : res.id)
//		{
//			mfcStr.Format("C:\\Users\\管理员\\Desktop\\test\\Jennifer_Lawrence_%04d.jpg", id);//----需要修改路径
//			vecDataBase.push_back(mfcStr.GetBuffer());
//		}
//		ShowImg(vecDataBase, cv::Size(rect.Width(), rect.Height()), vecDataBaseWindow.at(indexWindow));
//		
//		//在此处显示text
//		ShowText(vecStatusID.at(indexWindow), res.dis);
//
//		//识别结果图像
//		vecResult.push_back(res.path);
//		indexWindow++;
//	}
//
//	//显示识别结果人脸
//	GetDlgItem(IDC_RES_PIC_1)->GetWindowRect(&rect);
//	ShowImg(vecResult, cv::Size(rect.Width(), rect.Height()), m_ResultNamedWindow);
//}

void CTG_DynamicPicDlg::ShowPic()
{
	//auto searcher = FaceSearcher::Create();
	CRect rect, rect1;

	//------显示底部 采集到的人脸pic----------------------------------------
	GetDlgItem(IDC_CAPTURE_P1)->GetWindowRect(&rect);
	ShowImg(m_CapturePicPath, cv::Size(rect.Width(), rect.Height()), m_CaptureNamedWindow);

	//------显示 识别到的人脸pic & 数据库人脸----------------------------------------
	//------显示 识别到的人脸pic & 数据库人脸----------------------------------------
	
	//1.将数据库显示图片框分成三行 显示状态栏窗口也分成三行
	vector<vector<string>> vecDataBaseWindow(3);  //3rows
	vector<vector<int>> vecStatusID(3);
	for (int i = 0; i < vecDataBaseWindow.size(); ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			vecDataBaseWindow[i].push_back(m_DataBaseNamedWindow.at(i * 7 + j));
			vecStatusID[i].push_back(m_StatusID.at(i * 7 + j));
		}
	}

	//2.显示数据库人脸
	GetDlgItem(IDC_DATABASE_1_1)->GetWindowRect(&rect);
	vector<string> vecResult;
	int indexWindow = 0, index_show = 0;
	CStringA mfcStr;

	for (auto res : m_Result)
	{
		//if用来解决停止后 上下翻页图像的问题
		if (index_show < m_ShowIndex)
		{
			index_show++;
			continue;
		}

		vector<string> vecDataBase;
		if (indexWindow >= 3)
			break;
		//利用图像id进行获取图像
		//for (auto id : res.id)
		//{
		//	auto face_data = searcher->GetData(id);
		//	mfcStr.Format("%s.jpg", face_data.path.c_str());
		//	vecDataBase.push_back(mfcStr.GetBuffer());
		//}
		for (auto id : res.id)
		{
			mfcStr.Format("C:\\Users\\管理员\\Desktop\\test\\Jennifer_Lawrence_%04d.jpg", id);	//----需要修改路径
			vecDataBase.push_back(mfcStr.GetBuffer());
		}
		ShowImg(vecDataBase, cv::Size(rect.Width(), rect.Height()), vecDataBaseWindow.at(indexWindow));

		//在此处显示text
		ShowText(vecStatusID.at(indexWindow), res.dis);

		//识别结果图像
		vecResult.push_back(res.path);
		indexWindow++;
	}

	//显示识别结果人脸
	GetDlgItem(IDC_RES_PIC_1)->GetWindowRect(&rect);
	ShowImg(vecResult, cv::Size(rect.Width(), rect.Height()), m_ResultNamedWindow);
}

void CTG_DynamicPicDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	Stop();

	CDialogEx::OnClose();
}


void CTG_DynamicPicDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	static int preRes = 0;
	static int preCap = 0;

	//m_TotalCaptureNum, m_Result.size()
	
	if (nIDEvent == 1000)
	{
		m_capFPS = m_TotalCaptureNum - preCap;
		m_resFPS = m_Result.size() - preRes;
	}

	preCap = m_TotalCaptureNum;
	preRes = m_Result.size();
	UpDateStatus();
	CDialogEx::OnTimer(nIDEvent);
}


void CTG_DynamicPicDlg::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// This feature requires Windows Vista or greater.
	// The symbol _WIN32_WINNT must be >= 0x0600.
	// TODO: Add your message handler code here and/or call default


	CDialogEx::OnMouseHWheel(nFlags, zDelta, pt);
}

BOOL CTG_DynamicPicDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bRecord)
	{
		if (zDelta > 0)
			ChangeIndex(1);
		else
			ChangeIndex(-1);

		UpDateShowData();
	}

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}
