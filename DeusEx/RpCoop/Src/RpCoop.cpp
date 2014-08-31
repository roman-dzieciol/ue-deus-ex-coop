/*=============================================================================
	RpCoop.cpp
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/

// Includes.
#include "RpCoop.h"


/*-----------------------------------------------------------------------------
	The following must be done once per package (.dll).
-----------------------------------------------------------------------------*/

// This is some necessary C++/UnrealScript glue logic.
// If you forget this, you get a VC++ linker errors like:
// SampleClass.obj : error LNK2001: unresolved external symbol "class FName  RPCOOP_SampleEvent" (?RPCOOP_SampleEvent@@3VFName@@A)
#define NAMES_ONLY
#define AUTOGENERATE_NAME(name) RPCOOP_API FName RPCOOP_##name;
#define AUTOGENERATE_FUNCTION(cls,idx,name) IMPLEMENT_FUNCTION(cls,idx,name)
#include "RpCoopScript.h"
#undef AUTOGENERATE_FUNCTION
#undef AUTOGENERATE_NAME
#undef NAMES_ONLY


void RegisterRpCoopNames()
{
	static INT Registered=0;
	if(!Registered++)
	{
		#define NAMES_ONLY
		#define AUTOGENERATE_NAME(name) extern RPCOOP_API FName RPCOOP_##name; RPCOOP_##name=FName(TEXT(#name),FNAME_Intrinsic);
		#define AUTOGENERATE_FUNCTION(cls,idx,name)
		#include "RpCoopScript.h"
		#undef DECLARE_NAME
		#undef NAMES_ONLY
	}
}


/*-----------------------------------------------------------------------------
	Implementation.
-----------------------------------------------------------------------------*/

// Package implementation.
IMPLEMENT_PACKAGE(RpCoop);

// Class implementation.
// If you forget this, you get a VC++ linker error like:
// SampleClass.obj : error LNK2001: unresolved external symbol "private: static class UClass  APxActor::PrivateStaticClass" (?PrivateStaticClass@APxActor@@0VUClass@@A)

IMPLEMENT_CLASS(URpCanvas);
IMPLEMENT_CLASS(URpGameEngine);

RPCOOP_API FRpLineBatcher	GLineBatcher;
RPCOOP_API class URpGameEngine*	GRpEngine;
RPCOOP_API UBOOL GRpFileOverride;
RPCOOP_API FString GRpCacheSrc;
RPCOOP_API FString GRpCacheDest;



/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
