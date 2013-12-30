#pragma once
#include "meshnode.h"

struct BONEREFINFO
{
	std::string strNodeName;
	cMeshNode* pRefBoneMesh;		//본은 무조건 메쉬이다	
	D3DXMATRIX	BoneOffSetTM_INV;

	BONEREFINFO()
	{
		pRefBoneMesh=NULL;
		D3DXMatrixIdentity(&BoneOffSetTM_INV);
	}
};

struct BONEWEIGHT
{
	BYTE	bone_index;
	float	bone_weight;		
	static BOOL GreatBoneWeight(BONEWEIGHT& a,BONEWEIGHT& b)
	{
		if ( a.bone_weight > b.bone_weight)
			return TRUE;

		return FALSE;
	}
};

class SkinnedMeshNode :
	public cMeshNode
{
public:
	SkinnedMeshNode(void);
	virtual ~SkinnedMeshNode(void);

protected:
	std::vector<BONEREFINFO>		m_vecBoneRef;				//메쉬가 참조하는 본 정보
	D3DXMATRIX*				m_pArrayMatBoneRef;

public:
	virtual void			Render();	
	virtual void			RenderShadow();
	virtual void			BuildComposite();

	void					SetBoneRef(std::vector<BONEREFINFO>& vecBoneRef);
	std::vector<BONEREFINFO>&	GetArrayBoneRef()  { return m_vecBoneRef; }
	void					LinkToBone();


	virtual void			QueueRenderer(cView* pView,bool bTraversal);
};
