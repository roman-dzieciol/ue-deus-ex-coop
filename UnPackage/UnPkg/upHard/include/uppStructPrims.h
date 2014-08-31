// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	uppStructPrims.h
// ============================================================================
#ifndef UPPSTRUCTPRIMS_H
#define UPPSTRUCTPRIMS_H

#include "uppStructObjs.h"

#pragma warning( push )
#pragma warning( disable : 4355 )

// ----------------------------------------------------------------------------
//  upspFloat :: because -0, +0 and NaN's should be unique
// ----------------------------------------------------------------------------
struct upspFloat
{
	float F;

	friend bool operator == ( const upspFloat& a, const upspFloat& b ) 
	{ 
		return memcmp(&a.F,&b.F,sizeof(float)) == 0;
	}

	friend bool operator != ( const upspFloat& a, const upspFloat& b ) 
	{ 
		return memcmp(&a.F,&b.F,sizeof(float)) != 0;
	}
	
	friend void operator << ( upArchive& A, upspFloat& D )
	{
		A << D.F;
	}
};


// ----------------------------------------------------------------------------
//  upspVector
// ----------------------------------------------------------------------------
struct upspVector
{
	upspFloat X;
	upspFloat Y;
	upspFloat Z;

	friend bool operator == ( const upspVector& a, const upspVector& b ) 
	{ 
		return a.X == b.X && a.Y == b.Y && a.Z == b.Z;
	}

	friend bool operator != ( const upspVector& a, const upspVector& b ) 
	{ 
		return !( a == b );
	}
	
	friend void operator << ( upArchive& A, upspVector& D )
	{
		A << D.X;
		A << D.Y;
		A << D.Z;
	}
};

typedef upstData<upspVector> upsVector;



// ----------------------------------------------------------------------------
//  upspPlane
// ----------------------------------------------------------------------------
struct upspPlane
{
	upspFloat X;
	upspFloat Y;
	upspFloat Z;	
	upspFloat W;

	friend bool operator == ( const upspPlane& a, const upspPlane& b ) 
	{ 
		return a.X == b.X && a.Y == b.Y && a.Z == b.Z && a.W == b.W;
	}

	friend bool operator != ( const upspPlane& a, const upspPlane& b ) 
	{ 
		return !( a == b );
	}
	
	friend void operator << ( upArchive& A, upspPlane& D )
	{
		A << D.X;
		A << D.Y;
		A << D.Z;
		A << D.W;
	}
};

typedef upstData<upspPlane> upsPlane;


// ----------------------------------------------------------------------------
//  upspBoundingBox
// ----------------------------------------------------------------------------
struct upspBoundingBox
{
	upspVector Min;
	upspVector Max;
	byte Valid;

	friend bool operator == ( const upspBoundingBox& a, const upspBoundingBox& b ) 
	{ 
		return	a.Min == b.Min && a.Max == b.Max && a.Valid == b.Valid;
	}

	friend bool operator != ( const upspBoundingBox& a, const upspBoundingBox& b ) 
	{ 
		return !( a == b );
	}
	
	friend void operator << ( upArchive& A, upspBoundingBox& D )
	{
		A << D.Min;
		A << D.Max;
		A << D.Valid;
	}
};

typedef upstData<upspBoundingBox> upsBoundingBox;


// ----------------------------------------------------------------------------
//  upspBoundingSphere
// ----------------------------------------------------------------------------
struct upspBoundingSphere
{
	upspVector Center;
	upspFloat Radius;

	friend bool operator == ( const upspBoundingSphere& a, const upspBoundingSphere& b ) 
	{ 
		return	a.Center == b.Center && a.Radius == b.Radius;
	}

	friend bool operator != ( const upspBoundingSphere& a, const upspBoundingSphere& b ) 
	{ 
		return !( a == b );
	}
	
	friend void operator << ( upArchive& A, upspBoundingSphere& D )
	{
		A << D.Center;
		A << D.Radius;
	}
};

typedef upstData<upspBoundingSphere> upsBoundingSphere;


// ----------------------------------------------------------------------------
//  upspPrimitive
// ----------------------------------------------------------------------------
struct upspPrimitive
{
	upspBoundingBox BoundingBox;
	upspBoundingSphere BoundingSphere;

	friend bool operator == ( const upspPrimitive& a, const upspPrimitive& b ) 
	{ 
		return	a.BoundingBox == b.BoundingBox && a.BoundingSphere == b.BoundingSphere;
	}

	friend bool operator != ( const upspPrimitive& a, const upspPrimitive& b ) 
	{ 
		return !( a == b );
	}
	
	friend void operator << ( upArchive& A, upspPrimitive& D )
	{
		A << D.BoundingBox;
		A << D.BoundingSphere;
	}
};

typedef upstData<upspPrimitive> upsPrimitive;


// ----------------------------------------------------------------------------
//  upspVert
// ----------------------------------------------------------------------------
struct upspVert
{
	uppIndex pVertex;
	uppIndex iSide;

	friend bool operator == ( const upspVert& a, const upspVert& b ) 
	{ 
		return	a.pVertex == b.pVertex && a.iSide == b.iSide;
	}

	friend bool operator != ( const upspVert& a, const upspVert& b ) 
	{ 
		return !( a == b );
	}
		
	friend void operator << ( upArchive& A, upspVert& D )
	{
		A << D.pVertex;
		A << D.iSide;
	}
};

typedef upstData<upspVert> upsVert;



// ----------------------------------------------------------------------------
//  upspMapItem
// ----------------------------------------------------------------------------
struct upspMapItem
{
	uppLASCIIZ Key;
	uppLASCIIZ Value;

	friend bool operator == ( const upspMapItem& a, const upspMapItem& b ) 
	{ 
		return	a.Key == b.Key && a.Value == b.Value;
	}

	friend bool operator != ( const upspMapItem& a, const upspMapItem& b ) 
	{ 
		return !( a == b );
	}
		
	friend void operator << ( upArchive& A, upspMapItem& D )
	{
		A << D.Key;
		A << D.Value;
	}
};

typedef upstData<upspMapItem> upsMapItem;

// ----------------------------------------------------------------------------
//  upspAIEventSettings
// ----------------------------------------------------------------------------
struct upspAIEventSettings
{
public:
	upspFloat visual;
	upspFloat audio;
	upspFloat audioRadius;
	upspFloat smell;

	friend bool operator == ( const upspAIEventSettings& a, const upspAIEventSettings& b ) 
	{ 
		return	a.visual == b.visual 
			 && a.audio == b.audio
			 && a.audioRadius == b.audioRadius
			 && a.smell == b.smell;
	}

	friend bool operator != ( const upspAIEventSettings& a, const upspAIEventSettings& b ) 
	{ 
		return !( a == b );
	}
		
	friend void operator << ( upArchive& A, upspAIEventSettings& D )
	{
		A << D.visual;
		A << D.audio;
		A << D.audioRadius;
		A << D.smell;
	}
};

typedef upstData<upspAIEventSettings> upsAIEventSettings;



#pragma warning( pop )

#endif UPPSTRUCTPRIMS_H
// ============================================================================
//	The End.
// ============================================================================