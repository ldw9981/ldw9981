#pragma once
#include "scenenode.h"
#include "Math/Sphere.h"
#include "Graphics/StaticD3DDevice9.h"
#include "Scene/RendererQueue.h"

class Entity :
	public cSceneNode
	,protected StaticD3DDEVICE9
{
public:
	Entity(void);
	virtual ~Entity(void);

public:
	cRendererQueue			m_renderQueueNormal[16];
	cRendererQueue			m_renderQueueBlend[16];
	cRendererQueue			m_renderQueueTerrain;
	cRendererQueue			m_renderQueueGUI;

	cRendererQueue			m_renderQueueNormalShadow;
	cRendererQueue			m_renderQueueBlendShadow;
	std::list<Entity*>::iterator m_itEntityList;

protected:
	cSphere				m_BoundingSphere;		// 기본 구 (한번만설정하며 매프레임 위치만 갱신)
	
public:
	void				SetBoundingSphere(cSphere& Sphere);
	cSphere&			GetBoundingSphere()  { return m_BoundingSphere; }

	virtual bool		CullRendererIntoRendererQueue(Frustum* pFrustum );
	void				RenderBound();
	virtual void		Update(DWORD elapseTime);
	virtual	void		BuildComposite();
};

