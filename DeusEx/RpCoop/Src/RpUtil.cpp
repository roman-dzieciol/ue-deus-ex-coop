// ============================================================================
//	Rp.h
//	Copyright 2007 Roman Dzieciol. All Rights Reserved.
// ============================================================================

// Includes.
#include "RpCoop.h"


/*-----------------------------------------------------------------------------
	RpDeleteDirectoryContents.
-----------------------------------------------------------------------------*/
void RpDeleteDirectoryContents( const TCHAR* Path )
{
	rpguard(RpDeleteDirectoryContents);
	if( !appStrlen(Path) )
		return;
	FString Spec = FString(Path) * TEXT("*");
	TArray<FString> List = GFileManager->FindFiles( *Spec, 1, 0 );
	for( INT i=0; i<List.Num(); i++ )
	{
		GFileManager->Delete(*(FString(Path) * List(i)),1,1);
	}
	unguard;
}

/*-----------------------------------------------------------------------------
	RpExplodePath
-----------------------------------------------------------------------------*/
void RpExplodePath( const FString& fullpath, FString& path, FString& file, FString& name, FString& ext )
{
	if( !fullpath.Split(TEXT("\\"),&path,&file,TRUE) )
		file = fullpath;

	if( !file.Split(TEXT("."),&name,&ext,TRUE) )
		name = file;
}


/*-----------------------------------------------------------------------------
	RpDumpLevels
-----------------------------------------------------------------------------*/
void RpDumpLevels()
{
	rpguard(RpDumpLevels);
	RP_LOG( RP_NAME, TEXT("%s >> %s ::"), RP_LOGP );

	for( TObjectIterator<ULevel> li; li; ++li )
	{
		RP_LOG( RP_NAME, TEXT("%s -> %s ::"), *SwTimeStr(), *ToStr(*li) );
		for( TTransArray<AActor*>::TIterator ai((*li)->Actors); ai; ++ai )
		{
			AActor* ap = *ai;
			if( !ap )
			{
				RP_LOG( RP_NAME, TEXT("%s -- %s :: [%s]"), *SwTimeStr(), *ToStr(*li), *ToStr(ap) );
				continue;
			}
			RP_LOG( RP_NAME, TEXT("%s -- %s :: [%s] [%d] [%d]"), *SwTimeStr(), *ToStr(*li), *ToStr(ap), ap->bJustTeleported, ap->bTicked  );

		}
		
		RP_LOG( RP_NAME, TEXT("%s <- %s ::"), *SwTimeStr(), *ToStr(*li) );
	}

	RP_LOG( RP_NAME, TEXT("%s << %s ::"), RP_LOGP );
	unguard;
}

/*-----------------------------------------------------------------------------
	RpDumpTravelInfo
-----------------------------------------------------------------------------*/
void RpDumpTravelInfo( const TMap<FString,FString>* TravelInfo )
{
	rpguard(RpDumpTravelInfo);
	RP_LOG( RP_NAME, TEXT("%s >> %s ::"), RP_LOGP );

	if( TravelInfo )
	{
		TMap<FString,FString> tmap = *TravelInfo;
		for( TMap<FString,FString>::TIterator it(tmap); it; ++it )
		{
			// TravelInfo contains more data than GLog can handle
			FString& s = it.Value();
			int i = s.InStr(TEXT("\r\n"));
			if( i != -1 )
				s = s.Left(i);
			RP_LOG( RP_NAME, TEXT("%s -- %s :: [%s],[%s]"), RP_LOGP, *it.Key(), *s  );
			//break; 
		}
	}

	RP_LOG( RP_NAME, TEXT("%s << %s ::"), RP_LOGP );
	unguard;
}


/*-----------------------------------------------------------------------------
	RpDumpRefs
-----------------------------------------------------------------------------*/
void RpDumpRefs()
{
	rpguard(RpDumpRefs);
	RP_LOG( RP_NAME, TEXT("%s >> %s :: Searching for actor references in non-actor objects.."), RP_LOGP );

	int counter=0;

	for( FObjectIterator oi; oi; ++oi )
	{
		UObject* op = *oi;
		
		// ignore actor classes
		UClass* cp = op->GetClass();
		if( !cp || cp->IsChildOf(AActor::StaticClass()) || !cp->RefLink  )
			continue;
		
		for( UObjectProperty* ref=cp->RefLink; ref; ref=ref->NextReference )
		{
			UClass* rcls = ref->PropertyClass;
			
			// only actor references pass through
			if( !rcls->IsChildOf(AActor::StaticClass()) )
				continue;

			// get var value
			TCHAR Temp[1024] = TEXT("");
			ref->ExportText( 0, Temp, (BYTE*)op, (BYTE*)op, PPF_Localized );

			// ignore empty
			static const FString nullstr(TEXT("None"));
			if( nullstr == Temp )
				continue;

			RP_LOG( RP_NAME, TEXT("%s -- %s :: %s.%s = %s"), RP_LOGP, op->GetPathName(), ref->GetName(), Temp );
			++counter;
		}
	}

	RP_LOG( RP_NAME, TEXT("%s << %s :: Found %d references."), RP_LOGP, counter );
	unguard;
}

/*
inline void RpDumpRefs()
{
	rpguard(RpDumpRefs);
	RP_LOG( RP_NAME, TEXT("%s >> %s ::"), RP_LOGP );

	for( TObjectIterator<UClass> li; li; ++li )
	{
		UClass* obj = *li;
		if(!obj 
		||  obj->IsChildOf(AActor::StaticClass())
		|| !obj->RefLink )
			continue;

		for( FObjectIterator ci; ci; ++ci )
		{
			UObject* co = *ci;
			if( !co || co->GetClass() != obj )
				continue;
			

			for( UObjectProperty* ref=obj->RefLink; ref; ref=ref->NextReference )
			{
				TCHAR Temp[1024]=TEXT("");//!!
				BYTE* Data = (BYTE*)co;
				BYTE* Delta = (BYTE*)co;
				ref->ExportText( 0, Temp, Data, Delta, PPF_Localized );

				if( strobj.InStr(TEXT("None")) != -1 )
					continue;

				FString strobj = Temp;
				FString strcls;

				int p = strobj.InStr(TEXT("'"));
				if( p == -1 )
					continue;
				strobj = strobj.Mid(p+1,strobj.Len()-p-1);

				
				//prop = FindObject<AActor>(ANY_PACKAGE,*strobj,0);

				p = strobj.InStr(TEXT("'"),TRUE);
				if( p == -1 )
					continue;
				strobj = strobj.Left(p);

				int p = strobj.InStr(TEXT("."),TRUE);
				if( p == -1 )
					continue;
				strobj = strobj.Mid(p+1,strobj.Len()-p-2);

				UObject* prop = NULL;

				//prop = FindObject<AActor>(ANY_PACKAGE,*strobj,0);
			
				for( TObjectIterator<AActor> ni; ni; ++ni )
				{
					UObject* no = *ni;
					if( !no )
						continue;

					if( strobj == no->GetName() )
					{
						prop = no;
						break;
					}
				}


//				UObject* prop = FindObject<AActor>(ANY_PACKAGE,*strobj,0);
				//ref->CopySingleValue(&prop,co);

				RP_LOG( RP_NAME, TEXT("%s -- %s :: <%s> %s=[%s] [%s] [%s]"), RP_LOGP, co->GetName(), ref->GetName(), Temp, *strobj, *ToStr(prop) );
			}
		}

		for( TArray<UObject*>::TIterator ci(UObject::GObjObjects); ci; ++ci )
		{
			UClass* co = *ci;
			if( !co || co->Is )
		}

		for( UObjectProperty* ref=obj->RefLink; ref; ref=ref->NextReference )
		{

		}

//		RP_LOG( RP_NAME, TEXT("%s -- %s :: [%s] [%s]"), RP_LOGP, *ToStr(obj), *ToStr(obj) );
	}

	RP_LOG( RP_NAME, TEXT("%s << %s ::"), RP_LOGP );
	unguard;
}
*/

/*inline void RpDumpRefs()
{
	rpguard(RpDumpRefs);
	RP_LOG( RP_NAME, TEXT("%s >> %s ::"), RP_LOGP );

	for( TObjectIterator<UObjectProperty> li; li; ++li )
	{
		UObjectProperty* obj = *li;
		if( !obj )
			continue;

		UObject* out = obj->GetOuter();
		if( !out || Cast<UFunction>(out) )
			continue;

		//UClass* cls = obj->PropertyClass;
		UClass* cls = obj->PropertyClass;
		if( !cls || !cls->IsChildOf(AActor::StaticClass()) )
			continue;
		
		TCHAR Temp[256]=TEXT("");//!!
		BYTE* Data = (BYTE*)out;
		BYTE* Delta = (BYTE*)out;
		obj->ExportText( 0, Temp, Data, Delta, PPF_Localized );

		FString s = Temp;
		RP_LOG( RP_NAME, TEXT("%s -- %s :: [%s] [%s] [%s]"), RP_LOGP, *ToStr(out), *ToStr(cls), *s );
	}

	RP_LOG( RP_NAME, TEXT("%s << %s ::"), RP_LOGP );
	unguard;
}*/

// ============================================================================
//  EOF
// ============================================================================