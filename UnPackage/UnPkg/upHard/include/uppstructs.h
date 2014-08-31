// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	uppStructs.h
// ============================================================================
#ifndef UPPSTRUCTS_H
#define UPPSTRUCTS_H

#include "uppStructObjs.h"
#include "uppStructPrims.h"

#pragma warning( push )
#pragma warning( disable : 4355 )


// ----------------------------------------------------------------------------
//  upsPoly
// ----------------------------------------------------------------------------
struct upsPoly : public upsoContainer
{
	//upsIndex VertCount;
	upsVector Base;
	upsVector Normal;
	upsVector TextureU;
	upsVector TextureV;
	upstArray<upsVector> Verts;
	upsDword PolyFlags;
	upsObjIndex Actor;
	upsObjIndex Texture;
	upsNameIndex ItemName;
	upsIndex iLink;
	upsIndex iBrushPoly;
	upsWord PanU;
	upsWord PanV;


	upsPoly( upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(parent,first)
	,	UPS_INIT	 ( Base )
	,	UPS_INIT	 ( Normal )
	,	UPS_INIT	 ( TextureU )
	,	UPS_INIT	 ( TextureV )
	,	UPS_INIT	 ( Verts )
	,	UPS_INIT	 ( PolyFlags )
	,	UPS_INIT	 ( Actor )
	,	UPS_INIT	 ( Texture )
	,	UPS_INIT	 ( ItemName )
	,	UPS_INIT	 ( iLink )
	,	UPS_INIT	 ( iBrushPoly )
	,	UPS_INIT	 ( PanU )
	,	UPS_INIT_LAST( PanV, Base )
	{
	}

	upsPoly( const upsPoly& base, upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(base,parent,first)
	,	UPS_COPY	 ( Base )
	,	UPS_COPY	 ( Normal )
	,	UPS_COPY	 ( TextureU )
	,	UPS_COPY	 ( TextureV )
	,	UPS_COPY	 ( Verts )
	,	UPS_COPY	 ( PolyFlags )
	,	UPS_COPY	 ( Actor )
	,	UPS_COPY	 ( Texture )
	,	UPS_COPY	 ( ItemName )
	,	UPS_COPY	 ( iLink )
	,	UPS_COPY	 ( iBrushPoly )
	,	UPS_COPY	 ( PanU )
	,	UPS_COPY_LAST( PanV, Base )
	{
	}

	virtual void Serialize( upArchive& A ) 
	{ 
		// serialize vertcount manually
		uppIndex count( static_cast<int>(Verts.size()) );
		A << count;

		if( A.IsLoading() )
		{
			Verts.reserve(count);
			Verts.resize(count);
		}

		upsoContainer::Serialize(A);
	}

	virtual void SerializeDelta( upArchive& A, upsObject& other, updTran& tran )
	{
		// serialize vertcount manually
		uppIndex count( static_cast<int>(Verts.size()) );
		A << count;

		if( A.IsLoading() )
		{
			Verts.reserve(count);
			Verts.resize(count);
		}

		upsoContainer::SerializeDelta(A,other,tran);
	}

	virtual void LoadBasis( upsObject& other, updTran& tran )
	{
		// Get vertcount from basis
		upsPoly& bother = static_cast<upsPoly&>(other);
		uppIndex count( static_cast<int>(bother.Verts.size()) );

		Verts.reserve(count);
		Verts.resize(count);

		upsoContainer::LoadBasis(other,tran);
	}
};


// ----------------------------------------------------------------------------
//  upsPolys
// ----------------------------------------------------------------------------
class upsPolys : public upsoContainer
{
public:
	upsDword Unknown;
	upstComboArray<dword,upsPoly> Polys;

	upsPolys( upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(parent,first)
	,	UPS_INIT	 ( Unknown )
	,	UPS_INIT_LAST( Polys, Unknown )
	{
	}

	upsPolys( const upsPolys& base, upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(base,parent,first)
	,	UPS_COPY	 ( Unknown )
	,	UPS_COPY_LAST( Polys, Unknown )
	{
	}
	
	virtual void SerializeDelta( upArchive& A, upsObject& other, updTran& tran )
	{
		if( A.IsLoading() )
		{	
			int y =4;
		}
		else
		{
			int y =4;
		}

		upsoContainer::SerializeDelta(A,other,tran);
	}
};


// ----------------------------------------------------------------------------
//  upsNode
// ----------------------------------------------------------------------------
struct upsNode : public upsoContainer
{
	upsPlane	Plane;
	upsQword	ZoneMask;
	upsByte		NodeFlags;
	upsIndex	iVertPool;
	upsIndex	iSurf;
	upsIndex	iFront;
	upsIndex	iBack;
	upsIndex	iPlane;
	upsIndex	iCollisionBound;
	upsIndex	iRenderBound;
	upsByte		iZoneA;
	upsByte		iZoneB;
	upsByte		NumVertices;
	upsDword	iLeafA;
	upsDword	iLeafB;


	upsNode( upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(parent,first)
	,	UPS_INIT	 ( Plane )
	,	UPS_INIT	 ( ZoneMask )
	,	UPS_INIT	 ( NodeFlags )
	,	UPS_INIT	 ( iVertPool )
	,	UPS_INIT	 ( iSurf )
	,	UPS_INIT	 ( iFront )
	,	UPS_INIT	 ( iBack )
	,	UPS_INIT	 ( iPlane )
	,	UPS_INIT	 ( iCollisionBound )
	,	UPS_INIT	 ( iRenderBound )
	,	UPS_INIT	 ( iZoneA )
	,	UPS_INIT	 ( iZoneB )
	,	UPS_INIT	 ( NumVertices )
	,	UPS_INIT	 ( iLeafA )
	,	UPS_INIT_LAST( iLeafB, Plane )
	{
	}

	upsNode( const upsNode& base, upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(base,parent,first)
	,	UPS_COPY	 ( Plane )
	,	UPS_COPY	 ( ZoneMask )
	,	UPS_COPY	 ( NodeFlags )
	,	UPS_COPY	 ( iVertPool )
	,	UPS_COPY	 ( iSurf )
	,	UPS_COPY	 ( iFront )
	,	UPS_COPY	 ( iBack )
	,	UPS_COPY	 ( iPlane )
	,	UPS_COPY	 ( iCollisionBound )
	,	UPS_COPY	 ( iRenderBound )
	,	UPS_COPY	 ( iZoneA )
	,	UPS_COPY	 ( iZoneB )
	,	UPS_COPY	 ( NumVertices )
	,	UPS_COPY	 ( iLeafA )
	,	UPS_COPY_LAST( iLeafB, Plane )
	{
	}

	virtual void SerializeDelta( upArchive& A, upsObject& other, updTran& tran )
	{
		if( A.IsLoading() )
		{	
			int y =4;
		}
		else
		{
			int y =4;
		}

		upsoContainer::SerializeDelta(A,other,tran);
	}
};


// ----------------------------------------------------------------------------
//  upsSurf
// ----------------------------------------------------------------------------
struct upsSurf : public upsoContainer
{
	upsObjIndex Texture;
	upsDword PolyFlags;
	upsIndex pBase;
	upsIndex vNormal;
	upsIndex vTextureU;
	upsIndex vTextureV;
	upsIndex iLightMap;
	upsIndex iBrushPoly;
	upsWord PanU;
	upsWord PanV;
	upsObjIndex Actor;

	upsSurf( upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(parent,first)
	,	UPS_INIT	 ( Texture )
	,	UPS_INIT	 ( PolyFlags )
	,	UPS_INIT	 ( pBase )
	,	UPS_INIT	 ( vNormal )
	,	UPS_INIT	 ( vTextureU )
	,	UPS_INIT	 ( vTextureV )
	,	UPS_INIT	 ( iLightMap )
	,	UPS_INIT	 ( iBrushPoly )
	,	UPS_INIT	 ( PanU )
	,	UPS_INIT	 ( PanV )
	,	UPS_INIT_LAST( Actor, Texture )
	{
	}
	
	upsSurf( const upsSurf& base, upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(base,parent,first)
	,	UPS_COPY	 ( Texture )
	,	UPS_COPY	 ( PolyFlags )
	,	UPS_COPY	 ( pBase )
	,	UPS_COPY	 ( vNormal )
	,	UPS_COPY	 ( vTextureU )
	,	UPS_COPY	 ( vTextureV )
	,	UPS_COPY	 ( iLightMap )
	,	UPS_COPY	 ( iBrushPoly )
	,	UPS_COPY	 ( PanU )
	,	UPS_COPY	 ( PanV )
	,	UPS_COPY_LAST( Actor, Texture )
	{
	}
};


// ----------------------------------------------------------------------------
//  upsZone
// ----------------------------------------------------------------------------
struct upsZone : public upsoContainer
{
	upsObjIndex ZoneActor;
	upsQword Connectivity;
	upsQword Visibility;

	upsZone( upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(parent,first)
	,	UPS_INIT	 ( ZoneActor )
	,	UPS_INIT	 ( Connectivity )
	,	UPS_INIT_LAST( Visibility, ZoneActor )
	{
	}
	
	upsZone( const upsZone& base, upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(base,parent,first)
	,	UPS_COPY	 ( ZoneActor )
	,	UPS_COPY	 ( Connectivity )
	,	UPS_COPY_LAST( Visibility, ZoneActor )
	{
	}
};


// ----------------------------------------------------------------------------
//  upsLightMap
// ----------------------------------------------------------------------------
struct upsLightMap : public upsoContainer
{
	upsDword DataOffset;
	upsVector Pan;
	upsIndex UClamp;
	upsIndex VClamp;
	upsFloat UScale;
	upsFloat VScale;
	upsDword iLightActors;

	upsLightMap( upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(parent,first)
	,	UPS_INIT	 ( DataOffset )
	,	UPS_INIT	 ( Pan )
	,	UPS_INIT	 ( UClamp )
	,	UPS_INIT	 ( VClamp )
	,	UPS_INIT	 ( UScale )
	,	UPS_INIT	 ( VScale )
	,	UPS_INIT_LAST( iLightActors, DataOffset )
	{
	}
	
	upsLightMap( const upsLightMap& base, upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(base,parent,first)
	,	UPS_COPY	 ( DataOffset )
	,	UPS_COPY	 ( Pan )
	,	UPS_COPY	 ( UClamp )
	,	UPS_COPY	 ( VClamp )
	,	UPS_COPY	 ( UScale )
	,	UPS_COPY	 ( VScale )
	,	UPS_COPY_LAST( iLightActors, DataOffset )
	{
	}
};


// ----------------------------------------------------------------------------
//  upsLeaves
// ----------------------------------------------------------------------------
struct upsLeaves : public upsoContainer
{
	upsObjIndex iZone;
	upsIndex iPermeating;
	upsIndex iVolumetric;
	upsQword VisibleZones;

	upsLeaves( upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(parent,first)
	,	UPS_INIT	 ( iZone )
	,	UPS_INIT	 ( iPermeating )
	,	UPS_INIT	 ( iVolumetric )
	,	UPS_INIT_LAST( VisibleZones, iZone )
	{
	}
	
	upsLeaves( const upsLeaves& base, upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(base,parent,first)
	,	UPS_COPY	 ( iZone )
	,	UPS_COPY	 ( iPermeating )
	,	UPS_COPY	 ( iVolumetric )
	,	UPS_COPY_LAST( VisibleZones, iZone )
	{
	}
};


// ----------------------------------------------------------------------------
//  upsLevelBase
// ----------------------------------------------------------------------------
class upsLevelBase : public upsoContainer
{
public:
	upsDword Unknown;
	upstComboArray<dword,upsObjIndex> Actors;

	upsLevelBase( upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(parent,first)
	,	UPS_INIT	 ( Unknown )
	,	UPS_INIT_LAST( Actors, Unknown )
	{
	}
	
	upsLevelBase( const upsLevelBase& base, upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(base,parent,first)
	,	UPS_COPY	 ( Unknown )
	,	UPS_COPY_LAST( Actors, Unknown )
	{
	}
};


// ----------------------------------------------------------------------------
//  upsReachSpec
// ----------------------------------------------------------------------------
class upsReachSpec : public upsoContainer
{
public:
	upsDword Distance;
	upsIndex Start;
	upsIndex _End;
	upsDword CollisionRadius;
	upsDword CollisionHeight;
	upsDword ReachFlags;
	upsByte bPruned;

	upsReachSpec( upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(parent,first)
	,	UPS_INIT	 ( Distance )
	,	UPS_INIT	 ( Start )
	,	UPS_INIT	 ( _End )
	,	UPS_INIT	 ( CollisionRadius )
	,	UPS_INIT	 ( CollisionHeight )
	,	UPS_INIT	 ( ReachFlags )
	,	UPS_INIT_LAST( bPruned, Distance )
	{
	}
	
	upsReachSpec( const upsReachSpec& base, upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(base,parent,first)
	,	UPS_COPY	 ( Distance )
	,	UPS_COPY	 ( Start )
	,	UPS_COPY	 ( _End )
	,	UPS_COPY	 ( CollisionRadius )
	,	UPS_COPY	 ( CollisionHeight )
	,	UPS_COPY	 ( ReachFlags )
	,	UPS_COPY_LAST( bPruned, Distance )
	{
	}
};


// ----------------------------------------------------------------------------
//  upsAIEvent
// ----------------------------------------------------------------------------
class upsAIEvent : public upsoContainer
{
public:
	upsIndex eventType;
	upsIndex eventActor;
	upsDword bBeingDestroyed;
	upsIndex nextEvent;

	upsAIEvent( upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(parent,first)
	,	UPS_INIT	 ( eventType )
	,	UPS_INIT	 ( eventActor )
	,	UPS_INIT	 ( bBeingDestroyed )
	,	UPS_INIT_LAST( nextEvent, eventType )
	{
	}
	
	upsAIEvent( const upsAIEvent& base, upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(base,parent,first)
	,	UPS_COPY	 ( eventType )
	,	UPS_COPY	 ( eventActor )
	,	UPS_COPY	 ( bBeingDestroyed )
	,	UPS_COPY_LAST( nextEvent, eventType )
	{
	}
};


// ----------------------------------------------------------------------------
//  upsURL
// ----------------------------------------------------------------------------
class upsURL : public upsoContainer
{
public:
	upsLASCIIZ Protocol;
	upsLASCIIZ Host;
	upsLASCIIZ Map;
	upsLASCIIZ Portal;
	upstComboArray<uppIndex,upsLASCIIZ> Options;
	upsDword Port;
	upsDword Valid;

	upsURL( upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(parent,first)
	,	UPS_INIT	 ( Protocol )
	,	UPS_INIT	 ( Host )
	,	UPS_INIT	 ( Map )
	,	UPS_INIT	 ( Portal )
	,	UPS_INIT	 ( Options )
	,	UPS_INIT	 ( Port )
	,	UPS_INIT_LAST( Valid, Protocol )
	{
	}
	
	upsURL( const upsURL& base, upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(base,parent,first)
	,	UPS_COPY	 ( Protocol )
	,	UPS_COPY	 ( Host )
	,	UPS_COPY	 ( Map )
	,	UPS_COPY	 ( Portal )
	,	UPS_COPY	 ( Options )
	,	UPS_COPY	 ( Port )
	,	UPS_COPY_LAST( Valid, Protocol )
	{
	}
};


// ----------------------------------------------------------------------------
//  upsModel
// ----------------------------------------------------------------------------
class upsModel : public upsoContainer
{
public:
	upsPrimitive							Primitive;
	upstComboArray<uppIndex,upsVector>		Vectors;
	upstComboArray<uppIndex,upsVector>		Points;

	upstComboArray<uppIndex,upsNode>		Nodes;
	upstComboArray<uppIndex,upsSurf>		Surfs;
	upstComboArray<uppIndex,upsVert>		Verts;

	upsDword								NumSharedSides;

	upstComboArray<dword,upsZone>			Zones;	

	upsObjIndex								Polys;
	upstComboArray<uppIndex,upsLightMap>	LightMaps;	
	upstComboArray<uppIndex,upsByte>		LightBits;
	upstComboArray<uppIndex,upsBoundingBox>	Bounds;
	upstComboArray<uppIndex,upsDword>		LeafHulls;
	upstComboArray<uppIndex,upsLeaves>		Leaves;
	upstComboArray<uppIndex,upsObjIndex>	Lights;
	upsDword								RootOutside;
	upsDword								Linked;


	upsModel( upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(parent,first)
	,	UPS_INIT	 ( Primitive )
	,	UPS_INIT	 ( Vectors )
	,	UPS_INIT	 ( Points )
	,	UPS_INIT	 ( Nodes )
	,	UPS_INIT	 ( Surfs )
	,	UPS_INIT	 ( Verts )
	,	UPS_INIT	 ( NumSharedSides )
	,	UPS_INIT	 ( Zones )
	,	UPS_INIT	 ( Polys )
	,	UPS_INIT	 ( LightMaps )
	,	UPS_INIT	 ( LightBits )
	,	UPS_INIT	 ( Bounds )
	,	UPS_INIT	 ( LeafHulls )
	,	UPS_INIT	 ( Leaves )
	,	UPS_INIT	 ( Lights )
	,	UPS_INIT	 ( RootOutside )
	,	UPS_INIT_LAST( Linked, Primitive )
	{
	}
	
	upsModel( const upsModel& base, upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(base,parent,first)
	,	UPS_COPY	 ( Primitive )
	,	UPS_COPY	 ( Vectors )
	,	UPS_COPY	 ( Points )
	,	UPS_COPY	 ( Nodes )
	,	UPS_COPY	 ( Surfs )
	,	UPS_COPY	 ( Verts )
	,	UPS_COPY	 ( NumSharedSides )
	,	UPS_COPY	 ( Zones )
	,	UPS_COPY	 ( Polys )
	,	UPS_COPY	 ( LightMaps )
	,	UPS_COPY	 ( LightBits )
	,	UPS_COPY	 ( Bounds )
	,	UPS_COPY	 ( LeafHulls )
	,	UPS_COPY	 ( Leaves )
	,	UPS_COPY	 ( Lights )
	,	UPS_COPY	 ( RootOutside )
	,	UPS_COPY_LAST( Linked, Primitive )
	{
	}
};


// ----------------------------------------------------------------------------
//  upsAIEventType
// ----------------------------------------------------------------------------
class upsAIEventType : public upsoContainer
{
public:
	upsNameIndex eventName;
	upsDword eventHash;
	upsObjIndex senders;
	upsObjIndex receivers;
	upsObjIndex nextEventType;

	upsAIEventType( upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(parent,first)
	,	UPS_INIT	 ( eventName )
	,	UPS_INIT	 ( eventHash )
	,	UPS_INIT	 ( senders )
	,	UPS_INIT	 ( receivers )
	,	UPS_INIT_LAST( nextEventType, eventName )
	{
	}
	
	upsAIEventType( const upsAIEventType& base, upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(base,parent,first)
	,	UPS_COPY	 ( eventName )
	,	UPS_COPY	 ( eventHash )
	,	UPS_COPY	 ( senders )
	,	UPS_COPY	 ( receivers )
	,	UPS_COPY_LAST( nextEventType, eventName )
	{
	}
};


// ----------------------------------------------------------------------------
//  upsLevel
// ----------------------------------------------------------------------------
class upsLevel : public upsoContainer
{
public:
	upsLevelBase LevelBase;
	upsURL URL;
	upsIndex Model;
	upstComboArray<uppIndex,upsReachSpec> ReachSpecs;
	upsFloat AproxTime;
	upsIndex FirstDeleted;
	upstFixedArray<16,upsIndex> TextBlocks;
	upstComboArray<uppIndex,upsMapItem> TravelInfo;

	upsLevel( upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(parent,first)
	,	UPS_INIT	 ( LevelBase )
	,	UPS_INIT	 ( URL )
	,	UPS_INIT	 ( Model )
	,	UPS_INIT	 ( ReachSpecs )
	,	UPS_INIT	 ( AproxTime )
	,	UPS_INIT	 ( FirstDeleted )
	,	UPS_INIT	 ( TextBlocks )
	,	UPS_INIT_LAST( TravelInfo, LevelBase )
	{
	}
	
	upsLevel( const upsLevel& base, upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(base,parent,first)
	,	UPS_COPY	 ( LevelBase )
	,	UPS_COPY	 ( URL )
	,	UPS_COPY	 ( Model )
	,	UPS_COPY	 ( ReachSpecs )
	,	UPS_COPY	 ( AproxTime )
	,	UPS_COPY	 ( FirstDeleted )
	,	UPS_COPY	 ( TextBlocks )
	,	UPS_COPY_LAST( TravelInfo, LevelBase )
	{
	}
};


// ----------------------------------------------------------------------------
//  upsAISenderEvent
// ----------------------------------------------------------------------------
class upsAISenderEvent : public upsoContainer
{
public:
	upsAIEvent Event;
	upstFixedArray<16,upsAIEventSettings> settings;
	upsAIEventSettings currentSettings;

	upsAISenderEvent( upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(parent,first)
	,	UPS_INIT	 ( Event )
	,	UPS_INIT	 ( settings )
	,	UPS_INIT_LAST( currentSettings, Event )
	{
	}
	
	upsAISenderEvent( const upsAISenderEvent& base, upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(base,parent,first)
	,	UPS_COPY	 ( Event )
	,	UPS_COPY	 ( settings )
	,	UPS_COPY_LAST( currentSettings, Event )
	{
	}
};


// ----------------------------------------------------------------------------
//  upsAIReceiverEvent
// ----------------------------------------------------------------------------
class upsAIReceiverEvent : public upsoContainer
{
public:
	upsAIEvent Event;
	upsNameIndex callback;
	upsNameIndex scoreCallback;
	upsDword bInvokeCallback;
	upsByte eventState;
	upsDword bCheckVisibility;
	upsDword bCheckDir;
	upsDword bCheckCylinder;
	upsDword bCheckLOS;
	upsDword bEventOn;
	upsFloat bestScore;
	upsObjIndex bestSender;
	upsDword nextSlot;
	upsObjIndex nextProcess;
	upsObjIndex prevProcess;

	upsAIReceiverEvent( upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(parent,first)
	,	UPS_INIT	 ( Event )
	,	UPS_INIT	 ( callback )
	,	UPS_INIT	 ( scoreCallback )
	,	UPS_INIT	 ( bInvokeCallback )
	,	UPS_INIT	 ( eventState )
	,	UPS_INIT	 ( bCheckVisibility )
	,	UPS_INIT	 ( bCheckDir )
	,	UPS_INIT	 ( bCheckCylinder )
	,	UPS_INIT	 ( bCheckLOS )
	,	UPS_INIT	 ( bEventOn )
	,	UPS_INIT	 ( bestScore )
	,	UPS_INIT	 ( bestSender )
	,	UPS_INIT	 ( nextSlot )
	,	UPS_INIT	 ( nextProcess )
	,	UPS_INIT_LAST( prevProcess, Event )
	{
	}
	
	upsAIReceiverEvent( const upsAIReceiverEvent& base, upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(base,parent,first)
	,	UPS_COPY	 ( Event )
	,	UPS_COPY	 ( callback )
	,	UPS_COPY	 ( scoreCallback )
	,	UPS_COPY	 ( bInvokeCallback )
	,	UPS_COPY	 ( eventState )
	,	UPS_COPY	 ( bCheckVisibility )
	,	UPS_COPY	 ( bCheckDir )
	,	UPS_COPY	 ( bCheckCylinder )
	,	UPS_COPY	 ( bCheckLOS )
	,	UPS_COPY	 ( bEventOn )
	,	UPS_COPY	 ( bestScore )
	,	UPS_COPY	 ( bestSender )
	,	UPS_COPY	 ( nextSlot )
	,	UPS_COPY	 ( nextProcess )
	,	UPS_COPY_LAST( prevProcess, Event )
	{
	}
};

// ----------------------------------------------------------------------------
//  upsEventManager
// ----------------------------------------------------------------------------
class upsEventManager : public upsoContainer
{
public:
	upsObjIndex level;
	upsDword refProcessing;
	upsDword deleteCount;
	upsDword currentSlot;
	upsObjIndex firstProcess;
	upstFixedArray<256,upsObjIndex> eventTable;

	upsEventManager( upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(parent,first)
	,	UPS_INIT	 ( level )
	,	UPS_INIT	 ( refProcessing )
	,	UPS_INIT	 ( deleteCount )
	,	UPS_INIT	 ( currentSlot )
	,	UPS_INIT	 ( firstProcess )
	,	UPS_INIT_LAST( eventTable, level )
	{
	}
	
	upsEventManager( const upsEventManager& base, upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsoContainer(base,parent,first)
	,	UPS_COPY	 ( level )
	,	UPS_COPY	 ( refProcessing )
	,	UPS_COPY	 ( deleteCount )
	,	UPS_COPY	 ( currentSlot )
	,	UPS_COPY	 ( firstProcess )
	,	UPS_COPY_LAST( eventTable, level )
	{
	}
};



#pragma warning( pop )

#endif UPPSTRUCTS_H
// ============================================================================
//	The End.
// ============================================================================