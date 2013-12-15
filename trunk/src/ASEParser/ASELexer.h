//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*                        Ver 2.0 / Bulid 2003.12.01                         *
//                          (for Game Academy Lessen)                         *
//*                                                                           *
//*                            ASE Importer Classes                           *
//*                                                                           *
//*                                                                           *
//*                                                                           *
//*                                                                           *
//*  This Program is programmed by Cho SangHyun. sangduck@paran.com           *
//*  Best for Game Developement and Optimized for Game Developement.          *
//*                                                                           *
//*                   (C)Sangduck All right reserved. 2003                    *
//*                           http://www.cgSDK.co.kr                          *
//*                                                                           *
//*****************************************************************************

#pragma once

#include "Lexer.h"

//*****************************************************************************
//*  Toke Definition                                                          *
//*                                                                           *
//*                                                                           *
//*                                                                           *
//*****************************************************************************
enum _ASEToken
{
	TOKENR_3DSMAX_ASCIIEXPORT			=  0,
	TOKENR_COMMENT						=  1,
	TOKENR_SCENE						=  2,
	TOKENR_MATERIAL_LIST				=  3,
	TOKENR_MATERIAL						=  4,
	TOKENR_LIGHTOBJECT					=  5,
	TOKENR_GEOMOBJECT					=  6,
	TOKENR_NODE_NAME					=  7,
	TOKENR_NODE_TM						=  8,
	TOKENR_MESH							=  9,
	TOKENR_MESH_VERTEX					= 10,
	TOKENR_MESH_FACE_LIST				= 11,
	TOKENR_MESH_FACE					= 12,
	TOKENR_MESH_NUMTVERTEX				= 13,
	TOKENR_MESH_NUMCVERTEX				= 14,
	TOKENR_MESH_NORMALS					= 15,
	TOKENR_MESH_FACENORMAL				= 16,
	TOKENR_MESH_VERTEXNORMAL			= 17,
	TOKENR_HELPEROBJECT					= 18,
	TOKENR_NODE_PARENT					= 19,
	TOKENR_HELPER_CLASS					= 20,
	TOKENR_INHERIT_POS					= 21,
	TOKENR_INHERIT_ROT					= 22,
	TOKENR_INHERIT_SCL					= 23,
	TOKENR_TM_ROW0						= 24,
	TOKENR_TM_ROW1						= 25,
	TOKENR_TM_ROW2						= 26,
	TOKENR_TM_ROW3						= 27,
	TOKENR_TM_POS						= 28,
	TOKENR_TM_ROTAXIS					= 29,
	TOKENR_TM_ROTANGLE					= 30,
	TOKENR_TM_SCALE						= 31,
	TOKENR_TM_SCALEAXIS					= 32,
	TOKENR_TM_SCALEAXISANG				= 33,
	TOKENR_BOUNDINGBOX_MIN				= 34,
	TOKENR_BOUNDINGBOX_MAX				= 35,
	TOKENR_TIMEVALUE					= 36,
	TOKENR_MESH_NUMVERTEX				= 37,
	TOKENR_MESH_NUMFACES				= 38,
	TOKENR_GROUP						= 39,
	TOKENR_MESH_TVERT					= 40,
	TOKENR_MESH_TFACELIST				= 41,
	TOKENR_MESH_TVERTLIST				= 42,
	TOKENR_PROP_MOTIONBLUR				= 43,
	TOKENR_PROP_CASTSHADOW				= 44,
	TOKENR_PROP_RECVSHADOW				= 45,
	TOKENR_MATERIAL_REF					= 46,
	TOKENR_SCENE_FILENAME				= 48,
	TOKENR_SCENE_FIRSTFRAME				= 49,
	TOKENR_SCENE_LASTFRAME				= 50,
	TOKENR_SCENE_FRAMESPEED				= 51,
	TOKENR_SCENE_TICKSPERFRAME			= 52,
	TOKENR_SCENE_ENVMAP					= 53,
	TOKENR_MAP_NAME						= 54,
	TOKENR_MAP_CLASS					= 55,
	TOKENR_MAP_SUBNO					= 56,
	TOKENR_MAP_AMOUNT					= 57,
	TOKENR_SCENE_AMBIENT_STATIC			= 58,
	TOKENR_MATERIAL_COUNT				= 59,
	TOKENR_MATERIAL_NAME				= 60,
	TOKENR_MATERIAL_CLASS				= 61,
	TOKENR_MATERIAL_AMBIENT				= 62,
	TOKENR_MATERIAL_DIFFUSE				= 63,
	TOKENR_MATERIAL_SPECULAR			= 64,
	TOKENR_MATERIAL_SHINE				= 65,
	TOKENR_MATERIAL_SHINESTRENGTH		= 66,
	TOKENR_MATERIAL_TRANSPARENCY		= 67,
	TOKENR_MATERIAL_WIRESIZE			= 68,
	TOKENR_MATERIAL_SHADING				= 69,
	TOKENR_MATERIAL_XP_FALLOFF			= 70,
	TOKENR_MATERIAL_SELFILLUM			= 71,
	TOKENR_MATERIAL_TWOSIDED			= 72,
	TOKENR_MATERIAL_FALLOFF				= 73,
	TOKENR_MATERIAL_XP_TYPE				= 74,
	TOKENR_MAP_DIFFUSE					= 75,
	TOKENR_MESH_VERTEX_LIST				= 76,
	TOKENR_MESH_NUMTVFACES				= 77,
	TOKENR_MESH_CVERTLIST				= 78,
	TOKENR_LIGHT_TYPE					= 79,
	TOKENR_LIGHT_SHADOWS				= 80,
	TOKENR_LIGHT_USELIGHT				= 81,
	TOKENR_LIGHT_SPOTSHAPE				= 82,
	TOKENR_LIGHT_USEGLOBAL				= 83,
	TOKENR_LIGHT_ABSMAPBIAS				= 84,
	TOKENR_LIGHT_OVERSHOOT				= 85,
	TOKENR_LIGHT_SETTINGS				= 86,
	TOKENR_LIGHT_COLOR					= 87,
	TOKENR_LIGHT_INTENS					= 88,
	TOKENR_LIGHT_ASPECT					= 89,
	TOKENR_LIGHT_TDIST					= 90,
	TOKENR_LIGHT_MAPBIAS				= 91,
	TOKENR_LIGHT_MAPRANGE				= 92,
	TOKENR_LIGHT_MAPSIZE				= 93,
	TOKENR_LIGHT_RAYBIAS				= 94,
	TOKENR_MESH_SMOOTHING				= 95,
	TOKENR_MESH_MTLID					= 96,
	TOKENR_WIREFRAME_COLOR				= 97,
	TOKENR_TM_ANIMATION					= 98,
	TOKENR_CONTROL_POS_TRACK			= 99,
	TOKENR_CONTROL_ROT_TRACK			=100,
	TOKENR_CONTROL_POS_SAMPLE			=101,
	TOKENR_CONTROL_ROT_SAMPLE			=102,
	TOKENR_CAMERAOBJECT					=103,
	TOKENR_CAMERA_TYPE					=104,
	TOKENR_CAMERA_SETTINGS				=105,
	TOKENR_CAMERA_NEAR					=106,
	TOKENR_CAMERA_FAR					=107,
	TOKENR_CAMERA_FOV					=108,
	TOKENR_CAMERA_TDIST					=109,
	TOKENR_NUMSUBMTLS					=110,
	TOKENR_SUBMATERIAL					=111,
	TOKENR_MAP_SPECULAR					=112,
	TOKENR_MAP_SHINE					=113,
	TOKENR_MAP_GENERIC					=114,
	TOKENR_BITMAP						=115,
	TOKENR_MAP_TYPE						=116,
	TOKENR_UVW_U_OFFSET					=117,
	TOKENR_UVW_V_OFFSET					=118,
	TOKENR_UVW_U_TILING					=119,
	TOKENR_UVW_V_TILING					=120,
	TOKENR_UVW_ANGLE					=121,
	TOKENR_UVW_BLUR						=122,
	TOKENR_UVW_BLUR_OFFSET				=123,
	TOKENR_UVW_NOUSE_AMT				=124,
	TOKENR_UVW_NOISE_SIZE				=125,
	TOKENR_UVW_NOISE_LEVEL				=126,
	TOKENR_UVW_NOISE_PHASE				=127,
	TOKENR_BITMAP_FILTER				=128,
	TOKENR_MESH_MAPPINGCHANNEL			=129,
	TOKENR_MESH_TFACE					=130,
	TOKENR_CONTROL_POS_BEZIER			=131,
	TOKENR_CONTROL_BEZIER_POS_KEY 		=132,
	TOKENR_CONTROL_ROT_TCB				=133,
	TOKENR_CONTROL_TCB_ROT_KEY 			=134,
	TOKENR_MAP_OPACITY					=135,
	TOKENR_MATERIAL_FACEMAP				=136,
	TOKENR_MESH_NUMSKINWEIGHT			=137,
	TOKENR_MESH_NUMBONE					=138,
	TOKENR_BONE_LIST					=139,
	TOKENR_BONE_NAME					=140,
	TOKENR_MESH_WVERTEXS				=141,
	TOKENR_MESH_WEIGHT					=142,
	TOKENR_BONE_BLENGING_WEIGHT			=143,
	TOKENR_MAP_REFLECT					=144,
	TOKENR_MAP_REFRACT					=145,
	TOKENR_MAP_BUMP						=146,
	TOKENR_SCENE_BACKGROUND_STATIC		=147,
	TOKENR_MAP_SELFILLUM				=148,
	TOKENR_LIGHT_EXCLUDELIST			=149,
	TOKENR_LIGHT_NUMEXCLUDED			=150,
	TOKENR_LIGHT_EXCLUDED_INCLUDE		=151,
	TOKENR_LIGHT_EXCLUDED_AFFECT_ILLUM	=152,
	TOKENR_LIGHT_EXCLUDED_AFFECT_SHADOW	=153,
	TOKENR_LIGHT_EXCLUDED				=154,
	TOKENR_LIGHT_HOTSPOT				=155,
	TOKENR_LIGHT_FALLOFF				=156,
	TOKENR_MESH_FACEMAPLIST				=157,
	TOKENR_MESH_FACEMAP					=158,
	TOKENR_MESH_FACEMAPVERT				=159,
	TOKENR_BITMAP_INVERT				=160,
	TOKENR_SHAPEOBJECT					=161,
	TOKENR_SHAPE_LINECOUNT				=162,
	TOKENR_SHAPE_LINE					=163,
	TOKENR_SHAPE_VERTEXCOUNT			=164,
	TOKENR_SHAPE_VERTEX_KNOT			=165,
	TOKENR_SHAPE_VERTEX_INTERP			=166,
	TOKENR_SHAPE_CLOSED					=167,
	TOKENR_IK_JOINT						=168,
	TOKENR_IK_TYPE						=169,
	TOKENR_IK_DOF						=170,
	TOKENR_IK_XACTIVE					=171,
	TOKENR_IK_YACTIVE					=172,
	TOKENR_IK_ZACTIVE					=173,
	TOKENR_IK_XLIMITED					=174,
	TOKENR_IK_YLIMITED					=175,
	TOKENR_IK_ZLIMITED					=176,
	TOKENR_IK_XEASE						=177,
	TOKENR_IK_YEASE						=178,
	TOKENR_IK_ZEASE						=179,
	TOKENR_IK_LIMITEXACT				=180,
	TOKENR_IK_JOINTINFO					=181,
	TOKENR_LIGHT_ATTNSTART				=182,
	TOKENR_LIGHT_ATTNEND				=183,
	TOKENR_MAP_AMBIENT					=184,
	TOKENR_MESH_VERTCOL					=185,
	TOKENR_MESH_NUMCVFACES				=186,
	TOKENR_MESH_CFACELIST				=187,
	TOKENR_MESH_CFACE					=188,
	TOKENR_MAP_SHINESTRENGTH			=189,
	TOKENR_MAP_FILTERCOLOR				=190,

	TOKENR_NODE_VISIBILITY_TRACK		=191,
	TOKENR_CONTROL_FLOAT_SAMPLE			=192,
	TOKENR_CONTROL_FLOAT_KEY			=193,

	TOKENR_BONE_PROPERTY				=194,
	TOKENR_BONE							=195,
	TOKENR_SKIN_INITTM					=196,
	TOKENR_CONTROL_SCALE_TRACK			=197,
	TOKENR_CONTROL_SCALE_SAMPLE			=198,
	TOKENR_SCENE_AMBIENT_ANIM			=199,
	TOKENR_CONTROL_POINT3_KEY			=200,
	TOKENR_CONTROL_TCB_POINT3_KEY		=201,
	TOKENR_CONTROL_TCB_FLOAT_KEY		=202,
	TOKENR_CONTROL_TCB_POS_KEY			=203,
	TOKENR_CONTROL_TCB_SCALE_KEY		=204,
	TOKENR_CONTROL_BEZIER_FLOAT_KEY		=205,
	TOKENR_CONTROL_BEZIER_POINT3_KEY	=206,
	TOKENR_CONTROL_BEZIER_SCALE_KEY		=207,
	TOKENR_CONTROL_POS_LINEAR			=208,
	TOKENR_CONTROL_POS_TCB				=209,
	TOKENR_CONTROL_ROT_LINEAR			=210,
	TOKENR_CONTROL_ROT_BEZIER			=211,
	TOKENR_CONTROL_SCALE_LINEAR			=212,
	TOKENR_CONTROL_SCALE_TCB			=213,
	TOKENR_CONTROL_SCALE_BEZIER			=214,

	TOKENR_CONTROL_POS_KEY				=215,
	TOKENR_CONTROL_ROT_KEY 				=216,
	TOKENR_CONTROL_SCALE_KEY			=217,

	TOKENR_CONTROL_POINT3_TCB			=218,
	TOKENR_CONTROL_POINT3_BEZIER		=219,
	TOKENR_CONTROL_COLOR_BEZIER			=220,
	TOKENR_CONTROL_POINT3_SAMPLE		=221,

	TOKENR_CONTROL_FLOAT_TCB			=222,
	TOKENR_CONTROL_FLOAT_BEZIER			=223,
	TOKENR_CONTROL_FLOAT_LINEAR			=224,

	TOKENR_CAMERA_ANIMATION				=225,
	TOKENR_LIGHT_ANIMATION				=226,

	TOKENR_MATERIAL_WIREUNITS			=227,

	TOKENR_SCENE_MESHFRAMESTEP			=228,
	TOKENR_SCENE_KEYFRAMESTEP			=229,

	TOKENR_PROP_HIDDEN					=230,
	TOKENR_PROP_NORENDERABLE			=231,
	TOKENR_PROP_BONE					=232,		
	TOKENR_MAX							=233
};											



//*****************************************************************************
//  ASE Lexer
//  
//  1) ASE Lexer란...
//     - 정해진 File을 차례대로 읽어서 ASE에 정의된 Token단위로 잘라주는 역할을
//       하는 Class이다.
//     - GetToken(m_TokenString)함수를 호출하면 Text File의 첫 Token부터 차례대로 나온다.
//     - GetToken(LPSTR p_TokenString)의 Return Value는 Token의 종류를 돌려주고
//       넘져진 p_TokenString에는 그엥 해당하는 String을 돌려준다.
//  
//*****************************************************************************
class cASELexer : public cLexer
{
public:
			cASELexer(const char* p_FileName=NULL, DWORD p_SizeOfBuffer=MAX_SIZE_OF_BUFFER);
	virtual ~cASELexer();

	virtual	LONG		GetToken(LPSTR p_TokenString);

	virtual void		AddTokenHistory( int identifier );
	void				TraceHistory();
};
