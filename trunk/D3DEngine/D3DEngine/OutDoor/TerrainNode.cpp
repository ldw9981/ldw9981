#include "StdAfx.h"
#include "TerrainNode.h"
#include "../dib/dib.h"
#include "../Framework/D3DFramework.h"
#include "../Log/DebugInfoScene.h"

#include "../Resource/ResourceMng.h"
#include "../Resource/RscIndexBuffer.h"
#include "../Resource/RscTexture.h"
#include "../Geometrics/CollisionDetector.h"

/// 생성자
cTerrainNode::cTerrainNode()
{
	m_cxDIB			= 0;
	m_czDIB			= 0;
	m_cyDIB			= 0;
	m_vfScale		= D3DXVECTOR3(10.0f, 0.1f, 10.0f);
	m_pvHeightMap	= NULL;
	
//	for( int i = 0 ; i < MAX_TERRAIN_TEX ; i++ ) m_pTex[i] = NULL;
	m_pTex = NULL;
	m_nTriangles	= 0;
	m_pQuadTree		= NULL;
}

/// 소멸자
cTerrainNode::~cTerrainNode()
{
	Destroy();
}

/**
 * @brief 지형객체를 초기화한다.
 * @param pDev : 출력용 D3D디바이스 
 * @param pvfScale : 높이맵의 x,y,z값에 곱할 척도값(scale vector)
 * @param lpBMPFilename : 높이맵용 BMP파일명
 * @param lpTexFilename : 출력용 D3D디바이스 
 */
HRESULT	cTerrainNode::Create( D3DXVECTOR3* pvfScale, LPCTSTR lpBMPFilename, LPCTSTR lpTEXFilename )
{		
	if (pvfScale!=NULL)
	{
		m_vfScale = *pvfScale;
	}	

	try
	{
		if( FAILED( BuildHeightMap( lpBMPFilename ) ) ) throw _T("지형 높이맵 BMP파일없음\n");
		if( FAILED( LoadTextures( lpTEXFilename ) ) ) throw _T("지형 텍스쳐 파일없음\n");
		if( FAILED( CreateVIB() ) ) throw _T("버텍스 인덱스 버퍼 생성에러\n");
	}
	catch (LPCTSTR msg)
	{
		::MessageBox(NULL,msg,MB_OK,0);
		Destroy(); 
		return E_FAIL;
	}
	m_pQuadTree = new cQuadTreeNode( this,0,m_cxDIB - 1,m_cxDIB * ( m_czDIB - 1 ), m_cxDIB * m_czDIB - 1 );
	AttachChildNode(m_pQuadTree);

	CreateCullingSphere();
	(*GetCullingSphere())=*m_pQuadTree->GetCullingSphere();
	CreateBoundingSphere();
	(*GetBoundingSphere())=*m_pQuadTree->GetCullingSphere();

	return S_OK;
}

/// 지형객체를 메모리에서 소거한다.
HRESULT	cTerrainNode::Destroy()
{
	DettachChildNode(m_pQuadTree);
	SAFE_DELETE( m_pQuadTree );
	SAFE_DELETEARRAY( m_pvHeightMap );
	SAFE_RELEASE( m_pRscVertexBuffer );
	SAFE_RELEASE( m_pRscIndexBuffer );
//	for( int i = 0 ; i < 4 ; i++ ) SAFE_RELEASE( m_pTex[i] );
	SAFE_RELEASE(m_pTex);
	return S_OK;
}

/// 지형객체에서 사용할 텍스처를 읽어들인다.
HRESULT	cTerrainNode::LoadTextures( LPCTSTR lpTexFilename )
{
// 	for( int i = 0 ; i < 1 ; i++ )
// 		D3DXCreateTextureFromFile( m_pD3DDevice, lpTexFilename[i], &m_pTex[i] );

//	D3DXCreateTextureFromFile( m_pD3DDevice, lpTexFilename, &m_pTex );

	m_pRscTexture = g_pD3DFramework->GetResourceMng()->CreateRscTexture(lpTexFilename);
	if (m_pRscTexture==NULL)
	{
		return E_FAIL;
	}
	return S_OK;
}

/// BMP파일을 열어서 높이맵을 생성한다.
HRESULT	cTerrainNode::BuildHeightMap( LPCTSTR lpFilename )
{	
#ifdef _UNICODE
	char temp[256]={0,};
	memset(temp,0,sizeof(temp));
	WideCharToMultiByte(CP_ACP, 0, lpFilename, -1, temp, 256, NULL, NULL);	
	LPBYTE	pDIB = DibLoadHandle( temp );	
#else
	LPBYTE	pDIB = DibLoadHandle( lpFilename );
#endif
	if( pDIB==NULL ) 
	{
		return E_FAIL;
	}
	
	m_cxDIB = DIB_CX( pDIB );
	m_czDIB = DIB_CY( pDIB );
	
	// 여기서 m_cxDIB나 m_czDIB가 (2^n+1)이 아닌경우 E_FAIL을 반환하도록 수정할 것

	m_pvHeightMap = new TERRAINVERTEX[m_cxDIB * m_czDIB];

	TERRAINVERTEX v;
	for( int z = 0 ; z < m_czDIB ; z++ )
	{
		for( int x = 0 ; x < m_cxDIB ; x++ )
		{
			v.p.x = (float)( ( x - m_cxDIB / 2 ) * m_vfScale.x );
			v.p.z = -(float)( ( z - m_czDIB / 2 ) * m_vfScale.z );
			v.p.y = (float)( *( DIB_DATAXY_INV( pDIB, x, z ) ) ) * m_vfScale.y;
			D3DXVec3Normalize( &v.n, &v.p );
			v.u = (float)x / (float)( m_cxDIB - 1 );
			v.v = (float)z / (float)( m_czDIB - 1 );
			m_pvHeightMap[x + z * m_czDIB] = v;
		}
	}

	DibDeleteHandle( pDIB );
	return S_OK;
}



/// 정점, 인덱스 버퍼를 생성한다.
HRESULT	cTerrainNode::CreateVIB()
{
 	m_pRscVertexBuffer = g_pD3DFramework->GetResourceMng()->CreateRscVertexBuffer(m_cxDIB*m_czDIB*sizeof(TERRAINVERTEX));
 	if (m_pRscVertexBuffer == NULL)
 	{
		Destroy();
		return E_FAIL;
 	}
	VOID* pVertices;
	pVertices=m_pRscVertexBuffer->Lock();
	if (pVertices == NULL)
	{
		Destroy();
		return E_FAIL;
	}
	memcpy( pVertices, m_pvHeightMap, m_cxDIB*m_czDIB*sizeof(TERRAINVERTEX) );
	m_pRscVertexBuffer->Unlock();

	m_pRscIndexBuffer = g_pD3DFramework->GetResourceMng()->CreateRscIndexBuffer((m_cxDIB-1)*(m_czDIB-1)*2 * sizeof(WORD)*3);
	if (m_pRscIndexBuffer == NULL)
	{
		Destroy();
		return E_FAIL;
	}	
    return S_OK;
}

/// 화면에 지형을 출력한다.
void	cTerrainNode::Render()
{
	FillIndexBuffer();
	m_pRscVertexBuffer->SetStreamSource(sizeof(TERRAINVERTEX));		
	m_pRscIndexBuffer->SetIndices();

//	m_pD3DDevice->SetTexture( 0, m_pTex );								// 0번 텍스쳐 스테이지에 텍스쳐 고정(색깔맵)
	m_pRscTexture->SetTexture(0);

	m_pD3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );	// 0번 텍스처 스테이지의 확대 필터
 	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );		// 0번 텍스처 : 0번 텍스처 인덱스 사용
 	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
 	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
 	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	


	m_pD3DDevice->SetFVF( TERRAINVERTEX::FVF );
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &GetWorldTM() );	
	m_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_cxDIB * m_czDIB, 0, m_nTriangles );

}

HRESULT cTerrainNode::FillIndexBuffer()
{
	cCameraNode* pCamera=cCameraNode::GetActiveCamera();

	LPDWORD		pI=NULL;
	pI=(LPDWORD)m_pRscIndexBuffer->Lock();	
	m_nTriangles=0;
	m_pQuadTree->GenTriIndex(pCamera, m_nTriangles, pI );
	m_pRscIndexBuffer->Unlock();

	g_pD3DFramework->GetDebugInfoScene()->AddDebugString(_T("현재 지형 Tri %d\n"),m_nTriangles);
	
	return S_OK;
}

void cTerrainNode::Update( DWORD elapseTime )
{

}

void cTerrainNode::Init()
{

}

void cTerrainNode::Uninit()
{

}

BOOL cTerrainNode::GetHeight( float x,float z,float& y )
{
	D3DXVECTOR3 pos(x,0,z);
	if(m_pQuadTree->GetCellIntersection(pos))
	{
		y=pos.y;
		return TRUE;
	}
	return FALSE;
}

/*
	SceneGraphNode의 함수와 다른것은 CheckWorldFrustumWithoutYAxis를 쓴다는것
*/
void cTerrainNode::CullRendererTraversal( cRendererQueue* pRendererQueue,cCameraNode* pActiveCamera )
{
	IRenderer* pIRenderer=dynamic_cast<IRenderer*>(this);

	if (pIRenderer!=NULL)
	{		
		if (m_pCullingSphere!=NULL)
		{			
			int ret=pActiveCamera->CheckWorldFrustumWithoutYAxis(m_pCullingSphere);
			if( ret == cCollision::OUTSIDE)
			{	//  밖에 있는것이면 노드순회 없음
				return;
			}
			else if (ret == cCollision::INSIDE)
			{	// 완전히 내부면 자식은 모두 큐에 넣고 순회없음
				InsertToRendererQueueTraversal(pRendererQueue,pActiveCamera);
				return;
			}
		}

		// cCollision::INTERSECT 겹치면 자신의 바운딩 스피어랑 검사. 
		if (m_pBoundingSphere!=NULL)
		{
			int ret=pActiveCamera->CheckWorldFrustumWithoutYAxis(m_pBoundingSphere);
			if( ret != cCollision::OUTSIDE)	// INTERSECT or INSIDE는 큐에 넣는다.
			{	
				pRendererQueue->Insert(pIRenderer);						
			}
		}
	}	

	list<cSceneGraphNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->CullRendererTraversal(pRendererQueue,pActiveCamera);
	}
}