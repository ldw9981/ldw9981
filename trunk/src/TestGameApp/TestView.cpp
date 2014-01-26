#include "StdAfx.h"
#include "TestView.h"
#include "Scene/ViewMng.h"

#include "GUI/GUIButton.h"
#include "TestGameApp.h"
#include "Ground.h"

#include "Ground.h"
#include "BackWall.h"
#include "TestView.h"
#include "MenuView.h"
#include "GlobalView.h"
#include "Framework/D3DFramework.h"
#include "ASEParser/ASEParser.h"

#include "Scene/ZTerrain.h"
#include "TestView.h"
#include "TestStateA.h"
#include "TestStateB.h"
#include "GlobalView.h"
#include "ObjTank.h"
#include "Graphics/Graphics.h"

cTestView::cTestView(void)
{
	m_pTestStateA=NULL;
	m_pTestStateB=NULL;
	m_pGlobalButtonScene=NULL;
	m_pP38=NULL;
}

cTestView::~cTestView(void)
{

}


void cTestView::Enter()
{
	SetViewPortInfo(0,0,g_pApp->GetRequestRectWidth(),g_pApp->GetRequestRectHeight());
	m_Camera.SetActive();
	m_Camera.SetPerspective(D3DXToRadian(45),1.0f,10000.0f,
		(float)g_pApp->GetRequestRectWidth(),(float)g_pApp->GetRequestRectHeight());
	m_Camera.SetLookAt(&D3DXVECTOR3(0.0f, 0.0f, -1.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 1.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));		

	m_Camera.SetLocalPos(D3DXVECTOR3(0.0f,0.0f,-1000.0f));
	m_Camera.SetProcessInput(true);

	std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
	cASEParser parser;
	m_pP38 = new cObjTank;	
	parser.Load(std::string(strDataPath+"Light Map.ase").c_str(),m_pP38);
	parser.Close();
	m_pP38->Build();
	m_pP38->Init();
	m_pP38->SetVelocityRotation(D3DXVECTOR3(0.0f,-45,0.0f));
	AttachObject(m_pP38);

	m_pTestStateA = new TestStateA;
	m_pTestStateA->SetParentView(this);	

	m_pTestStateB = new TestStateB;
	m_pTestStateB->SetParentView(this);
	
	
	m_pGlobalButtonScene = new cGlobalView;
	m_pGlobalButtonScene->SetParentView(this);
	m_pGlobalButtonScene->Enter();
	AttachObject(m_pGlobalButtonScene);		
}

void cTestView::Leave()
{
	m_pGlobalButtonScene->Leave();
	DettachObject(m_pGlobalButtonScene);
	SAFE_DELETE(m_pGlobalButtonScene);		

	cView* pView = GetState().GetCurr();
	if (pView)
	{
		pView->Leave();
	}
	SAFE_DELETE(m_pTestStateB);	
	SAFE_DELETE(m_pTestStateA);	

	DettachObject(m_pP38);
	SAFE_DELETE(m_pP38);

}

void cTestView::Control()
{	
	cView::Control();

	if (g_pInput->IsTurnDn(DIK_SPACE))
	{
		TestGameApp* p=(TestGameApp*)g_pApp;
			
		m_pGlobalButtonScene->SetHide(m_pGlobalButtonScene->GetHide());
	}
	if (g_pInput->IsTurnDn(DIK_EQUALS))
	{
		Graphics::g_pGraphics->m_WorldLightPosition.y += 50;

	}

	if (g_pInput->IsTurnDn(DIK_MINUS))
	{
		Graphics::g_pGraphics->m_WorldLightPosition.y -= 50;
	}


	if (g_pInput->IsTurnDn(DIK_F12))
	{
		Graphics::g_pGraphics->m_bDebugBound = !Graphics::g_pGraphics->m_bDebugBound;
	}
	
	m_Camera.Control();
}

void cTestView::Notify( cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam )
{
	if (pSource== m_pGlobalButtonScene->m_pBtNextScene)
	{
		m_Camera.SetProcessInput(false);

		TestGameApp* p = (TestGameApp*)g_pApp;
		
		if (GetState().IsCurr(m_pTestStateB))
		{
			GetState().Transite(m_pTestStateA);
		}
		else 
		{
			DettachObject(m_pP38);
			SAFE_DELETE(m_pP38);

			GetState().Transite(m_pTestStateB);
		}		
	}
	else if (pSource == m_pGlobalButtonScene->m_pBtExit)
	{
		g_pApp->QuitLoop();
	}
}

void cTestView::Update( DWORD elapseTime )
{
	cView::Update(elapseTime);
	m_Camera.Update(elapseTime);
}

void cTestView::ProcessRender()
{
	m_Camera.Render();
	cView::ProcessRender();
}