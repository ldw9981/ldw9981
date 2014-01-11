#pragma once
#include "Scene/Entity.h"
#include "Foundation/Interface.h"
#include "WinInput/WinInput.h"

class cObjDragon :
	public Entity,
	public cStaticWinInput,
	public IControlable
{
public:
	cObjDragon();
	virtual ~cObjDragon(void);
	BOOL		m_bControl;
private:
	cSceneNode* m_pDummy;		//���� HelperObj
	
public:
	//IObject
	virtual	void	Init();
	virtual	void	Uninit();

	virtual void    Control();
	virtual void	Render();

};