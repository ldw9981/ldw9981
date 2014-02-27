#pragma once
#include "Foundation/interface.h"
#include "Scene/Transformable.h"

namespace Sophia
{

struct SCENEINFO; 
class cASEParser; 
class cRscTransformAnm;
class cRendererQueue;
class cCameraNode;
class cView;
class Frustum;
class Entity;
class SceneAnimation;
class EntityAnimation;
class EntityMaterial;
class SceneMaterial;

struct SCENENODEINFO;
typedef unsigned char SCENETYPE;
class cSceneNode:
	public IUnknownObject,
	public IUpdatable,	
	public cTransformable,
	public IRenderer,
	public ISerializable
{
public:
	cSceneNode(void);
	virtual ~cSceneNode(void);
	
	enum TYPE { TYPE_ROOT,TYPE_SCENE,TYPE_MESH,TYPE_SKINNEDMESH};
public:
	std::list<cSceneNode*>		m_listChildNode;		
protected:
	SCENETYPE 					m_type;
	std::string					m_strNodeName;			
	std::string					m_strParentName;		
	
	cSceneNode*				m_pParentNode;
	Entity*					m_pRootNode;
	bool					m_bIsBone;

	
	// Transform 애니메이션 정보
	std::vector<SceneAnimation*>	m_vecSceneAnimation;
	

	BOOL					m_bIsActiveAnimation;
	D3DXMATRIX				m_AnimationTM;			
	bool					m_bRender;
	//DWORD					m_animationTime;
	std::list<cSceneNode*>::iterator m_ParentListIt;
public:	

	D3DXMATRIX				m_nodeTM;
	Sophia::SCENETYPE	GetType() const { return m_type; }
	void				SetNodeTM(D3DXMATRIX& val) { m_nodeTM = val; }
	D3DXMATRIX&			GetNodeTM();
		
	D3DXMATRIX*			UpdateSceneAnimation();
	D3DXMATRIX&			GetAnimationTM();
			

	//  Object list
	// 자식 오브젝트를 추가한다.
	void				AttachChildNode(cSceneNode* pItem);
	void				DettachChildNode(cSceneNode* pItem);

	// 자신을 포함한 자식ASE오브젝트에서 ASE오브젝트를 찾는다.
	cSceneNode*			FindNode(std::string& nodename);

	//Get/Set
	void				SetNodeName( const char* nodename );
	std::string&				GetNodeName();

	void				SetParentName( const char* nodename );
	std::string&				GetParentName();
	BOOL				IsExistParentName();		
	BOOL				IsRootNode();
	
	cSceneNode*			GetParentNode() const { return m_pParentNode; }
	void				SetParentNode(cSceneNode* val) { m_pParentNode = val; }


	Entity*				GetRootNode() const { return m_pRootNode; }
	void				SetRootNode(Entity* val) { m_pRootNode = val; }
	bool				GetIsBone() const { return m_bIsBone; }
	void				SetIsBone(bool val) { m_bIsBone = val; }	


	BOOL				GetIsActiveAnimation() const { return m_bIsActiveAnimation; }
	void				SetIsActiveAnimation(BOOL val) { m_bIsActiveAnimation = val; }

	bool GetRender() const { return m_bRender; }
	void SetRender(bool val) { m_bRender = val; }
	void				UpdateChildren(DWORD elapseTime);
	void				RenderChildren();
	void				FreeChildren();
	
	cSceneNode&	operator=(const cSceneNode& other);


	virtual void		Render();
	virtual void		RenderShadow();
	virtual void		Update(DWORD elapseTime);

	/*
		빌드란 씬노드구성후 작동하기전에 노드의 후처리 작업으로 완성시킴을 의미한다.
	*/
	virtual	void		BuildComposite(Entity* pEntity);

	// ISerialize
	virtual void		SerializeIn(std::ifstream& stream);
	virtual void		SerializeOut(std::ofstream& stream);

	virtual void		QueueRenderer(Entity* pEntity,bool bTraversal);
	virtual void		QueueRendererShadow(Entity* pEntity,bool bTraversal);
	virtual void		Release();	

	cSceneNode*		CreateNode(SCENETYPE type);

	virtual void PushAnimation(EntityAnimation* pEntityAnimation);
	virtual void PopAnimation();
	virtual void PushMaterial(EntityMaterial* pEntityMaterial);
	virtual void PopMaterial();
	void Test(void(*Func)(cSceneNode*));
};

}