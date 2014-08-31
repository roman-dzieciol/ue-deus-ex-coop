// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	uppObject.h
// ============================================================================
#ifndef UPPOBJECT_H
#define UPPOBJECT_H

#include "uppTables.h"
#include "uppProperty.h"
#include "uppObjData.h"
#include "uppStructObjs.h"

#pragma warning( push )
#pragma warning( disable : 4355 )

// ----------------------------------------------------------------------------
//  upsStateFrame
// ----------------------------------------------------------------------------
struct upsStateFrame : public upsoContainer
{
	upsObjIndex Node;
	upsObjIndex StateNode;
	upsQword ProbeMask;
	upsInt LatentAction;
	upsIndex Offset;

	upsStateFrame( upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(parent,first)
	,	UPS_INIT	 ( Node )
	,	UPS_INIT	 ( StateNode )
	,	UPS_INIT	 ( ProbeMask )
	,	UPS_INIT	 ( LatentAction )
	,	UPS_INIT_LAST( Offset, Node )
	{
	}

	upsStateFrame( const upsStateFrame& base, upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(base,parent,first)
	,	UPS_COPY	 ( Node )
	,	UPS_COPY	 ( StateNode )
	,	UPS_COPY	 ( ProbeMask )
	,	UPS_COPY	 ( LatentAction )
	,	UPS_COPY_LAST( Offset, Node )
	{
	}

	virtual void Serialize( upArchive& A ) 
	{ 
		A << Node;
		A << StateNode;
		A << ProbeMask;
		A << LatentAction;
		if( StateNode.GetData() != 0 )
			A << Offset;
		else
			Offset.GetData() = 0;
	}

	//virtual void SerializeDelta( upArchive& A, upsObject& other, updTran& tran )
	//{
	//	Serialize(A);
	//}
};

#pragma warning( pop )

// ============================================================================
// uppObject
// ============================================================================
class uppObject
{
public:
	uppPkg&				Pkg;
	uppExportItem&		Item;
	upsStateFrame		StateFrame;
	uppProperties		Properties;
	uppObjData*			ObjData;

public:

	uppObject( uppPkg& pkg, uppExportItem& item ) : Pkg(pkg), Item(item), ObjData(0)
	{
		item.Object = this;
		Properties.reserve(8);
	}

	~uppObject()
	{
		if( ObjData )
			delete ObjData;
	}

	wxString GetInfo( upArchive& A );
	
	void Serialize( upArchive& A );
};



#endif UPPOBJECT_H
// ============================================================================
//	The End.
// ============================================================================