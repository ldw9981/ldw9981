#pragma once
#include "../Common/interface.h"
#include "../Geometrics/Sphere.h"
#include "../Resource/RscVertexBuffer.h"
#include "../Resource/RscIndexBuffer.h"
#include "SceneGraphNode.h"
#include "MaterialNode.h"

class cMeshNode;
struct BONEREFINFO
{
	wstring strNodeName;
	cMeshNode* pBoneRef;		//본은 무조건 메쉬이다	
	D3DXMATRIX	BoneOffSetTM_INV;
};

struct BONEWEIGHT
{
	BYTE	bone_index;
	float	bone_weight;		
	static BOOL GreatBoneWeight(BONEWEIGHT* a,BONEWEIGHT* b)
	{
		if ( a->bone_weight > b->bone_weight)
			return TRUE;

		return FALSE;
	}
};

class cMeshNode:
	public cSceneGraphNode,
	public cIRenderer,
	private cStaticD3DDEVICE9
{
public:
	cMeshNode(void);
	virtual ~cMeshNode(void);

private:	
			

	vector<BONEREFINFO>		m_arrayBoneRef;				//메쉬가 참조하는 본 정보	
	map<SUBMATINDEX,WORD>	m_mapSubIndexCount;
	UINT					m_TotalBoneRef;

	D3DXMATRIX				m_BoneOffsetTM;
	D3DXMATRIX				m_BoneOffsetTMInv;

	BOOL					m_bIsBone;

	D3DXVECTOR3				m_vecVelocity;		// 속도	
	D3DXVECTOR3				m_WorldPosOld;		// 이전 위치

	UINT					m_TotalVertex;

	cMaterialEx				m_Matrial;
	cRscIndexBuffer*		m_pRscIndexBuffer;	
	cRscVertexBuffer*		m_pRscVetextBuffer;	
	
public:

	UINT 					GetTotalBoneRef() const { return m_TotalBoneRef; }
	void 					SetTotalBoneRef(UINT val) { m_TotalBoneRef = val; }

	UINT 					GetTotalVertex() const { return m_TotalVertex; }
	void 					SetTotalVertex(UINT val) { m_TotalVertex = val; }

	// cSceneGraphNode
	vector<BONEREFINFO>&	GetArrayBoneRef()  { return m_arrayBoneRef; }
	map<SUBMATINDEX,WORD>&	GetMapSubIndexCount()  { return m_mapSubIndexCount; }
	cRscIndexBuffer*		GetRscIndexBuffer() const { return m_pRscIndexBuffer; }
	void					SetRscIndexBuffer(cRscIndexBuffer* val) { m_pRscIndexBuffer = val; }
	
	cRscVertexBuffer*		GetRscVetextBuffer() const { return m_pRscVetextBuffer; }
	void					SetRscVetextBuffer(cRscVertexBuffer* val) { m_pRscVetextBuffer = val; }

	// 참조하는 본의 노드네임을 추가한다.
	void					InsertBoneRefName(LPCWSTR strNodeName);
	
	// 루트에서 부터 본을 찾아 찾아 연결한다.(본의 연결은 모든 본을 게층구조에 넣은후에 연결한다.) 
	void					LinkToBone();


	BOOL					GetIsBone() const { return m_bIsBone; }
	void					SetIsBone(BOOL val) { m_bIsBone = val; }		

	BOOL					IsSkinnedMesh();


	const D3DXVECTOR3&		GetVelocity()  { return m_vecVelocity; }

	void					SetMatrial(cMaterialEx& val) { m_Matrial = val; }	




	virtual void			Init();
	virtual void			Uninit();

	virtual void			Update(DWORD elapseTime);
	virtual void			Render();
	
};



