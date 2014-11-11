/*
	

*/
#include "StdAfx.h"
#include "SkinnedMeshNode.h"
#include "MeshNode.h"


#include "Foundation/Trace.h"
#include "Math/Sphere.h"
#include "Math/CollisionDetector.h"
#include "Graphics/RscTexture.h"
#include "Graphics/Graphics.h"

#include "Math/Sphere.h"
#include "Foundation/Define.h"
#include "Graphics/Entity.h"

#include "Graphics/IndexStreamVertexBuffer.h"
#include "Graphics/BoneStreamTexture.h"

namespace Sophia
{


#define SKINNEDMESH_LASTEST 1

SkinnedMeshNode::SkinnedMeshNode(void)
{
	m_bIsActiveAnimation = true;
	m_pMatrixPallete = NULL;
	m_type = TYPE_SKINNEDMESH;
	m_pIndexStreamVertexBuffer=NULL;
	m_pBoneStreamTexture=NULL;
	m_pMatrixPalleteTexture = NULL;
}

SkinnedMeshNode::~SkinnedMeshNode(void)
{
	Release();
}

void SkinnedMeshNode::LinkToBone(Entity* pEntity)
{
	assert(!m_vecBoneRef.empty());
	D3DXMATRIX tmBoneWorldReferenceInv;
		
	size_t iBoneRef=0,nBoneRefSize =m_vecBoneRef.size() ;


	for (iBoneRef=0;iBoneRef<nBoneRefSize;iBoneRef++)
	{
		BONEREFINFO& refItem=m_vecBoneRef[iBoneRef];

		refItem.pNode = pEntity->FindNode(refItem.strNodeName);	
		assert(refItem.pNode!=NULL);	
		// 찾지 못하는경우가 있어서는 안됨 블렌트 버택스에 boneIndex가 들어가있으므로		
		D3DXMatrixInverse(&tmBoneWorldReferenceInv,NULL,&refItem.pNode->GetNodeTM());
		refItem.SkinOffset = GetNodeTM() * tmBoneWorldReferenceInv;	// LocalTM = WorldTM * Parent.WorldTM.Inverse
	}
}

/*

일반 Object, Bone , Skined Mesh 전부 그리고음.
*/
void SkinnedMeshNode::Render()
{	

	m_pRscVetextBuffer->SetStreamSource(0,sizeof(BLEND_VERTEX));
	m_pRscIndexBuffer->SetIndices();				
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();

	UpdateMatrixPallete();
	size_t nBoneRefSize = m_vecBoneRef.size();
	pEffect->SetMatrixArray(Graphics::m_pInstance->m_hmPalette,m_pMatrixPallete,nBoneRefSize);	



	pEffect->CommitChanges();	

	Graphics::m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 
		0,  
		0, 
		m_pRscVetextBuffer->GetVertexCount(),
		m_startIndex,
		m_primitiveCount );			
	
}

void SkinnedMeshNode::BuildComposite(Entity* pEntity)
{	
	cSceneNode::BuildComposite(pEntity);
	LinkToBone(pEntity);		

	if (m_bInstancingEnable)
	{
		CreateInstancingResource();
	}
	else
	{
		CreateMatrixPallete();
	}

	QueueRenderer(pEntity,false);
}

void SkinnedMeshNode::SetBoneRef( std::vector<BONEREFINFO>& vecBoneRef )
{
	m_vecBoneRef = vecBoneRef;
}

void SkinnedMeshNode::QueueRenderer(Entity* pEntity,bool bTraversal)
{
	assert(m_materialRefIndex < m_pRootNode->m_pEntityMaterial->m_ref.size());
	std::vector<Material*>& sub = m_pRootNode->m_pEntityMaterial->m_ref[m_materialRefIndex];
	assert(m_materialSubIndex < sub.size());
	m_pMaterial = sub[m_materialSubIndex];  

	if (m_bShow)
	{	
		int i = m_pMaterial->index_renderer_queue();
		if (m_pMaterial->AlphaBlend == false)
		{
			pEntity->m_renderQueueSkinned.InsertIntoMeshList(this);
		}
		else
		{
			pEntity->m_renderQueueSkinnedAlphaBlend.InsertIntoMeshList(this);
		}		
	}
	
	if (!bTraversal)
		return;

	for ( auto it_child=m_vecChildNode.begin() ;it_child!=m_vecChildNode.end();++it_child )
	{
		(*it_child)->QueueRenderer(pEntity,bTraversal);
	}
}


void SkinnedMeshNode::Release()
{
	cMeshNode::Release();	
	m_vecBoneRef.clear();	
	DeleteMatrixPallete();
	ReleaseInstancingResource();
}

void SkinnedMeshNode::SerializeIn( std::ifstream& stream )
{
	// 이미 앞에서 타입은 읽었다.
	unsigned short ver = 0;
	unsigned char count =0 ;

	//scene
	stream.read((char*)&ver,sizeof(ver));
	ReadString(stream,m_strNodeName);
	ReadString(stream,m_strParentName);
	ReadMatrix(stream,m_nodeTM);	
	stream.read((char*)&m_materialRefIndex,sizeof(m_materialRefIndex));
	stream.read((char*)&m_materialSubIndex,sizeof(m_materialSubIndex));
	stream.read((char*)&m_primitiveCount,sizeof(m_primitiveCount));
	stream.read((char*)&m_startIndex,sizeof(m_startIndex));
	ReadBool(stream,m_bInstancingEnable);
	// mesh
	SerializeInMesh(stream);

	// child	
	stream.read((char*)&count,sizeof(count));
	for ( int i=0 ; i<count ; i++ )
	{
		SCENETYPE type;
		stream.read((char*)&type,sizeof(type));
		cSceneNode* pNode = CreateNode(type);
		pNode->SetRootNode(m_pRootNode);
		pNode->SetParentNode(this);
		pNode->SetParentName(m_strNodeName.c_str());
		AttachChildNode(pNode);
		pNode->SerializeIn(stream);		
	}
}

void SkinnedMeshNode::SerializeOut( std::ofstream& stream )
{
	unsigned short ver = SKINNEDMESH_LASTEST;
	unsigned char count =0 ;

	//scene
	stream.write((char*)&m_type,sizeof(m_type));
	stream.write((char*)&ver,sizeof(ver));
	WriteString(stream,m_strNodeName);
	WriteString(stream,m_strParentName);
	WriteMatrix(stream,m_nodeTM);	
	stream.write((char*)&m_materialRefIndex,sizeof(m_materialRefIndex));
	stream.write((char*)&m_materialSubIndex,sizeof(m_materialSubIndex));	
	stream.write((char*)&m_primitiveCount,sizeof(m_primitiveCount));	
	stream.write((char*)&m_startIndex,sizeof(m_startIndex));	
	WriteBool(stream,m_bInstancingEnable);

	// mesh 
	SerializeOutMesh(stream);

	// child
	count = m_vecChildNode.size();
	stream.write((char*)&count,sizeof(count));
	auto it_child = m_vecChildNode.begin();
	for ( ;it_child!=m_vecChildNode.end();++it_child )
	{
		(*it_child)->SerializeOut(stream);
	}	
}

void SkinnedMeshNode::SerializeOutMesh( std::ofstream& stream )
{
	// index
	DWORD bufferSize =0;
	bufferSize = m_pRscIndexBuffer->GetBufferSize();
	stream.write((char*)&bufferSize,sizeof(bufferSize));

	TRIANGLE* pIndices=(TRIANGLE*)m_pRscIndexBuffer->Lock(0,m_pRscIndexBuffer->GetBufferSize(),0);
	stream.write((char*)pIndices,bufferSize);
	m_pRscIndexBuffer->Unlock();		

	//vertex
	bufferSize = m_pRscVetextBuffer->GetBufferSize();
	stream.write((char*)&bufferSize,sizeof(bufferSize));
	BLEND_VERTEX* pVertices=(BLEND_VERTEX*)m_pRscVetextBuffer->Lock(m_pRscVetextBuffer->GetBufferSize(),0);
	stream.write((char*)pVertices,bufferSize);
	m_pRscVetextBuffer->Unlock();	

	// bone info
	unsigned char count;
	count = m_vecBoneRef.size();
	stream.write((char*)&count,sizeof(count));
	for ( int i=0 ; i<count ; i++ )
	{
		BONEREFINFO& info = m_vecBoneRef[i];
		WriteString(stream,info.strNodeName); 
	}
}

void SkinnedMeshNode::SerializeInMesh( std::ifstream& stream )
{
	// index
	DWORD bufferSize =0;
	stream.read((char*)&bufferSize,sizeof(bufferSize));
	cRscIndexBuffer* pRscIndexBuffer = cResourceMng::m_pInstance->CreateRscIndexBuffer(m_pRootNode->GetNodeName().c_str(),m_strNodeName.c_str(),bufferSize);
	if(pRscIndexBuffer->GetRefCounter() == 0)
	{
		TRIANGLE* pIndices=(TRIANGLE*)pRscIndexBuffer->Lock(0,pRscIndexBuffer->GetBufferSize(),0);
		stream.read((char*)pIndices,bufferSize);
		pRscIndexBuffer->Unlock();		
		pRscIndexBuffer->SetTriangleCount(bufferSize/sizeof(TRIANGLE));
	}
	else
	{
		stream.seekg( bufferSize,std::ios_base::cur);
	}
	SetRscIndexBuffer(pRscIndexBuffer);

	// vertex
	stream.read((char*)&bufferSize,sizeof(bufferSize));
	cRscVertexBuffer* pRscVetextBuffer = cResourceMng::m_pInstance->CreateRscVertexBuffer(m_pRootNode->GetNodeName().c_str(),m_strNodeName.c_str(),bufferSize);
	if(pRscVetextBuffer->GetRefCounter() == 0)
	{
		BLEND_VERTEX* pVertices=(BLEND_VERTEX*)pRscVetextBuffer->Lock(pRscVetextBuffer->GetBufferSize(),0);
		stream.read((char*)pVertices,bufferSize);
		pRscVetextBuffer->Unlock();		
		pRscVetextBuffer->SetVertexCount(bufferSize/sizeof(BLEND_VERTEX));
	}
	else
	{
		stream.seekg( bufferSize,std::ios_base::cur);
	}
	SetRscVertextBuffer(pRscVetextBuffer);

	// bone info
	unsigned char count = 0;
	stream.read((char*)&count,sizeof(count));
	for ( int i=0 ; i<count ; i++ )
	{
		BONEREFINFO info;
		ReadString(stream,info.strNodeName); 
		m_vecBoneRef.push_back(info);
	}
}

void SkinnedMeshNode::Update( DWORD elapseTime )
{
	cMeshNode::Update(elapseTime);
	m_updateBlendMatrix = false;
}

void SkinnedMeshNode::UpdateMatrixPallete()
{
	size_t nBoneRefSize = m_vecBoneRef.size();
	if (!m_updateBlendMatrix)
	{
		size_t iBoneRef=0;
		for (iBoneRef=0;iBoneRef<nBoneRefSize;iBoneRef++)
		{
			BONEREFINFO& refItem=m_vecBoneRef[iBoneRef];
			//m_pMatrixPallete[iBoneRef] = refItem.SkinOffset * refItem.pNode->GetWorldTM();	// WorldTM = LocalTM * Parent.WorldTM
			D3DXMatrixMultiply(&m_pMatrixPallete[iBoneRef],&refItem.SkinOffset,refItem.pNode->GetWorldMatrixPtr());
		}
		
		m_updateBlendMatrix = true;
	}
}

void SkinnedMeshNode::CreateInstancingResource()
{
	if (m_pIndexStreamVertexBuffer==NULL)
	{
		m_pIndexStreamVertexBuffer = cResourceMng::m_pInstance->CreateIndexStreamVertexBuffer(SCENE_KEY(m_pRscVetextBuffer,m_pMaterial,m_pRscIndexBuffer));
		m_pIndexStreamVertexBuffer->AddRef();
	}

	if (m_pBoneStreamTexture==NULL)
	{
		size_t nBoneRefSize = m_vecBoneRef.size();
		DWORD textureSize = (DWORD) pow(2.0f,ceil(log(sqrt((float) 4*nBoneRefSize *INSTANCING_MAX ))/log(2.0f)));
		m_pBoneStreamTexture = cResourceMng::m_pInstance->CreateBoneStreamTexture(SCENE_KEY(m_pRscVetextBuffer,m_pMaterial,m_pRscIndexBuffer),textureSize);
		m_pBoneStreamTexture->AddRef();
	}
	
}

void SkinnedMeshNode::ReleaseInstancingResource()
{
	SAFE_RELEASE(m_pIndexStreamVertexBuffer);
	SAFE_RELEASE(m_pBoneStreamTexture);
}

void SkinnedMeshNode::UpdateMatrixPalleteTexture()
{
/*
	if (!m_updateBlendMatrix)
	{
		D3DXMATRIX* pDst=NULL;
		DWORD offset_bytes = 0;
		DWORD offset_line = 0;
		DWORD bytesMatrix = sizeof(D3DXMATRIX);
		DWORD bytesPerLine= bytesMatrix * (m_pMatrixPalleteTexture->GetWidth()/4); // 1mat= 4pixel

		D3DLOCKED_RECT lock;	
		m_pMatrixPalleteTexture->GetD3DTexture()->LockRect(0,&lock,NULL,D3DLOCK_DISCARD);


		DWORD boneSize = m_vecBoneRef.size();
		for (DWORD boneIndex=0;boneIndex<boneSize;boneIndex++)
		{
			pDst = (D3DXMATRIX*)((LPBYTE)lock.pBits + offset_line*lock.Pitch + offset_bytes);				
			BONEREFINFO& refItem=m_vecBoneRef[boneIndex];
			// = refItem.SkinOffset * refItem.pNode->GetWorldTM();	// WorldTM = LocalTM * Parent.WorldTM
			D3DXMatrixMultiply(pDst,&refItem.SkinOffset,refItem.pNode->GetWorldMatrixPtr());		

			offset_bytes += bytesMatrix;		
			if (offset_bytes >= bytesPerLine)
			{
				offset_line++;			
				offset_bytes=0;
			}	
		}	
		m_pMatrixPalleteTexture->GetD3DTexture()->UnlockRect(0);
		m_updateBlendMatrix = true;
	}*/
}

void SkinnedMeshNode::CreateMatrixPallete()
{
	assert(m_pMatrixPallete==NULL);
	size_t nBoneRefSize = m_vecBoneRef.size();
	m_pMatrixPallete = new D3DXMATRIX[nBoneRefSize];	
}

void SkinnedMeshNode::DeleteMatrixPallete()
{
	SAFE_DELETEARRAY(m_pMatrixPallete);
}

void SkinnedMeshNode::ChangeInstancingEnable( bool val )
{
	m_bInstancingEnable = val;
	if (m_bInstancingEnable)
	{	
		CreateInstancingResource();		
		DeleteMatrixPallete();
	}
	else
	{
		ReleaseInstancingResource();
		CreateMatrixPallete();
	}
}

}