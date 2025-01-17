﻿#pragma once
/*
	쉐이더 하나와 대응되는 렌더러를 그리는 리스트

*/
#include "Foundation/Interface.h"

namespace Sophia
{

class IRenderer;
class Material;
class MultiSub;
class cCameraNode;
class cMeshNode;
class cRscVertexBuffer;
class cRscIndexBuffer;

struct SCENE_KEY
{
	cRscVertexBuffer*	pVertexBuffer;
	Material*			pMaterial;
	cRscIndexBuffer*	pIndexBuffer;	

	SCENE_KEY();
	SCENE_KEY(cRscVertexBuffer* param0,Material* param1,cRscIndexBuffer* param2);
	bool operator<(const SCENE_KEY& other) const;
};

class cRendererQueue
{
public:
	cRendererQueue();
	~cRendererQueue();

	typedef std::pair<cMeshNode*,Material*> MESH_MATERIAL_PAIR;
	typedef std::pair<cMeshNode*,float>		MESH_DISTANCE_PAIR;
	typedef std::vector<cMeshNode*>			MESHPTR_CONTAINER;


	std::vector<MESH_DISTANCE_PAIR>					m_distanceOrder;
	std::map<Material*,MESHPTR_CONTAINER>		m_materialOrder;	// Material같고 index,vertex다를수있다.
	std::map<SCENE_KEY,MESHPTR_CONTAINER>		m_sceneOrder;		// Material,index,vertex모두같다.
private:
public:
	void	GatherRender(std::vector<cMeshNode*>& vecMesh); 
	void	GatherRenderAlphaBlend(std::vector<cMeshNode*>& vecMesh,D3DXVECTOR3* pCameraWorldPosition );	

	void	Clear();	

	void	RenderShadowByMaterialOrder(DWORD elapseTime, D3DXHANDLE hTShadowNotAlphaTest,D3DXHANDLE hTShadowAlphaTest);
	void	RenderShadowNormalInstancing(DWORD elapseTime,D3DXHANDLE hTShadowNotAlphaTest,D3DXHANDLE hTShadowAlphaTest);
	void	RenderShadowSkinnedInstancing(DWORD elapseTime,D3DXHANDLE hTShadowNotAlphaTest,D3DXHANDLE hTShadowAlphaTest);
	
	void	RenderNotAlphaBlendByMaterialOrder(DWORD elapseTime,std::vector<D3DXHANDLE>& vecTechnique);
	void	RenderNotAlphaBlendNormalInstancing(DWORD elapseTime,std::vector<D3DXHANDLE>& vecTechnique);
	void	RenderNotAlphaBlendSkinnedInstancing(DWORD elapseTime,std::vector<D3DXHANDLE>& vecTechnique);
	void	RenderAlphaBlendByDistanceOrder(DWORD elapseTime,std::vector<D3DXHANDLE>& vecTechnique);

protected:
	void	ChangeMaterial(Material* pMaterial,bool textureOpacityOnly);
	void	SubRenderAlphaBlend(DWORD elapseTime,std::vector<D3DXHANDLE>& vecTechnique,std::vector<MESH_DISTANCE_PAIR>& containerTemp );
};
}