#pragma once
#include "Resource/Resource.h"
#include "Resource/Restore.h"

namespace Sophia
{



// ������ ���ؽ��� �������������� �ϴ� �����ڴ´�.
class cRscVertexBuffer:
		public cResource,
		public cIRestore
{
public:
	cRscVertexBuffer(void);
	virtual ~cRscVertexBuffer(void);

protected:
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVertexBuffer;
	DWORD					m_BufferSize;
	D3DPOOL					m_pool;
	int						m_vertexCount;
	DWORD					m_usage;
	UINT					m_OffSet;
public:	
	// cResource
	virtual	BOOL			Create();	
	virtual	void			Free();

	// cIRestore
	virtual void			Restore();

	LPDIRECT3DVERTEXBUFFER9 GetD3DVertexBuffer() const { return m_pD3DVertexBuffer; }

	DWORD					GetBufferSize() const { return m_BufferSize; }
	void					SetBufferSize(DWORD val) { m_BufferSize = val; }

	int GetVertexCount() const { return m_vertexCount; }
	void SetVertexCount(int val) { m_vertexCount = val; }


	void*					Lock(UINT SizeToLock,DWORD Flags);
	void					Unlock();
	void					SetStreamSource(UINT streamNumber,UINT stride);
	void					SetStreamSourceFreq(UINT streamNumber,UINT setting);

	D3DPOOL					GetPool() const { return m_pool; }
	void					SetPool(D3DPOOL val) { m_pool = val; }
	DWORD GetUsage() const { return m_usage; }
	void SetUsage(DWORD val) { m_usage = val; }
};

}