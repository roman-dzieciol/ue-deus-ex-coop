// ============================================================================
//	Rp.h
//	Copyright 2007 Roman Dzieciol. All Rights Reserved.
// ============================================================================

// Includes.
#include "RpCoop.h"


/* ----------------------------------------------------------------------------
	SwTimeStr.
---------------------------------------------------------------------------- */
FString SwTimeStr( UBOOL bShort )
{	
	INT tYear, tMonth, tDayOfWeek, tDay, tHour, tMin, tSec, tMSec;
	appSystemTime( tYear, tMonth, tDayOfWeek, tDay, tHour, tMin, tSec, tMSec );
	FString Result = FString::Printf( TEXT("[%02d:%02d:%03d]"), tMin, tSec, tMSec );
	return Result;
}


/* ----------------------------------------------------------------------------
	ToStr :: UObject
---------------------------------------------------------------------------- */
FString ToStr( UObject* p, UBOOL bShort )
{
	FString s = SwGetName(p);
	if( p == NULL || bShort )
		return s;
	
	s += RP_SEP + FString::Printf(TEXT("%s"),p->GetFullName());
	s += RP_SEP + FString::Printf(TEXT("%.08x"),p->GetFlags());
	return s;
}



/* ----------------------------------------------------------------------------
	ToStr :: AActor
---------------------------------------------------------------------------- */
FString ToStr( AActor* p, UBOOL bShort )
{
	FString s = SwGetName(p);
	if( p == NULL || bShort )
		return s;
	
	s += RP_SEP + FString::Printf(TEXT("%.08x"),p->GetFlags());
	s += RP_SEP + FString::Printf(TEXT("%d%d%d"),p->Role,p->RemoteRole,p->Level->NetMode);
	
	return s;
}

/* ----------------------------------------------------------------------------
	ToStr :: ETravelType
---------------------------------------------------------------------------- */
FString ToStr( ETravelType a, UBOOL bShort )
{
	switch( a )
	{
	case TRAVEL_Absolute: return FString(TEXT("ABSOLUTE"));
	case TRAVEL_Partial: return FString(TEXT("PARTIAL"));
	case TRAVEL_Relative: return FString(TEXT("RELATIVE"));
	default: return FString(TEXT("UNKNOWN"));
	}
}


/* ----------------------------------------------------------------------------
	ToStr :: ENetRole
---------------------------------------------------------------------------- */
FString ToStr( ENetRole a, UBOOL bShort )
{
	switch( a )
	{
	case ROLE_None: return FString(TEXT("ROLE_None"));
	case ROLE_DumbProxy: return FString(TEXT("ROLE_DumbProxy"));
	case ROLE_SimulatedProxy: return FString(TEXT("ROLE_SimulatedProxy"));
	case ROLE_AutonomousProxy: return FString(TEXT("ROLE_AutonomousProxy"));
	case ROLE_Authority: return FString(TEXT("ROLE_Authority"));
	default: return FString(TEXT("UNKNOWN"));
	}
}

/* ----------------------------------------------------------------------------
	ToStr :: FMemStack
---------------------------------------------------------------------------- */
FString ToStr( FMemStack* p, UBOOL bShort )
{
	return FString::Printf(TEXT("MS:%p %d"), p, p->GetByteCount()); 
}

/* ----------------------------------------------------------------------------
	ToStr :: FCheckResult
---------------------------------------------------------------------------- */
FString ToStr( FCheckResult* c, UBOOL bShort )
{
	FString S;
	int i = 0;

	while( c )
	{
		FString aname;
		if( c->Actor )
			aname = c->Actor->GetFullName();

		FString pname;
		if( c->Primitive )
			pname = c->Primitive->GetFullName();

		S += FString::Printf( TEXT("FCR[%d]< P[%s] L[%s] N[%s] T[%f] I[%d] A[%s] N[%p] > ")
		, i
		, *pname
		, *ToStr(c->Location)
		, *ToStr(c->Normal)
		, c->Time
		, c->Item
		, *aname
		, c->Next
		);


		i++;
		c = c->GetNext();
	}

	return S;
}

/* ----------------------------------------------------------------------------
	ToStr :: ULevel
---------------------------------------------------------------------------- */
FString ToStr( ULevel* p, UBOOL bShort )
{
	FString s = FString::Printf(TEXT("LV:%p"), p); 
	if( p == NULL || bShort )
		return s;
	
	s += RP_SEP + FString::Printf(TEXT("%s"),p->GetFullName());
	return s;
}


/* ----------------------------------------------------------------------------
	ToStr :: FFileManager
---------------------------------------------------------------------------- */
FString ToStr( FFileManager* p, UBOOL bShort )
{
	FString s = FString::Printf(TEXT("FM:%p"), p); 
	if( p == NULL || bShort )
		return s;
	return s;
}

/* ----------------------------------------------------------------------------
	ToStr :: USound
---------------------------------------------------------------------------- */
FString ToStr( FSoundData p, UBOOL bShort )
{
	FString s = FString::Printf(TEXT("%d"), p.Num()); 
	return s;
}


/* ----------------------------------------------------------------------------
	ToStr :: USound
---------------------------------------------------------------------------- */
FString ToStr( USound* p, UBOOL bShort )
{
	FString s = FString::Printf(TEXT("SO:%p"), p); 
	if( p == NULL || bShort )
		return s;

	s += RP_SEP + p->GetName();
	s += RP_SEP + *ToStr(p->FileType);
	s += RP_SEP + *ToStr(p->OriginalSize);
	s += RP_SEP + *ToStr(p->Duration);
	s += RP_SEP + FString::Printf(TEXT("%p"), p->Handle); 
	//if( p->Handle )
	//	s += RP_SEP + *ToStr(p->Data);

	return s;
}

/* ----------------------------------------------------------------------------
	ToStr :: UMusic
---------------------------------------------------------------------------- */
FString ToStr( UMusic* p, UBOOL bShort )
{
	FString s = FString::Printf(TEXT("MU:%p"), p); 
	if( p == NULL || bShort )
		return s;

	s += RP_SEP + p->GetName();
	//s += RP_SEP + *ToStr(p->FileType);

	return s;
}

/* ----------------------------------------------------------------------------
	ToStr :: FCoords
---------------------------------------------------------------------------- */
FString ToStr( const FCoords p, UBOOL bShort )
{
	FString s = *ToStr(p.Origin);
	if( bShort )
		return s;

	s += RP_SEP + *ToStr(p.XAxis);
	s += RP_SEP + *ToStr(p.YAxis);
	s += RP_SEP + *ToStr(p.ZAxis);
	return s;
}


/* ----------------------------------------------------------------------------
	ToStr :: FPointRegion
---------------------------------------------------------------------------- */
FString ToStr( const FPointRegion p, UBOOL bShort )
{
	FString s = *ToStr(p.Zone);
	if( bShort )
		return s;

	s += RP_SEP + *ToStr(p.iLeaf);
	s += RP_SEP + *ToStr(p.ZoneNumber);
	return s;
}



/*-----------------------------------------------------------------------------
	ToStr :: UChannel
-----------------------------------------------------------------------------*/
FString ToStr( UChannel* p, UBOOL bShort )
{
	FString s = FString::Printf(TEXT("CH:%p"), p); 
	if( p == NULL || bShort )
		return s;

	switch( p->ChType )
	{
		case CHTYPE_None:		s += RP_SEP + FString::Printf(TEXT("NONE"));  break;
		case CHTYPE_Control:	s += RP_SEP + FString::Printf(TEXT("CTRL"));  break;
		case CHTYPE_Actor:		s += RP_SEP + FString::Printf(TEXT("ACTOR"));  break;
		case CHTYPE_File:		s += RP_SEP + FString::Printf(TEXT("FILE"));  break;
		default:				s += RP_SEP + FString::Printf(TEXT("UNKNOWN"));  break;
	}

	s += RP_SEP + FString::Printf(TEXT("%p"), p->Connection); 
	s += RP_SEP + FString::Printf(TEXT("%d"), p->OpenAcked); 
	s += RP_SEP + FString::Printf(TEXT("%d"), p->Closing); 
	s += RP_SEP + FString::Printf(TEXT("%d"), p->ChIndex); 
	s += RP_SEP + FString::Printf(TEXT("%d"), p->OpenedLocally); 
	s += RP_SEP + FString::Printf(TEXT("%d"), p->OpenPacketId); 
	s += RP_SEP + FString::Printf(TEXT("%d"), p->OpenTemporary); 
	s += RP_SEP + FString::Printf(TEXT("%d"), p->NumInRec); 
	s += RP_SEP + FString::Printf(TEXT("%d"), p->NumOutRec); 
	s += RP_SEP + FString::Printf(TEXT("%p"), p->InRec); 
	s += RP_SEP + FString::Printf(TEXT("%p"), p->OutRec); 


	switch( p->ChType )
	{
		case CHTYPE_None: 
		{
			break;
		}

		case CHTYPE_Control:
		{
			break;
		}

		case CHTYPE_Actor:
		{
			break;
		}

		case CHTYPE_File: 
		{
			UFileChannel* fc = (UFileChannel*)p; 
			s += RP_SEP + FString::Printf(TEXT("%s"), fc->Filename); 
			s += RP_SEP + FString::Printf(TEXT("%s"), fc->PrettyName); 
			s += RP_SEP + FString::Printf(TEXT("%s"), fc->Error); 
			s += RP_SEP + FString::Printf(TEXT("%p"), fc->FileAr); 
			s += RP_SEP + FString::Printf(TEXT("%d"), fc->Transfered); 
			s += RP_SEP + FString::Printf(TEXT("%d"), fc->PackageIndex); 
			break;
		}

		default:
		{
			break;
		}
	}
	
	return s;
}




/*-----------------------------------------------------------------------------
	ToStr :: UNetConnection
-----------------------------------------------------------------------------*/
FString ToStr( UNetConnection* p, UBOOL bShort )
{
	FString s = FString::Printf(TEXT("NC:%p"), p); 
	if( p == NULL || bShort )
		return s;

	switch( p->State )
	{
		case USOCK_Invalid:	s += RP_SEP + FString::Printf(TEXT("Invalid")); break;
		case USOCK_Closed:	s += RP_SEP + FString::Printf(TEXT("Closed")); break;
		case USOCK_Pending:	s += RP_SEP + FString::Printf(TEXT("Pending")); break;
		case USOCK_Open:	s += RP_SEP + FString::Printf(TEXT("Open")); break;
		default:			s += RP_SEP + FString::Printf(TEXT("Unknown")); break;
	}

	s += RP_SEP + FString::Printf(TEXT("%s"),*p->URL.String());
	s += RP_SEP + FString::Printf(TEXT("%.8x"),p->UserFlags);
	//s += RP_SEP + FString::Printf(TEXT(" | %s"),*p->LowLevelDescribe());

	for( TArray<UChannel*>::TIterator it=p->OpenChannels; it; ++it )
	{
		UChannel* c = *it;
		s += FString::Printf(TEXT("<%s>"),*ToStr(c));
	}
	
	return s;
}


/*-----------------------------------------------------------------------------
	ToStr :: FPackageInfo
-----------------------------------------------------------------------------*/
FString ToStr( FPackageInfo* p, UBOOL bShort )
{
	FString s = FString::Printf(TEXT("PI:%p"), p); 
	if( p == NULL || bShort )
		return s;

	s += RP_SEP + FString::Printf(TEXT("%s"),*p->URL);
	s += RP_SEP + FString::Printf(TEXT("%p"),p->Linker);
	//s += RP_SEP + FString::Printf(TEXT("%p"),p->Parent);
	s += RP_SEP + FString::Printf(TEXT("%s"),p->Guid.String());
	s += RP_SEP + FString::Printf(TEXT("%d"),p->FileSize);
	//s += RP_SEP + FString::Printf(TEXT("%d"),p->ObjectBase);
	//s += RP_SEP + FString::Printf(TEXT("%d"),p->ObjectCount);
	//s += RP_SEP + FString::Printf(TEXT("%d"),p->NameBase);
	//s += RP_SEP + FString::Printf(TEXT("%d"),p->NameCount);
	//s += RP_SEP + FString::Printf(TEXT("%d"),p->LocalGeneration);
	//s += RP_SEP + FString::Printf(TEXT("%d"),p->RemoteGeneration);
	//s += RP_SEP + FString::Printf(TEXT("%d"),p->PackageFlags);
	return s;
}

/*-----------------------------------------------------------------------------
	ToStr :: UPackageMap
-----------------------------------------------------------------------------*/
FString ToStr( UPackageMap* p, UBOOL bShort )
{
	FString s = FString::Printf(TEXT("PM:%p"), p); 
	if( p == NULL || bShort )
		return s;

	/*for( TArray<FPackageInfo>::TIterator it(p->List); it; ++it )
	{
		FPackageInfo* f = &(*it)
		s += TEXT("\n") + FString::Printf(TEXT("%s"),*ToStr(f));
	}*/
	return s;
}


/*-----------------------------------------------------------------------------
	ToStr :: FNetworkNotify
-----------------------------------------------------------------------------*/
FString ToStr( FNetworkNotify* p, UBOOL bShort )
{
	FString s = FString::Printf(TEXT("NN:%p"), p); 
	if( p == NULL || bShort )
		return s;
	
	return s;
}



/*-----------------------------------------------------------------------------
	ToStr :: UPendingLevel
-----------------------------------------------------------------------------*/
FString ToStr( UPendingLevel* p, UBOOL bShort )
{
	FString s = FString::Printf(TEXT("PL:%p"), p); 
	if( p == NULL || bShort )
		return s;
	
	s += RP_SEP + FString::Printf(TEXT("%s"),p->GetFullName());
	return s;
}



// ============================================================================
//  EOF
// ============================================================================