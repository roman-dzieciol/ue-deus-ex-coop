/*=============================================================================
	RpDt.h
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/

// Includes.
#include "RpCoop.h"
#include "RpDtMacros.h"


RP_DETOUR_STATIC( UObject, UObject*, StaticConstructObject, ( UClass* Class, UObject* InOuter, FName Name, DWORD SetFlags, UObject* Template, FOutputDevice* Error ) )
{
	//RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP );
	UObject* r = RP_DETOUR_STC( StaticConstructObject(Class,InOuter,Name,SetFlags,Template,Error) );
	if( Cast<USubsystem>(r) || Cast<UClient>(r) )
	{
		RP_LOG( RP_NAME, TEXT("%s ## %s :: [%s]"), RP_LOGP, r->GetFullName() );
		if( Cast<UNetDriver>(r) )
		{
			RpDtUNetDriver(Cast<UNetDriver>(r));
		}
		
	}
	//RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
	return r;
	RPDT_FINISH;
}


RP_DETOUR_STATIC( UObject, void, CollectGarbage, ( DWORD KeepFlags ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%.8x]"), RP_LOGP, KeepFlags );
	RP_DETOUR_STC( CollectGarbage(KeepFlags) );
	RpDumpRefs();
	//RpDumpLevels();
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%.8x]"), RP_LOGP, KeepFlags );
	RPDT_FINISH;
}


RP_DETOUR_STATIC( UObject, UBOOL, IsReferenced, ( UObject*& Res, DWORD KeepFlags, UBOOL IgnoreReference ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s],[%.8x],[%d]"), RP_LOGP, *ToStr(Res),KeepFlags,IgnoreReference );
	UBOOL r = RP_DETOUR_STC( IsReferenced(Res,KeepFlags,IgnoreReference) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s],[%.8x],[%d] [%d]"), RP_LOGP, *ToStr(Res),KeepFlags,IgnoreReference, r );
	return r;
	RPDT_FINISH;
}

	

void RpDtsUObject()
{
	rpguard(RpDtsUObject);
	RP_LOG( RP_NAME, TEXT("%s ## %s :: "), RP_LOGP );
	//RP_DETOUR_MF(UObject,IsReferenced);
	RP_DETOUR_MF(UObject,CollectGarbage);
	RP_DETOUR_MF(UObject,StaticConstructObject);
	unguard;
}


/*

	// Systemwide functions.
	static UObject* StaticFindObject( UClass* Class, UObject* InOuter, const TCHAR* Name, UBOOL ExactClass=0 );
	static UObject* StaticFindObjectChecked( UClass* Class, UObject* InOuter, const TCHAR* Name, UBOOL ExactClass=0 );
	static UObject* StaticLoadObject( UClass* Class, UObject* InOuter, const TCHAR* Name, const TCHAR* Filename, DWORD LoadFlags, UPackageMap* Sandbox );
	static UClass* StaticLoadClass( UClass* BaseClass, UObject* InOuter, const TCHAR* Name, const TCHAR* Filename, DWORD LoadFlags, UPackageMap* Sandbox );
	static UObject* StaticAllocateObject( UClass* Class, UObject* InOuter=(UObject*)GetTransientPackage(), FName Name=NAME_None, DWORD SetFlags=0, UObject* Template=NULL, FOutputDevice* Error=GError, UObject* Ptr=NULL );
	static UObject* StaticConstructObject( UClass* Class, UObject* InOuter=(UObject*)GetTransientPackage(), FName Name=NAME_None, DWORD SetFlags=0, UObject* Template=NULL, FOutputDevice* Error=GError );
	static void StaticInit();
	static void StaticExit();
	static UBOOL StaticExec( const TCHAR* Cmd, FOutputDevice& Ar=*GLog );
	static void StaticTick();
	static UObject* LoadPackage( UObject* InOuter, const TCHAR* Filename, DWORD LoadFlags );
	static UBOOL SavePackage( UObject* InOuter, UObject* Base, DWORD TopLevelFlags, const TCHAR* Filename, FOutputDevice* Error=GError, ULinkerLoad* Conform=NULL );
	static void CollectGarbage( DWORD KeepFlags );
	static void SerializeRootSet( FArchive& Ar, DWORD KeepFlags, DWORD RequiredFlags );
	static UBOOL IsReferenced( UObject*& Res, DWORD KeepFlags, UBOOL IgnoreReference );
	static UBOOL AttemptDelete( UObject*& Res, DWORD KeepFlags, UBOOL IgnoreReference );
	static void BeginLoad();
	static void EndLoad();
	static void InitProperties( BYTE* Data, INT DataCount, UClass* DefaultsClass, BYTE* Defaults, INT DefaultsCount );
	static void ExitProperties( BYTE* Data, UClass* Class );
	static void ResetLoaders( UObject* InOuter, UBOOL DynamicOnly, UBOOL ForceLazyLoad );
	static UPackage* CreatePackage( UObject* InOuter, const TCHAR* PkgName );
	static ULinkerLoad* GetPackageLinker( UObject* InOuter, const TCHAR* Filename, DWORD LoadFlags, UPackageMap* Sandbox, FGuid* CompatibleGuid );
	static void StaticShutdownAfterError();
	static UObject* GetIndexedObject( INT Index );
	static void GlobalSetProperty( const TCHAR* Value, UClass* Class, UProperty* Property, INT Offset, UBOOL Immediate );
	static void ExportProperties( FOutputDevice& Out, UClass* ObjectClass, BYTE* Object, INT Indent, UClass* DiffClass, BYTE* Diff );
	static void ResetConfig( UClass* Class );
	static void GetRegistryObjects( TArray<FRegistryObjectInfo>& Results, UClass* Class, UClass* MetaClass, UBOOL ForceRefresh );
	static void GetPreferences( TArray<FPreferencesInfo>& Results, const TCHAR* Category, UBOOL ForceRefresh );
	static UBOOL GetInitialized();
	static UPackage* GetTransientPackage();
	static void VerifyLinker( ULinkerLoad* Linker );
	static void ProcessRegistrants();
	static void BindPackage( UPackage* Pkg );
	static const TCHAR* GetLanguage();
	static void SetLanguage( const TCHAR* LanguageExt );
  */

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/