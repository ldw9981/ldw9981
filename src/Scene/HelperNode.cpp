#include "StdAfx.h"
#include "../ASEParser/ASEParser.h"
#include "../ASEParser/ASELexer.h"
#include "HelperNode.h"


cHelperNode::cHelperNode(void)
{
//	SetNodeType(HELPER);	
}

cHelperNode::~cHelperNode(void)
{
	
}

void cHelperNode::Update(DWORD elapseTime)
{
	UpdateWorldTM(UpdateTransformAnm(elapseTime),GetParentNode());
	UpdateChildren(elapseTime);
}

void cHelperNode::Render()
{

}
