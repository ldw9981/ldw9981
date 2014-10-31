#include "StdAfx.h"
#include "Graphics.h"
#include "Foundation/Define.h"
#include "GUI/GUIFont.h"
#include "GUI/GUIButton.h"
#include "RscVertexBuffer.h"
#include "RscTexture.h"
#include "Vertex.h"
#include "MaterialEx.h"
#include "World.h"
#include "Foundation/Trace.h"

#define PI           3.14159265f
#define FOV          (PI/4.0f)							// �þ߰�
#define ASPECT_RATIO (1024/(float)768)		// ȭ���� ��Ⱦ��
#define NEAR_PLANE   1									// ���� ���
#define FAR_PLANE    10000								// ���Ÿ� ���

#define T_SIZE 256

#define SHADOWMAP_SIZE 4096
#define SET_TEXTURE_NULL 

namespace Sophia
{





Graphics*	Graphics::m_pInstance = NULL;
LPDIRECT3DDEVICE9 Graphics::m_pDevice;
Graphics::Graphics(void)
{
	m_pInstance = this;
/*
	m_viewPortInfo.X = 0;
	m_viewPortInfo.Y = 0;
	m_viewPortInfo.Width = 1024;
	m_viewPortInfo.Height = 768;	
	m_viewPortInfo.MinZ = 0.0f;
	m_viewPortInfo.MaxZ = 1.0f;*/

	m_bDebugBound = false;

	m_nTechniqueSize = (int)pow(2.0f,Material::MAX);
	for (int i=0;i<m_nTechniqueSize;i++)
	{	
		m_vecTechniqueNormal.push_back(NULL);
		m_vecTechniqueSkinned.push_back(NULL);
		m_vecTechniqueNormalInstancing.push_back(NULL);
		m_vecTechniqueSkinnedInstancing.push_back(NULL);
	}
}

Graphics::~Graphics(void)
{

}

void Graphics::SetPos( UINT x,UINT y )
{
	// �̹����� ��ġ, ������� ���� ���
	m_Rect.left = x;
	m_Rect.top = y;
	m_Rect.right = (LONG)x+T_SIZE;
	m_Rect.bottom = (LONG)y+T_SIZE;	

	// vertex�� ����

	g_vertices[0].vertex = D3DXVECTOR4( (float)m_Rect.left,(float)m_Rect.top,0.0f,0.0f);		
	g_vertices[0].tex.u = 0.0f;
	g_vertices[0].tex.v = 1.0f;

	g_vertices[1].vertex = D3DXVECTOR4( (float)m_Rect.right,(float)m_Rect.top,0.0f,0.0f);		
	g_vertices[1].tex.u = 1.0f;
	g_vertices[1].tex.v = 1.0f;

	g_vertices[2].vertex = D3DXVECTOR4( (float)m_Rect.right,(float)m_Rect.bottom,0.0f,0.0f);		
	g_vertices[2].tex.u = 1.0f;
	g_vertices[2].tex.v = 0.0f;

	g_vertices[3].vertex = D3DXVECTOR4( (float)m_Rect.left,(float)m_Rect.bottom,0.0f,0.0f);		
	g_vertices[3].tex.u = 0.0f;
	g_vertices[3].tex.v = 0.0f;				

}


bool Graphics::Init(HWND hWndPresent,bool bWindowed,int width,int height)
{
	HRESULT hr;
	m_width = width;
	m_height = height;	
	m_hWndPresent = hWndPresent;
	char szTemp[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,szTemp);

	// 1) D3D�� �����Ѵ�.
	m_pD3D9 = Direct3DCreate9( D3D_SDK_VERSION );
	V( m_pD3D9->GetDeviceCaps( D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&m_caps) );      


	m_vecRenderTarget.resize(m_caps.NumSimultaneousRTs,(LPDIRECT3DSURFACE9)NULL);


	// 2) Device�� ������ ���� Parameter�� �����Ѵ�.
	memset(&m_D3DPP,0,sizeof(m_D3DPP));
	if(bWindowed)
	{
		m_D3DPP.Windowed	 = true;
		m_D3DPP.BackBufferFormat	 = D3DFMT_UNKNOWN;
	}
	else
	{
		m_D3DPP.Windowed	 = false;
		m_D3DPP.BackBufferFormat = D3DFMT_X8R8G8B8;    // set the back buffer format to 32-bit
		m_D3DPP.BackBufferWidth = m_width;    // set the width of the buffer
		m_D3DPP.BackBufferHeight = m_height;    // set the height of the buffer
	}
	m_D3DPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; 
	m_D3DPP.SwapEffect	 = D3DSWAPEFFECT_DISCARD ;


	// Zbuffer���
	m_D3DPP.EnableAutoDepthStencil	= TRUE;		
	m_D3DPP.AutoDepthStencilFormat	= D3DFMT_D16;


	// 3) Device�� �����Ѵ�

	V( m_pD3D9->CreateDevice( 
		D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL, 
		hWndPresent,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,		
		&m_D3DPP, 
		&m_pDevice ));


	if( FAILED( hr ) )
		MessageBox(NULL,"Call to CreateDevice failed!", "ERROR",MB_OK|MB_ICONEXCLAMATION);



	V(m_pDevice->SetRenderState(D3DRS_ZENABLE,TRUE));	

	m_pNewFont = new cGUIFont();	

	m_pDevice->CreateVertexDeclaration(declNormal, &m_pNormalVertexDeclaration);
	m_pDevice->CreateVertexDeclaration(declBlend, &m_pSkinnedVertexDeclaration);
	m_pDevice->CreateVertexDeclaration(declNormalInstance, &m_pNormalInstancingVertexDeclaration);
	m_pDevice->CreateVertexDeclaration(declBlendInstance, &m_pSkinnedInstancingVertexDeclaration);

	
	SetPos(width-T_SIZE,0);

	return true;
}

void Graphics::Finalize()
{
	SAFE_RELEASE(m_pNormalVertexDeclaration);
	SAFE_RELEASE(m_pSkinnedVertexDeclaration)
	SAFE_RELEASE(m_pNormalInstancingVertexDeclaration);
	SAFE_RELEASE(m_pSkinnedInstancingVertexDeclaration);
	SAFE_DELETE(m_pNewFont);	
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pD3D9);
}


void Graphics::RenderDebugString(int x,int y,const char* szText)
{	
	m_pNewFont->DrawText(x,y,szText);
}

void Graphics::LoadHLSL(const char* szFileName)
{
	if (!szFileName)
	{
		return;
	}

	HRESULT hr;
	LPD3DXBUFFER pErr=NULL;
	if( FAILED( hr = D3DXCreateEffectFromFile(m_pDevice, szFileName, NULL, NULL,D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION , NULL, &m_pEffect, &pErr )))
	{
		MessageBox( NULL, (LPCTSTR)pErr->GetBufferPointer(), "ERROR", MB_OK);
		DXTrace(__FILE__, __LINE__, hr, _T("Error"), TRUE);
		return;
	}

	D3DXEFFECT_DESC desc;
	hr = m_pEffect->GetDesc(&desc);

	m_hmWorld = m_pEffect->GetParameterByName( NULL, "gWorldMatrix" );
	m_hmView = m_pEffect->GetParameterByName( NULL, "gViewMatrix" );
	m_hmProjection = m_pEffect->GetParameterByName( NULL, "gProjectionMatrix" );
	m_hmViewProjection = m_pEffect->GetParameterByName( NULL, "gViewProjectionMatrix" );
	m_hmPalette = m_pEffect->GetParameterByName( NULL, "Palette" );
	m_hvWorldLightPosition = m_pEffect->GetParameterByName( NULL, "gWorldLightPosition" );
	m_hvWorldCameraPosition = m_pEffect->GetParameterByName( NULL, "gWorldCameraPosition" );
	m_hmLightView = m_pEffect->GetParameterByName( NULL, "gLightViewMatrix" );
	m_hmLightProjection = m_pEffect->GetParameterByName( NULL, "gLightProjectionMatrix" );
	m_hfVertexTextureWidth = m_pEffect->GetParameterByName( NULL, "gVertexTextureWidth" );
	m_hfVertexTextureHeight = m_pEffect->GetParameterByName( NULL, "gVertexTextureHeight" );

	m_hTVertexTransform =					m_pEffect->GetTechniqueByName( _T("TVertexTransformation"));
	m_hTLine =								m_pEffect->GetTechniqueByName( _T("TLine") );
	m_hTerrain =							m_pEffect->GetTechniqueByName( _T("TTerrain") );
	m_hTPhong =								m_pEffect->GetTechniqueByName( _T("TPhong") );
	m_hTPhongDiffuse =						m_pEffect->GetTechniqueByName( _T("TPhongDiffuse") );
	m_hTPhongDiffuseLight =					m_pEffect->GetTechniqueByName( _T("TPhongDiffuseLight") );	
	m_hTPhongDiffuseBump =					m_pEffect->GetTechniqueByName( _T("TPhongDiffuseBump") );
	m_hTPhongDiffuseOpacity =				m_pEffect->GetTechniqueByName( _T("TPhongDiffuseOpacity") );
	m_hTPhongDiffuseSpecular =				m_pEffect->GetTechniqueByName( _T("TPhongDiffuseSpecular") );
	m_hTPhongDiffuseBumpSpecular =			m_pEffect->GetTechniqueByName( _T("TPhongDiffuseBumpSpecular") );

	m_hTPhongDiffuseInstancing =			m_pEffect->GetTechniqueByName( _T("TPhongDiffuseInstancing") );
	m_hTPhongDiffuseOpacityInstancing =		m_pEffect->GetTechniqueByName( _T("TPhongDiffuseOpacityInstancing") );
	m_hTPhongDiffuseLightInstancing =		m_pEffect->GetTechniqueByName( _T("TPhongDiffuseLightInstancing") );

	m_hTSkinningPhong =						m_pEffect->GetTechniqueByName( _T("TSkinningPhong") );	
	m_hTSkinningPhongDiffuse =				m_pEffect->GetTechniqueByName( _T("TSkinningPhongDiffuse") );	
	m_hTSkinningPhongDiffuseInstancing =	m_pEffect->GetTechniqueByName( _T("TSkinningPhongDiffuseInstancing") );

	m_hTShadowNormalNotAlphaTest =			m_pEffect->GetTechniqueByName( _T("TShadowNormalNotAlphaTest") );
	m_hTShadowNormalAlphaTest =				m_pEffect->GetTechniqueByName( _T("TShadowNormalAlphaTest") );
	m_hTShadowNormalNotAlphaTestInstancing=	m_pEffect->GetTechniqueByName( _T("TShadowNormalNotAlphaTestInstancing") );
	m_hTShadowNormalAlphaTestInstancing =	m_pEffect->GetTechniqueByName( _T("TShadowNormalAlphaTestInstancing") );

	m_hTShadowSkinnedNotAlphaTest =			m_pEffect->GetTechniqueByName( _T("TShadowSkinnedNotAlphaTest") );	
	m_hTShadowSkinnedAlphaTest =			m_pEffect->GetTechniqueByName( _T("TShadowSkinnedAlphaTest") );
	m_hTShadowSkinnedNotAlphaTestInstancing=m_pEffect->GetTechniqueByName( _T("TShadowSkinnedNotAlphaTestInstancing") );	
	m_hTShadowSkinnedAlphaTestInstancing=	m_pEffect->GetTechniqueByName( _T("TShadowSkinnedAlphaTestInstancing") );

	m_hTGUI = m_pEffect->GetTechniqueByName( _T("TGUI") );




	SAFE_RELEASE(pErr);


	std::bitset<Material::MAX> indexRenderQueue;

	m_vecTechniqueNormal[indexRenderQueue.to_ulong()] = m_hTPhong;

	indexRenderQueue.set(Material::DIFFUSE);
	m_vecTechniqueNormal[indexRenderQueue.to_ulong()] = m_hTPhongDiffuse;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::NORMAL);
	m_vecTechniqueNormal[indexRenderQueue.to_ulong()] = m_hTPhongDiffuseBump;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::LIGHT);
	m_vecTechniqueNormal[indexRenderQueue.to_ulong()] = m_hTPhongDiffuseLight;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::OPACITY);
	m_vecTechniqueNormal[indexRenderQueue.to_ulong()] = m_hTPhongDiffuseOpacity;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::SPECULAR);
	m_vecTechniqueNormal[indexRenderQueue.to_ulong()] = m_hTPhongDiffuseSpecular;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::NORMAL);
	indexRenderQueue.set(Material::SPECULAR);
	m_vecTechniqueNormal[indexRenderQueue.to_ulong()] = m_hTPhongDiffuseBumpSpecular;


	for (int i=0;i<m_nTechniqueSize;i++)
	{	
		if (m_vecTechniqueNormal[i] == NULL )	
			m_vecTechniqueNormal[i] = m_hTPhongDiffuse;
	}

	indexRenderQueue = 0;
	m_vecTechniqueSkinned[indexRenderQueue.to_ulong()] = m_hTSkinningPhong;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	m_vecTechniqueSkinned[indexRenderQueue.to_ulong()] = m_hTSkinningPhongDiffuse;

	for (int i=0;i<m_nTechniqueSize;i++)
	{	
		if (m_vecTechniqueSkinned[i] == NULL )	
			m_vecTechniqueSkinned[i] = m_hTSkinningPhongDiffuse;
	}


	// m_vecTechniqueNormalInstancing
	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	m_vecTechniqueNormalInstancing[indexRenderQueue.to_ulong()] = m_hTPhongDiffuseInstancing;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::OPACITY);
	m_vecTechniqueNormalInstancing[indexRenderQueue.to_ulong()] = m_hTPhongDiffuseOpacityInstancing;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::LIGHT);
	m_vecTechniqueNormalInstancing[indexRenderQueue.to_ulong()] = m_hTPhongDiffuseLightInstancing;
	

	for (int i=0;i<m_nTechniqueSize;i++)
	{	
		if (m_vecTechniqueNormalInstancing[i] == NULL )	
			m_vecTechniqueNormalInstancing[i] = m_hTPhongDiffuseInstancing;
	}

	// m_vecTechniqueSkinnedInstancing
	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	m_vecTechniqueSkinnedInstancing[indexRenderQueue.to_ulong()] = m_hTSkinningPhongDiffuseInstancing;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::OPACITY);
	m_vecTechniqueSkinnedInstancing[indexRenderQueue.to_ulong()] = m_hTPhongDiffuseOpacityInstancing;

	indexRenderQueue = 0;
	indexRenderQueue.set(Material::DIFFUSE);
	indexRenderQueue.set(Material::LIGHT);
	m_vecTechniqueSkinnedInstancing[indexRenderQueue.to_ulong()] = m_hTPhongDiffuseLightInstancing;


	for (int i=0;i<m_nTechniqueSize;i++)
	{	
		if (m_vecTechniqueSkinnedInstancing[i] == NULL )	
			m_vecTechniqueSkinnedInstancing[i] = m_hTPhongDiffuseInstancing;
	}


}


void Graphics::Begin()
{
	m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );
	m_pDevice->BeginScene();	
#ifdef SET_TEXTURE_NULL
	for (int i=0; i< 16; i++) 
		m_pDevice->SetTexture(i, NULL);
#endif	
}

void Graphics::End()
{
	m_pDevice->EndScene();
	m_pDevice->Present( NULL, NULL, m_hWndPresent, NULL );	
}

void Graphics::SetEffectVector_WorldLightPosition(D3DXVECTOR4* pVec )
{
	m_pEffect->SetVector(m_hvWorldLightPosition,pVec);
}

void Graphics::SetEffectMatirx_LightView(D3DXMATRIX* pMat )
{
	m_pEffect->SetMatrix(m_hmLightView, pMat);
}

void Graphics::SetEffectMatirx_LightProjection(D3DXMATRIX* pMat )
{
	m_pEffect->SetMatrix(m_hmLightProjection,pMat);	
}

void Graphics::BackupRenderTarget( unsigned int renderTargetIndex )
{
	HRESULT hr;
	V( m_pDevice->GetRenderTarget(renderTargetIndex, &m_vecRenderTarget[renderTargetIndex]) );
}

void Graphics::RestoreRenderTarget( unsigned int renderTargetIndex )
{
	if (m_vecRenderTarget[renderTargetIndex] == NULL)
		return;

	m_pDevice->SetRenderTarget(renderTargetIndex,m_vecRenderTarget[renderTargetIndex]);
	m_vecRenderTarget[renderTargetIndex]->Release();
	m_vecRenderTarget[renderTargetIndex] = NULL;
}

void Graphics::BackupDepthStencilSurface()
{
	HRESULT hr;
	V( Graphics::m_pDevice->GetDepthStencilSurface(&m_depthStencilSurface) );	
}

void Graphics::RestoreDepthStencilSurface()
{
	Graphics::m_pDevice->SetDepthStencilSurface(m_depthStencilSurface);	
	m_depthStencilSurface->Release();
	m_depthStencilSurface = NULL;
}

}