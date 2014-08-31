// ============================================================================
//	Rp.h
//	Copyright 2007 Roman Dzieciol. All Rights Reserved.
// ============================================================================
#pragma once




/* ----------------------------------------------------------------------------
	SwTimeStr.
---------------------------------------------------------------------------- */
extern FString SwTimeStr( UBOOL bShort=0 );


/* ----------------------------------------------------------------------------
	SwGetName
---------------------------------------------------------------------------- */
inline FString SwGetName( UObject* p, UBOOL bShort=0 )
{
	return p ? p->GetName() : TEXT("None");
}


/* -----------------------------------------------------------------------------
	SwGetPath
---------------------------------------------------------------------------- */
inline FString SwGetPathName( UObject* p, UBOOL bShort=0 )
{
	return p ? p->GetPathName() : TEXT("None");
}



/* ----------------------------------------------------------------------------
	ToStr :: Basic
---------------------------------------------------------------------------- */
inline FString ToStr( const FString& p, UBOOL bShort=0 )		{ return p; }
inline FString ToStr( const TCHAR* p, UBOOL bShort=0 )			{ return FString(p); }
inline FString ToStr( const FName p, UBOOL bShort=0 )			{ return *p; }
inline FString ToStr( const FVector p, UBOOL bShort=0 )			{ return FString::Printf(TEXT("%f,%f,%f"), p.X, p.Y, p.Z); }
inline FString ToStr( const FRotator p, UBOOL bShort=0 )		{ return FString::Printf(TEXT("%d,%d,%d"), p.Pitch, p.Yaw, p.Roll); }
inline FString ToStr( const float p, UBOOL bShort=0 )			{ return FString::Printf(TEXT("%f"), p);  }
inline FString ToStr( const unsigned int p, UBOOL bShort=0 )	{ return FString::Printf(TEXT("%u"), p);  }
inline FString ToStr( const signed int p, UBOOL bShort=0 )		{ return FString::Printf(TEXT("%i"), p);  }
inline FString ToStr( const unsigned long p, UBOOL bShort=0 )	{ return FString::Printf(TEXT("%lu"), p);  }
inline FString ToStr( const signed long p, UBOOL bShort=0 )		{ return FString::Printf(TEXT("%li"), p);  }


/* ----------------------------------------------------------------------------
	ToStr :: Complex
---------------------------------------------------------------------------- */
extern FString ToStr( UObject* p, UBOOL bShort=1 );
extern FString ToStr( AActor* p, UBOOL bShort=1 );
extern FString ToStr( ETravelType a, UBOOL bShort=0 );
extern FString ToStr( ENetRole a, UBOOL bShort=0 );
extern FString ToStr( FMemStack* p, UBOOL bShort=0 );
extern FString ToStr( FCheckResult* c, UBOOL bShort=0 );
extern FString ToStr( ULevel* p, UBOOL bShort=0 );
extern FString ToStr( FFileManager* p, UBOOL bShort=0 );
extern FString ToStr( FSoundData p, UBOOL bShort=0 );
extern FString ToStr( USound* p, UBOOL bShort=0 );
extern FString ToStr( UMusic* p, UBOOL bShort=0 );
extern FString ToStr( const FCoords p, UBOOL bShort=0 );
extern FString ToStr( const FPointRegion p, UBOOL bShort=0 );
extern FString ToStr( UChannel* p, UBOOL bShort=0 );
extern FString ToStr( UNetConnection* p, UBOOL bShort=0 );
extern FString ToStr( FPackageInfo* p, UBOOL bShort=0 );
extern FString ToStr( UPackageMap* p, UBOOL bShort=0 );
extern FString ToStr( FNetworkNotify* p, UBOOL bShort=0 );
extern FString ToStr( UPendingLevel* p, UBOOL bShort=0 );


// ============================================================================
//  EOF
// ============================================================================