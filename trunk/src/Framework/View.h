#pragma once
#include "Foundation/Interface.h"
#include "Scene/CameraNode.h"
#include "Framework/ViewMng.h"
#include "Graphics/Entity.h"


class cCameraNode;
class cView:
	public IUnknownObject,
	public IUpdatable,
	public IRenderable,
	public IRenderer,
	public IControlable,
	public INotifiable,
	public FiniteState
{
public:
	cView(void);
	virtual ~cView(void);
	friend class cViewMng;
protected:
	std::list<IRenderable*>		m_RenderableList;
	std::list<IUpdatable*>		m_ProgressableList;
	std::list<IControlable*>	m_ControlableList;

	cView*					m_pParentView;				// 부모 씬
	

	BOOL					m_bHide;					//입력을 받지않고 그리지않는다.	갱신은 한다.	
	D3DVIEWPORT9			m_ViewPortInfo;	
	std::string				m_strName;
public:
	cViewMng				m_ViewState;
	std::list<Entity*>		m_listEntity;
	std::list<Entity*>		m_listEntityShadow;
	std::list<Entity*>		m_listEntityRender;

	cRendererQueue			m_renderQueueNormal[16];
	cRendererQueue			m_renderQueueBlend[16];
	cRendererQueue			m_renderQueueTerrain;
	cRendererQueue			m_renderQueueGUI;

	cRendererQueue			m_renderQueueNormalShadow;
	cRendererQueue			m_renderQueueBlendShadow;

public:
	cViewMng& GetState() { return m_ViewState; }
	cView* GetParentView() const { return m_pParentView; }
	void SetParentView(cView* val) { m_pParentView = val; }
	D3DVIEWPORT9&			GetViewPortInfo()  { return m_ViewPortInfo; }
	void					SetViewPortInfo(UINT x,UINT y,UINT width,UINT height);
	void					SetViewPortInfo(D3DVIEWPORT9& val) { m_ViewPortInfo = val; }
	void					SetViewPort();

	BOOL					GetHide() const { return m_bHide; }
	void					SetHide(BOOL val) { m_bHide = val; }
	void					AttachObject(IUnknownObject* pIUnknownObject);
	void					DettachObject(IUnknownObject* pIUnknownObject);
	virtual void			Enter();
	virtual void			Leave();

	// cIRenderable
	virtual void			ProcessRender();
	void					ProcessRenderEx();

	// cIUpdatable
	virtual void			Update(DWORD elapseTime);

	virtual void			Control();

	virtual void			Render();

	virtual void			Notify(cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam);
	void					DebugRender();

	void					CullFrustum();
};
