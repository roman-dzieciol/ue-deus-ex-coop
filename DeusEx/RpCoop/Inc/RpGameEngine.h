/*=============================================================================
	RpGameEngine.h
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/
#pragma once


/*-----------------------------------------------------------------------------
	URpGameEngine.
-----------------------------------------------------------------------------*/

class RPCOOP_API URpGameEngine : public DDeusExGameEngine
{
	DECLARE_CLASS(URpGameEngine, DDeusExGameEngine, CLASS_Config|CLASS_Transient)

public:

	//
	// URpGameEngine
	//
	URpGameEngine();
	int RpLoadListenMap( const FURL& URL, UPendingLevel* Pending, const TMap<FString,FString>* TravelInfo, FString& Error );
	//void CoopTravel(const TCHAR* Cmd);
	//void SAVE(int DirectoryIndex = -2, bool bSavePlayer = FALSE);


	//
	// UObject interface.
	//
	//virtual void ProcessEvent( UFunction* Function, void* Parms, void* Result=NULL );
	//virtual void ProcessState( FLOAT DeltaSeconds );
	//virtual UBOOL ProcessRemoteFunction( UFunction* Function, void* Parms, FFrame* Stack );
	//virtual void Modify();
	//virtual void PostLoad();
	virtual void Destroy();
	//virtual void Serialize( FArchive& Ar );
	//virtual UBOOL IsPendingKill() {return 0;}
	//virtual EGotoState GotoState( FName State );
	//virtual INT GotoLabel( FName Label );
	//virtual void InitExecution();
	//virtual void ShutdownAfterError();
	//virtual void PostEditChange();
	//virtual void CallFunction( FFrame& TheStack, RESULT_DECL, UFunction* Function );
	//virtual UBOOL ScriptConsoleExec( const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor );
	//virtual void Register();
	//virtual void LanguageChange();


	//
	// UEngine interface.
	//
	virtual void Init(void);
	virtual void Exit();
	virtual UBOOL Exec( const TCHAR* Cmd, FOutputDevice& Out=*GLog );
	//virtual void Flush( UBOOL AllowPrecache );
	//virtual UBOOL Key( UViewport* Viewport, EInputKey Key );
	//virtual UBOOL InputEvent( UViewport* Viewport, EInputKey iKey, EInputAction State, FLOAT Delta=0.0 );
	//virtual void Tick( FLOAT DeltaSeconds )=0;
	//virtual void Draw( UViewport* Viewport, UBOOL Blit=1, BYTE* HitData=NULL, INT* HitSize=NULL )=0;
	//virtual void MouseDelta( UViewport* Viewport, DWORD Buttons, FLOAT DX, FLOAT DY )=0;
	//virtual void MousePosition( UViewport* Viewport, DWORD Buttons, FLOAT X, FLOAT Y )=0;
	//virtual void Click( UViewport* Viewport, DWORD Buttons, FLOAT X, FLOAT Y )=0;
	virtual void SetClientTravel( UPlayer* Viewport, const TCHAR* NextURL, UBOOL bItems, ETravelType TravelType );
	virtual INT ChallengeResponse( INT Challenge );
	//virtual FLOAT GetMaxTickRate();
	//virtual void SetProgress( const TCHAR* Str1, const TCHAR* Str2, FLOAT Seconds );
	//void InitAudio();


	//
	// UGameEngine interface.
	//
	virtual UBOOL Browse( FURL URL, const TMap<FString,FString>* TravelInfo, FString& Error );
	virtual ULevel* LoadMap( const FURL& URL, UPendingLevel* Pending, const TMap<FString,FString>* TravelInfo, FString& Error );
	virtual void SaveGame( INT Position );
	virtual void CancelPending();
	//virtual void PaintProgress();
	//virtual void UpdateConnectingMessage();
	virtual void BuildServerMasterMap( UNetDriver* NetDriver, ULevel* InLevel );
	virtual void NotifyLevelChange();
};



	/*static UBOOL SavePackageDE( UObject* InOuter, UObject* Base, DWORD TopLevelFlags, const TCHAR* Filename, FOutputDevice* Error=GError, ULinkerLoad* Conform=NULL )
	{
		FString S;

		if( InOuter )	S += FString::Printf(TEXT(" Outer:[%s]"), InOuter->GetFullName());
		if( Base )		S += FString::Printf(TEXT(" Base:[%s]"), Base->GetFullName());
						S += FString::Printf(TEXT(" Flags:[%d]"), TopLevelFlags);
		if( Filename )	S += FString::Printf(TEXT(" File:[%s]"), Filename);
		if( Error )		S += FString::Printf(TEXT(" Error:[%p]"), Error);
		if( Conform )	S += FString::Printf(TEXT(" Conform:[%s]"), *Conform->Filename);

		GLog->Logf( RP_NAME, TEXT("UObject::SavePackage() %s"), *S );

		return SavePackageTR(InOuter,Base,TopLevelFlags,Filename,Error,Conform);
	}
	
	static UObject* LoadPackageDE( UObject* InOuter, const TCHAR* Filename, DWORD LoadFlags )
	{
		FString S;

		if( InOuter )	S += FString::Printf(TEXT(" Outer:[%s]"), InOuter->GetFullName());
		if( Filename )	S += FString::Printf(TEXT(" File:[%s]"), Filename);
						S += FString::Printf(TEXT(" Flags:[%d]"), LoadFlags);

		GLog->Logf( RP_NAME, TEXT("UObject::LoadPackage() %s"), *S );

		return LoadPackageTR(InOuter,Filename,LoadFlags);
	}*/

	

	/*static void BeginLoadDE()
	{
		GLog->Logf( RP_NAME, TEXT("UObject::BeginLoad()") );
		UObject::BeginLoad();
	}*/

	/*static void EndLoadDE()
	{
		GLog->Logf( RP_NAME, TEXT("UObject::EndLoad()") );
		UObject::EndLoad();
	}*/


/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
