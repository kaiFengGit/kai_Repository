
// TG_DynamicPic.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTG_DynamicPicApp:
// See TG_DynamicPic.cpp for the implementation of this class
//

class CTG_DynamicPicApp : public CWinApp
{
public:
	CTG_DynamicPicApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTG_DynamicPicApp theApp;