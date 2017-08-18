
// TG_DynamicPicDlg.h : header file
//


#pragma once

#include <face_searcher.hpp>
#include <search_processor.hpp>


// CTG_DynamicPicDlg dialog
class CTG_DynamicPicDlg : public CDialogEx
{
// Construction
public:
	CTG_DynamicPicDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TG_DYNAMICPIC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	void BindOpencvWindowWithPicControl();
	void BindWIndow(int ID);
	void ShowPic();
	void UpDateShowData();
	void GetStatusControlHandle();
	void ShowText(vector<int> vecHandle, vector<float> dis);
	void ChangeIndex(int num);
	void ClearPic_Text();
	void UpDateStatus();

	vector<string> m_DataBaseNamedWindow;
	vector<string> m_ResultNamedWindow;
	vector<string> m_CaptureNamedWindow;

	vector<int> m_StatusID;

	int m_ShowIndex;
	int m_TotalCaptureNum;

	int m_capFPS;
	int m_resFPS;
	bool m_bRecord;

	list<string> m_CapturePicPath;
	list<SearchResult> m_Result;

	CFont blackTopicFont;
	CFont blackStatusFont;

	enum TYPE { DataBasePic, ResultPic, CapturePic };
	afx_msg void OnBegin();
	afx_msg void OnStop();
	afx_msg void OnUp();
	afx_msg void OnDown();
	afx_msg void OnTest();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
