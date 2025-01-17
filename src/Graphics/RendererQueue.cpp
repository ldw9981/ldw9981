#include "StdAfx.h"
#include "RendererQueue.h"
#include "Foundation/Interface.h"
#include "Graphics/Graphics.h"
#include "Graphics/MaterialEx.h"
#include "Graphics/CameraNode.h"
#include "Graphics/RscTexture.h"
#include "Graphics/MeshNode.h"
#include "Graphics/SkinnedMeshNode.h"


#include "Graphics/MatrixTexture.h"

#include "Graphics/Vertex.h"
#include "Foundation/Trace.h"

namespace Sophia
{


cRendererQueue::cRendererQueue()
{
	
}

cRendererQueue::~cRendererQueue()
{
	
}


static bool GreateDistance(cRendererQueue::MESH_DISTANCE_PAIR& a,cRendererQueue::MESH_DISTANCE_PAIR& b)
{
	if ( a.second > b.second)
		return true;

	return false;
}

void cRendererQueue::GatherRenderAlphaBlend(std::vector<cMeshNode*>& vecMesh , D3DXVECTOR3* pCameraWorldPosition )
{
	auto it = vecMesh.begin();
	auto it_end = vecMesh.end();
	D3DXVECTOR3 temp;
	for ( ; it!=it_end ; ++it )
	{		
		auto& pMesh = *it;		
		temp = *pCameraWorldPosition - *(pMesh->GetWorldPositionPtr());		
		m_distanceOrder.push_back(MESH_DISTANCE_PAIR(pMesh,D3DXVec3LengthSq(&temp)));
	}
}



void cRendererQueue::Clear()
{
	m_materialOrder.clear();
	m_sceneOrder.clear();
	m_distanceOrder.clear();
}


void cRendererQueue::RenderAlphaBlendByDistanceOrder(DWORD elapseTime,std::vector<D3DXHANDLE>& vecTechnique)
{
	//그릴때 한번에 정렬
	std::sort(m_distanceOrder.begin(),m_distanceOrder.end(),&GreateDistance);

	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true); 	
	Graphics::m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Graphics::m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();		

	std::vector<MESH_DISTANCE_PAIR>	containerTemp;

	Material* pPrevMaterial=NULL;
	for ( auto it=m_distanceOrder.begin() ; it!=m_distanceOrder.end(); ++it )
	{		
		MESH_DISTANCE_PAIR& item = *it;		
		Material* pCurrMaterial = item.first->GetMaterial();
		if (pPrevMaterial!= pCurrMaterial && !containerTemp.empty())
		{
			SubRenderAlphaBlend(elapseTime,vecTechnique,containerTemp);
			containerTemp.clear();
		}		
		containerTemp.push_back(*it);				
		pPrevMaterial = pCurrMaterial;
	}
	SubRenderAlphaBlend(elapseTime,vecTechnique,containerTemp);

	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false); 	
	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}

void cRendererQueue::SubRenderAlphaBlend(DWORD elapseTime, std::vector<D3DXHANDLE>& vecTechnique,std::vector<MESH_DISTANCE_PAIR>& containerTemp )
{
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();	
	UINT passes = 0;		

	for ( auto it=containerTemp.begin() ; it!=containerTemp.end(); ++it )
	{		
		MESH_DISTANCE_PAIR& item = *it;		
		Material* pMaterial = item.first->GetMaterial();
		
		int i = pMaterial->index_renderer_queue();
		pEffect->SetTechnique(vecTechnique[i]);
		pEffect->Begin(&passes, 0);	
		// Material적용
		ChangeMaterial(pMaterial,false);

		pEffect->BeginPass(0);	
		(*it).first->Render(elapseTime);

		pEffect->EndPass();
		pEffect->End();	
	}
}

void cRendererQueue::RenderNotAlphaBlendByMaterialOrder(DWORD elapseTime,std::vector<D3DXHANDLE>& vecTechnique)
{
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();
	
	
	Material* pPrevMaterial=NULL;

	for ( auto it = m_materialOrder.begin() ; it!=m_materialOrder.end();++it)
	{
		Material* pMaterial = it->first;
		int i = pMaterial->index_renderer_queue();
		pEffect->SetTechnique(vecTechnique[i]);

		ChangeMaterial(pMaterial,false);

		UINT passes = 0;		
		pEffect->Begin(&passes, 0);	
		pEffect->BeginPass(0);	
		MESHPTR_CONTAINER& vecMesh = it->second;
		for (auto it_sub = vecMesh.begin() ; it_sub!=vecMesh.end();++it_sub)
		{
			(*it_sub)->Render(elapseTime);
		}
		pEffect->EndPass();
		pEffect->End();		
	}
	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}

void cRendererQueue::ChangeMaterial(Material* pMaterial ,bool textureOpacityOnly)
{
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();


	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, pMaterial->AlphaTestEnable); 	
	if (pMaterial->AlphaTestEnable)
	{
		Graphics::m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		Graphics::m_pDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)pMaterial->AlphaTestRef);
	}

	cRscTexture* pRscTexture;
	pRscTexture = pMaterial->GetMapOpacity();
	if( pRscTexture != NULL )	
		pEffect->SetTexture("Tex_Opacity",pRscTexture->GetD3DTexture());

	if (textureOpacityOnly)
		return;

	pRscTexture = pMaterial->GetMapDiffuse();
	if( pRscTexture != NULL )	
		pEffect->SetTexture("Tex_Diffuse",pRscTexture->GetD3DTexture());

	pRscTexture = pMaterial->GetMapNormal();
	if( pRscTexture != NULL )	
		pEffect->SetTexture("Tex_Normal",pRscTexture->GetD3DTexture());

	pRscTexture = pMaterial->GetMapLight();
	if( pRscTexture != NULL )	
		pEffect->SetTexture("Tex_Light",pRscTexture->GetD3DTexture());

	pRscTexture = pMaterial->GetMapSpecular();
	if( pRscTexture != NULL )	
		pEffect->SetTexture("Tex_Specular",pRscTexture->GetD3DTexture());	
}



void cRendererQueue::RenderShadowByMaterialOrder(DWORD elapseTime, D3DXHANDLE hTShadowNotAlphaTest,D3DXHANDLE hTShadowAlphaTest )
{
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();

	UINT passes = 0;		
	Material* pPrevMaterial=NULL;

	for ( auto it = m_materialOrder.begin() ; it!=m_materialOrder.end();++it)
	{
		Material* pMaterial = it->first;

		if (pMaterial->AlphaTestEnable)
			pEffect->SetTechnique(hTShadowAlphaTest);
		else
			pEffect->SetTechnique(hTShadowNotAlphaTest );
		
		ChangeMaterial(pMaterial,true);		

		pEffect->Begin(&passes, 0);	
		pEffect->BeginPass(0);	
		MESHPTR_CONTAINER& vecMesh = it->second;
		for (auto it_sub = vecMesh.begin() ; it_sub!=vecMesh.end();++it_sub)
		{
			(*it_sub)->Render(elapseTime);
		}
		pEffect->EndPass();
		pEffect->End();		
	}
	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}

void cRendererQueue::GatherRender(std::vector<cMeshNode*>& vecMesh )
{
	size_t size = vecMesh.size();
	for (size_t i=0;i<size;i++)
	{
		cMeshNode* pMesh = vecMesh[i];	

		Material* pMaterial = pMesh->GetMaterial();
		if (pMesh->GetInstancingEnable())
		{
			SCENE_KEY key(pMesh->GetRscVetextBuffer(),pMesh->GetMaterial(),pMesh->GetRscIndexBuffer());
			MESHPTR_CONTAINER& list = m_sceneOrder[key];
			list.push_back(pMesh);
		}
		else
		{
			MESHPTR_CONTAINER& list = m_materialOrder[pMaterial];
			list.push_back(pMesh);
		}
	}
}




void cRendererQueue::RenderNotAlphaBlendNormalInstancing(DWORD elapseTime, std::vector<D3DXHANDLE>& vecTechnique )
{
	HRESULT hr;
	HR_V( Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pNormalInstancingVertexDeclaration) );
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();
	
	
	for ( auto it = m_sceneOrder.begin() ; it!=m_sceneOrder.end();++it)
	{	
		const SCENE_KEY& refScene = it->first;
		MESHPTR_CONTAINER& list = it->second;

		// Set Matrix Instance
		unsigned long nCount=list.size();
		auto it_sub = list.begin();

		cMeshNode* pMeshNode = *it_sub;

		MatrixTexture* pMatrixInstancingTexture = pMeshNode->GetMatrixInstancingTexture();			

		if (!pMatrixInstancingTexture->GetValid())
		{
			D3DLOCKED_RECT lock;
			pMatrixInstancingTexture->GetD3DTexture()->LockRect(0, &lock, NULL, D3DLOCK_DISCARD);
			pMeshNode->UpdateMatrixInstancing(list, lock);
			pMatrixInstancingTexture->GetD3DTexture()->UnlockRect(0);
			pMatrixInstancingTexture->SetValid(true);	//다음 프레임에 다시 계산
		}

		pEffect->SetTexture("Tex_MatrixInstanceData",pMatrixInstancingTexture->GetD3DTexture());
		pEffect->SetFloat(Graphics::m_pInstance->m_hfMatrixTextureSize,(float)pMatrixInstancingTexture->GetSize());
		int i = refScene.pMaterial->index_renderer_queue();
		HR_V(pEffect->SetTechnique(vecTechnique[i]));
		ChangeMaterial(refScene.pMaterial,false);

		pEffect->CommitChanges();		
		
		UINT passes;
		pEffect->Begin(&passes, 0);	
		pEffect->BeginPass(0);	
		pMeshNode->RenderInstancing(nCount);
		pEffect->EndPass();
		pEffect->End();		

		pMatrixInstancingTexture->SetValid(false);
	}
	Graphics::m_pDevice->SetStreamSourceFreq( 0, 1 );
	Graphics::m_pDevice->SetStreamSourceFreq( 1, 1 );
	HR_V(Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false));		
}

void cRendererQueue::RenderShadowNormalInstancing(DWORD elapseTime, D3DXHANDLE hTShadowNotAlphaTest,D3DXHANDLE hTShadowAlphaTest )
{
	HRESULT hr;
	HR_V( Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pNormalInstancingVertexDeclaration) );
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();

	for ( auto it = m_sceneOrder.begin() ; it!=m_sceneOrder.end();++it)
	{	
		const SCENE_KEY& refScene = it->first;
		MESHPTR_CONTAINER& list = it->second;

		// Set Matrix Instance
		unsigned long nCount=list.size();
		auto it_sub = list.begin();

		cMeshNode* pMeshNode = *it_sub;

		MatrixTexture* pMatrixInstancingTexture = pMeshNode->GetMatrixInstancingTexture();


		if (!pMatrixInstancingTexture->GetValid())
		{
			D3DLOCKED_RECT lock;
			pMatrixInstancingTexture->GetD3DTexture()->LockRect(0, &lock, NULL, D3DLOCK_DISCARD);
			pMeshNode->UpdateMatrixInstancing(list, lock);
			pMatrixInstancingTexture->GetD3DTexture()->UnlockRect(0);
			//pMatrixInstancingTexture->SetValid(true);  //그림자용이므로 무효화 하지않음 다음 메쉬는 계산없이 그림
		}

		pEffect->SetTexture("Tex_MatrixInstanceData",pMatrixInstancingTexture->GetD3DTexture());
		pEffect->SetFloat(Graphics::m_pInstance->m_hfMatrixTextureSize,(float)pMatrixInstancingTexture->GetSize());

		if (refScene.pMaterial->AlphaTestEnable)
			pEffect->SetTechnique(hTShadowAlphaTest);
		else
			pEffect->SetTechnique(hTShadowNotAlphaTest );


		ChangeMaterial(refScene.pMaterial,true);	
		pEffect->CommitChanges();		

		UINT passes;
		pEffect->Begin(&passes, 0);	
		pEffect->BeginPass(0);	
		pMeshNode->RenderInstancing(nCount);
		pEffect->EndPass();
		pEffect->End();		
	}
	Graphics::m_pDevice->SetStreamSourceFreq( 0, 1 );
	Graphics::m_pDevice->SetStreamSourceFreq( 1, 1 );
	HR_V(Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false));		
}

void cRendererQueue::RenderNotAlphaBlendSkinnedInstancing(DWORD elapseTime, std::vector<D3DXHANDLE>& vecTechnique )
{
	HRESULT hr;
	HR_V( Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pSkinnedInstancingVertexDeclaration) );
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();


	for ( auto it = m_sceneOrder.begin() ; it!=m_sceneOrder.end();++it)
	{	
		const SCENE_KEY& refScene = it->first;
		MESHPTR_CONTAINER& list = it->second;

		// Set Matrix Instance
		unsigned long nCount=list.size();
		auto it_sub = list.begin();

		cMeshNode* pMeshNode = *it_sub;
	
		MatrixTexture* pMatrixInstancingTexture = pMeshNode->GetMatrixInstancingTexture();
		

		if (!pMatrixInstancingTexture->GetValid())
		{
			D3DLOCKED_RECT lock;
			pMatrixInstancingTexture->GetD3DTexture()->LockRect(0, &lock, NULL, D3DLOCK_DISCARD);
			pMeshNode->UpdateMatrixInstancing(list, lock);
			pMatrixInstancingTexture->GetD3DTexture()->UnlockRect(0);

			pMatrixInstancingTexture->SetValid(true);	//다음프레임 다시 계산
		}

		pEffect->SetTexture("Tex_MatrixInstanceData",pMatrixInstancingTexture->GetD3DTexture());
		pEffect->SetFloat(Graphics::m_pInstance->m_hfMatrixTextureSize,(float)pMatrixInstancingTexture->GetSize());

		int i = refScene.pMaterial->index_renderer_queue();
		HR_V(pEffect->SetTechnique(vecTechnique[i]));
		ChangeMaterial(refScene.pMaterial,false);
		pEffect->CommitChanges();		

		UINT passes;
		pEffect->Begin(&passes, 0);	
		pEffect->BeginPass(0);	
		pMeshNode->RenderInstancing(nCount);
		pEffect->EndPass();
		pEffect->End();		

		pMatrixInstancingTexture->SetValid(false);
	}
	Graphics::m_pDevice->SetStreamSourceFreq( 0, 1 );
	Graphics::m_pDevice->SetStreamSourceFreq( 1, 1 );
	HR_V(Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false));	
}

void cRendererQueue::RenderShadowSkinnedInstancing(DWORD elapseTime, D3DXHANDLE hTShadowNotAlphaTest,D3DXHANDLE hTShadowAlphaTest )
{
	HRESULT hr;
	HR_V( Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pSkinnedInstancingVertexDeclaration) );
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();

	for ( auto it = m_sceneOrder.begin() ; it!=m_sceneOrder.end();++it)
	{	
		const SCENE_KEY& refScene = it->first;
		MESHPTR_CONTAINER& list = it->second;

		// Set Matrix Instance
		unsigned long nCount=list.size();
		auto it_sub = list.begin();

		cMeshNode* pMeshNode = *it_sub;


		MatrixTexture* pMatrixInstancingTexture = pMeshNode->GetMatrixInstancingTexture();		

		if (!pMatrixInstancingTexture->GetValid())
		{
			D3DLOCKED_RECT lock;
			pMatrixInstancingTexture->GetD3DTexture()->LockRect(0, &lock, NULL, D3DLOCK_DISCARD);
			pMeshNode->UpdateMatrixInstancing(list,lock);					
			pMatrixInstancingTexture->GetD3DTexture()->UnlockRect(0);
			//pMatrixInstancingTexture->SetValid(true);	//그림자용이므로 무효화 하지않음
		}

		pEffect->SetTexture("Tex_MatrixInstanceData",pMatrixInstancingTexture->GetD3DTexture());
		pEffect->SetFloat(Graphics::m_pInstance->m_hfMatrixTextureSize,(float)pMatrixInstancingTexture->GetSize());
		if (refScene.pMaterial->AlphaTestEnable)
			pEffect->SetTechnique(hTShadowAlphaTest);
		else
			pEffect->SetTechnique(hTShadowNotAlphaTest );

		ChangeMaterial(refScene.pMaterial,true);
		pEffect->CommitChanges();		

		UINT passes;
		pEffect->Begin(&passes, 0);	
		pEffect->BeginPass(0);	
		pMeshNode->RenderInstancing(nCount);
		pEffect->EndPass();
		pEffect->End();		
	}
	Graphics::m_pDevice->SetStreamSourceFreq( 0, 1 );
	Graphics::m_pDevice->SetStreamSourceFreq( 1, 1 );
	HR_V(Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false));	
}



SCENE_KEY::SCENE_KEY()
{
	pVertexBuffer=NULL;
	pMaterial=NULL;
	pIndexBuffer=NULL;	
}

SCENE_KEY::SCENE_KEY(cRscVertexBuffer* param0,Material* param1,cRscIndexBuffer* param2)
{
	pVertexBuffer=param0;
	pMaterial=param1;
	pIndexBuffer=param2;	
}

bool SCENE_KEY::operator<( const SCENE_KEY& other ) const
{
	if (pVertexBuffer < other.pVertexBuffer)
		return true;

	else if (pVertexBuffer == other.pVertexBuffer)
	{
		if (pMaterial < other.pMaterial)
			return true;

		else if ( pMaterial == other.pMaterial)
		{
			return pIndexBuffer < other.pIndexBuffer;
		}

	}

	return false;
}



}