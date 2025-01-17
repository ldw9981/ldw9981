#pragma once
#include "meshnode.h"
namespace Sophia
{

class MultiSub;
class Material;
class cRscTexture;


struct BONEREFINFO
{
	// BoneSceneNode 기준 좌표계에서의 SkinnedMesh의 Transform 즉 Relative Transform(Local)이다.
	// SkinnedMesh가 그릴때 사용할 최종 WorldTransform = Relative Transform(Local) * 해당 BoneNode의 WorldTransform
	D3DXMATRIX	SkinnedMeshOffset;		
	cSceneNode* pBoneSceneNode;			
	std::string strNodeName;

	BONEREFINFO()
	{
		pBoneSceneNode=NULL;
		D3DXMatrixIdentity(&SkinnedMeshOffset);
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

	bool						m_updateMatrixPallete;
	cRscTexture*				m_pMatrixPalleteTexture;
public:
	virtual void			Update( DWORD elapseTime );
	virtual void			Render(DWORD elapseTime);
	virtual void			BuildComposite(Entity* pEntity);

	void					SetBoneRef(std::vector<BONEREFINFO>& vecBoneRef);
	std::vector<BONEREFINFO>&	GetArrayBoneRef()  { return m_vecBoneRef; }
	void					LinkToBone(Entity* pEntity);


	virtual void			GatherRender(Entity* pEntity,bool bTraversal);
	virtual	void			Release();

	// ISerialize
	virtual void		SerializeIn(std::ifstream& stream);
	virtual void		SerializeOut(std::ofstream& stream);

	virtual void SerializeOutMesh(std::ofstream& stream);
	virtual void SerializeInMesh(std::ifstream& stream);

	void UpdateMatrixPallete();
	
	virtual void RenderInstancing( int instanceSize );
	virtual void UpdateMatrixInstancing( std::vector<cMeshNode*>& list, const D3DLOCKED_RECT& lock);


	void ChangeInstancingEnable(bool val);
protected:
	virtual void CreateInstancingResource();
	void CreateMatrixPallete();
	void DeleteMatrixPallete();
};
}