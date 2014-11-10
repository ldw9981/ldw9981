#include "StdAfx.h"
#include "Vertex.h"

namespace Sophia
{

D3DVERTEXELEMENT9 declNormal[] = {  
	{0,		0, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,    0},  
	{0, 3 * 4, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,      0},   		
	{0, 6 * 4, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,     0},  
	{0, 9 * 4, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL,    0},  	
	{0,12 * 4, D3DDECLTYPE_FLOAT2,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,    0},  
	{0,14 * 4, D3DDECLTYPE_FLOAT2,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,    1},  
	D3DDECL_END()  
};  

D3DVERTEXELEMENT9 declBlend[] = {
	{0,		0, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,    0},  
	{0, 3 * 4, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,      0},   		
	{0, 6 * 4, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,     0},  
	{0, 9 * 4, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL,    0},  	
	{0,12 * 4, D3DDECLTYPE_FLOAT2,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,    0},  
	{0,14 * 4, D3DDECLTYPE_FLOAT2,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,    1},  
	{0,16 * 4, D3DDECLTYPE_FLOAT3,		  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT ,0},
	{0,19 * 4, D3DDECLTYPE_UBYTE4,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES,0},
	D3DDECL_END()  
};  

D3DVERTEXELEMENT9 declNormalInstance[] =
{
	{0,		0, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,    0},  
	{0, 3 * 4, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,      0},   		
	{0, 6 * 4, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,     0},  
	{0, 9 * 4, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL,    0},  	
	{0,12 * 4, D3DDECLTYPE_FLOAT2,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,    0},  
	{0,14 * 4, D3DDECLTYPE_FLOAT2,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,    1},  
	{1, 0, D3DDECLTYPE_FLOAT3,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
	{1, 12, D3DDECLTYPE_FLOAT3,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3},
	{1,24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4},
	{1,36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5},
	D3DDECL_END()
};

D3DVERTEXELEMENT9 declBlendInstance[] =
{
	{0,		0, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,    0},  
	{0, 3 * 4, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,      0},   		
	{0, 6 * 4, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,     0},  
	{0, 9 * 4, D3DDECLTYPE_FLOAT3,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL,    0},  	
	{0,12 * 4, D3DDECLTYPE_FLOAT2,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,    0},  
	{0,14 * 4, D3DDECLTYPE_FLOAT2,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,    1},  
	{0,16 * 4, D3DDECLTYPE_FLOAT3,		  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT ,0},
	{0,19 * 4, D3DDECLTYPE_UBYTE4,        D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES,0},
	{1, 0, D3DDECLTYPE_FLOAT2,			D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
	D3DDECL_END()
};

}