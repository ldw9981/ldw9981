#include "StdAfx.h"
#include "TestGameApp.h"

#include "Resource/ResourceMng.h"
#include "Graphics/RscTexture.h"

#include "Scene/ViewMng.h"
#include "TestView.h"
#include "MenuView.h"
#include "Framework/D3DFramework.h"
#include "Graphics/Graphics.h"
#include "Graphics/MaterialEx.h"
#include "Foundation/HeapValidator.h"


TestGameApp::TestGameApp( const char* szTitleName,BOOL bFullScreen,int nWidth,int nHeight)
:cD3DFramework(szTitleName,bFullScreen,nWidth,nHeight)
{

	
}

TestGameApp::~TestGameApp(void)
{
}

bool TestGameApp::Initialize()
{
	if(!cD3DFramework::Initialize())
		return false;
	
	std::string Path;
	Path = EnvironmentVariable::GetInstance().GetString("CurrPath");

	std::string::size_type index=Path.length();
	for (int i=0;i<3;i++)
	{
		index=Path.rfind("\\",index-1,1);
	}	
	Path  = Path.substr ( 0 ,index+1);		
	Path  += "Data\\";

	EnvironmentVariable::GetInstance().SetString("DataPath",Path.c_str());
	

	std::string strHLSL=Path;
	strHLSL+= "hlsl.fx";
	m_pGraphics->LoadHLSL(strHLSL.c_str());
	
	m_pView = new cMenuView;
	m_pView->SetViewPortInfo(0,0,GetRequestRectWidth(),GetRequestRectHeight());
	AttachObject(m_pView);
	m_pView->Enter();

	
	return true;
}

void TestGameApp::Finalize()
{
	m_pView->Leave();
	DettachObject(m_pView);
	SAFE_DELETE(m_pView);
	cD3DFramework::Finalize();
}


BOOL TestGameApp::OnWM_Keyboard( MSG& msg )
{
	return FALSE;
}


BOOL TestGameApp::OnWM_Mouse( MSG& msg )
{
	return FALSE;
}

BOOL TestGameApp::OnWM_General( MSG& msg )
{
	switch(msg.message)
	{
	case WM_ACTIVATEAPP:
		if (!msg.wParam)
		{
			//MessageBox(NULL,"the window is being deactivated.","�˸�",MB_OK);
		}
		else
		{
			//MessageBox(NULL,"the window is being activated.","�˸�",MB_OK);
		}
		return TRUE;
	
	}
	return FALSE;
}



int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	HeapValidator::SetDbgFlag();

	TestGameApp TestGameApp("TestGameApp",false,1024,768);
	if(TestGameApp.Initialize())
	{
		TestGameApp.Run();
	}
	TestGameApp.Finalize();
	
	HeapValidator::CheckMemory();
	return 0;
}