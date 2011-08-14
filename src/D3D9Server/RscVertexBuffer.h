#pragma once
#include "Resource/Resource.h"
#include "Resource/Restore.h"
#include "D3D9Server/StaticD3DDevice9.h"

struct TEXCOORD:public pair<float,float>
{
	TEXCOORD()
	{
		first=0.0f; second=0.0f;
	}
	TEXCOORD(float u,float v)
	{
		first=u; second=v;
	}
};

struct NORMALVERTEX
{	
	D3DXVECTOR3	vertex;		//x,y,z
	D3DXVECTOR3 normal;		//normal
	TEXCOORD	tex;
};


struct BLENDVERTEX
{
	//x,y,z
	D3DXVECTOR3	vertex;
	// weight
	float		weight[3];	
	// index
	BYTE		index[4];
	//normal
	D3DXVECTOR3 normal;
	TEXCOORD	tex;

	void SetWeight(float src_weight[3])
	{
		memcpy(&weight[0],&src_weight[0],sizeof(float)*3);
	}
	void SetIndex(BYTE src_index[4])
	{
		memcpy(&index[0],&src_index[0],sizeof(BYTE)*4);
	}
};

#define FVF_NORMALVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)  
#define FVF_BLENDVERTEX (D3DFVF_XYZB4 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL|D3DFVF_TEX1)


// ������ ���ؽ��� �������������� �ϴ� �����ڴ´�.
class cRscVertexBuffer:
		public cResource,
		public cIRestore,
		private StaticD3DDEVICE9
{
public:
	cRscVertexBuffer(void);
	virtual ~cRscVertexBuffer(void);

private:
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVertexBuffer;
	DWORD					m_BufferSize;
	D3DPOOL					m_Type;
	int						m_nVerties;

public:	
	// cResource
	virtual void			ProcessMakeUniqueKey();  
	virtual	BOOL			Create();	
	virtual	void			Free();

	// cIRestore
	virtual void			Restore();

	LPDIRECT3DVERTEXBUFFER9 GetD3DVertexBuffer() const { return m_pD3DVertexBuffer; }

	DWORD					GetBufferSize() const { return m_BufferSize; }
	void					SetBufferSize(DWORD val) { m_BufferSize = val; }

	int GetVerties() const { return m_nVerties; }
	void SetVerties(int val) { m_nVerties = val; }


	void*					Lock();
	void					Unlock();
	void					SetStreamSource(UINT stride);
	D3DPOOL					GetType() const { return m_Type; }
	void					SetType(D3DPOOL val) { m_Type = val; }
};