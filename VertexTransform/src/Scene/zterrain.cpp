#include "StdAfx.h"
#include "ZTerrain.h"
#include "dib.h"
#include "Graphics/Entity.h"
#include "Resource/ResourceMng.h"
#include "Graphics/RscIndexBuffer.h"
#include "Math/CollisionDetector.h"
#include "Math/Frustum.h"
#include "Foundation/Define.h"
#include "Graphics/Graphics.h"

namespace Sophia
{


/// ������
ZTerrain::ZTerrain()
{
	m_cxDIB			= 0;
	m_czDIB			= 0;
	m_cyDIB			= 0;
	m_vfScale		= D3DXVECTOR3(10.0f, 0.1f, 10.0f);
	m_pvHeightMap	= NULL;
	m_pRscIndexBuffer = NULL;
	m_pRscVertexBuffer = NULL;
	m_pTex = NULL;
	m_nTriangles	= 0;
	m_pQuadTree		= NULL;
}

/// �Ҹ���
ZTerrain::~ZTerrain()
{
	_Destroy();
}

/**
 * @brief ������ü�� �ʱ�ȭ�Ѵ�.
 * @param pDev : ��¿� D3D����̽� 
 * @param pvfScale : ���̸��� x,y,z���� ���� ô����(scale vector)
 * @param lpBMPFilename : ���̸ʿ� BMP���ϸ�
 * @param lpTexFilename : ��¿� D3D����̽� 
 */
HRESULT	ZTerrain::Create( D3DXVECTOR3* pvfScale, const char* lpBMPFilename, const char* lpTEXFilename )
{	
	if (pvfScale!=NULL)
	{
		m_vfScale = *pvfScale;
	}	
	
	try
	{
		if( FAILED( _BuildHeightMap( lpBMPFilename ) ) ) throw ("���� ���̸� BMP���Ͼ���\n");
		if( FAILED( _LoadTextures( lpTEXFilename ) ) ) throw ("���� �ؽ��� ���Ͼ���\n");
		if( FAILED( _CreateVIB() ) ) throw ("���ؽ� �ε��� ���� ��������\n");
	}
	catch (const char* msg)
	{
		OutputDebugString(msg);
		_Destroy(); 
		return E_FAIL;
	}
	m_pQuadTree = new ZQuadTree( this,0,m_cxDIB - 1,m_cxDIB * ( m_czDIB - 1 ), m_cxDIB * m_czDIB - 1 );
	AttachChildNode(m_pQuadTree);

	m_BoundingSphere = m_pQuadTree->GetBoundingSphere();
	QueueRenderer(this,true);
	return S_OK;
}

/// ������ü�� �޸𸮿��� �Ұ��Ѵ�.
HRESULT	ZTerrain::_Destroy()
{
	DettachChildNode(m_pQuadTree);
	SAFE_DELETE( m_pQuadTree );
	SAFE_DELETEARRAY( m_pvHeightMap );
	SAFE_RELEASE( m_pRscVertexBuffer );
	SAFE_RELEASE( m_pRscIndexBuffer );
	SAFE_RELEASE( m_pTex );

	return S_OK;
}

/// ������ü���� ����� �ؽ�ó�� �о���δ�.
HRESULT	ZTerrain::_LoadTextures( const char* lpTexFilename )
{	
	D3DXCreateTextureFromFile( Graphics::m_pDevice, lpTexFilename, &m_pTex );

	return S_OK;
}

/// BMP������ ��� ���̸��� �����Ѵ�.
HRESULT	ZTerrain::_BuildHeightMap( const char* lpFilename )
{
#ifdef _UNICODE

	char temp[256]={0,};
	WideCharToMultiByte(CP_ACP, 0, lpFilename, -1, temp, 256, NULL, NULL);
	LPBYTE	pDIB = DibLoadHandle( temp );

#else
	LPBYTE	pDIB = DibLoadHandle( lpFilename );
#endif
	
	if( !pDIB ) return E_FAIL;

	char filename[256];
	_splitpath_s(lpFilename,NULL,0,NULL,0,filename,256,NULL,0);
	m_strNodeName = filename;

	m_cxDIB = DIB_CX( pDIB );
	m_czDIB = DIB_CY( pDIB );
	
	// ���⼭ m_cxDIB�� m_czDIB�� (2^n+1)�� �ƴѰ�� E_FAIL�� ��ȯ�ϵ��� ������ ��

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



/// ����, �ε��� ���۸� �����Ѵ�.
HRESULT	ZTerrain::_CreateVIB()
{
 	m_pRscVertexBuffer = cResourceMng::m_pInstance->CreateRscVertexBuffer(m_strNodeName.c_str(),m_strNodeName.c_str(),m_cxDIB*m_czDIB*sizeof(TERRAINVERTEX));
	m_pRscVertexBuffer->AddRef();

 	if (m_pRscVertexBuffer == NULL)
 	{
		_Destroy();
		return E_FAIL;
 	}
	VOID* pVertices;
	pVertices=m_pRscVertexBuffer->Lock(m_pRscVertexBuffer->GetBufferSize(),0);
	if (pVertices == NULL)
	{
		_Destroy();
		return E_FAIL;
	}
	memcpy( pVertices, m_pvHeightMap, m_cxDIB*m_czDIB*sizeof(TERRAINVERTEX) );
	m_pRscVertexBuffer->Unlock();

	m_pRscIndexBuffer = cResourceMng::m_pInstance->CreateRscIndexBuffer(m_strNodeName.c_str(),m_strNodeName.c_str(),(m_cxDIB-1)*(m_czDIB-1)*2 * sizeof(WORD)*3);
	m_pRscIndexBuffer->AddRef();

	if (m_pRscIndexBuffer == NULL)
	{
		_Destroy();
		return E_FAIL;
	}	
    return S_OK;
}

/// ȭ�鿡 ������ ����Ѵ�.
void	ZTerrain::Render()
{	
	Graphics::m_pDevice->SetFVF( TERRAINVERTEX::FVF );
	m_pRscVertexBuffer->SetStreamSource(0,sizeof(TERRAINVERTEX));		
	m_pRscIndexBuffer->SetIndices();
	//�ؽ��� ����
	Graphics::m_pInstance->GetEffect()->SetTexture("Tex_Diffuse",m_pTex);	
	Graphics::m_pInstance->GetEffect()->SetMatrix(Graphics::m_pInstance->m_hmWorld,&m_matWorld);
	Graphics::m_pInstance->GetEffect()->CommitChanges();

	Graphics::m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_cxDIB * m_czDIB, 0, m_nTriangles );


	char temp[256];
	_itoa_s(m_nTriangles,temp,sizeof(temp),10);
	Graphics::m_pInstance->RenderDebugString(0,Graphics::m_pInstance->m_height-20,temp);
}

HRESULT ZTerrain::FillIndexBuffer(Frustum& frustum )
{
	LPDWORD		pI=NULL;
	pI=(LPDWORD)m_pRscIndexBuffer->Lock(0,m_pRscIndexBuffer->GetBufferSize(),0);	
	m_nTriangles=0;
	m_pQuadTree->GenTriIndex(frustum, m_nTriangles, pI,false );
	m_pRscIndexBuffer->Unlock();
	return S_OK;
}

void ZTerrain::Update( DWORD elapseTime )
{

}


BOOL ZTerrain::GetHeight( float x,float z,float& y )
{
	D3DXVECTOR3 pos(x,0,z);
	if(m_pQuadTree->GetCellIntersection(pos))
	{
		y=pos.y;
		return TRUE;
	}
	return FALSE;
}

bool ZTerrain::Cull(  Frustum* pFrustum ,float loose )
{
	if (!m_bShow)
		return false;

	cCollision::STATE retCS=cCollision::CheckWorldFrustum(*pFrustum,m_BoundingSphere,0.0f);
	if( retCS == cCollision::OUTSIDE)
		return false;

	FillIndexBuffer(*pFrustum);
	return true;	
}

void ZTerrain::QueueRenderer( Entity* pEntity,bool bTraversal )
{
	//pEntity->m_renderQueueTerrain.Insert(this,0);
}
}