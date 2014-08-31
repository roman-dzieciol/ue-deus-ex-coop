/*=============================================================================
	RpCoop.h
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/
#pragma once



/*-----------------------------------------------------------------------------
	Libs.
-----------------------------------------------------------------------------*/
#pragma comment(lib, "Core")
#pragma comment(lib, "Engine")
#pragma comment(lib, "Render")
#pragma comment(lib, "IpDrv")
#pragma comment(lib, "Extension")
#pragma comment(lib, "DeusEx")
#pragma comment(lib, "RpAr")


/*-----------------------------------------------------------------------------
	Headers.
-----------------------------------------------------------------------------*/

// Detours
#include <stddef.h>
#include <windows.h>
#include "detours.h"

// Unreal engine
#include "Engine.h"
#include "UnLinker.h"
#include "UnRender.h"
#include "UnNet.h"

// Deus Ex
#include "Extension.h"
#include "DeusEx.h"
#include "DeusExGameEngine.h"

// RpAr
#include "RpAr.h"


/*-----------------------------------------------------------------------------
	RpCoop definitions.
-----------------------------------------------------------------------------*/

// Package implementation.
#if 0
#define IMPLEMENT_PACKAGE_PLATFORM(pkgname) \
	extern "C" {HINSTANCE hInstance;} \
	INT DLL_EXPORT STDCALL DllMain( HINSTANCE hInInstance, DWORD Reason, void* Reserved ) \
	{ hInstance = hInInstance; return 1; }
#endif

// Macros
#define RPCOOP_API DLL_EXPORT
#define RP_NAME (EName)RPCOOP_RpCoop.GetIndex()
#define RP_SEP FString(TEXT(" | "))
#define rpguard(text) guard(text##())
#define RP_LOG GLog->Logf
#define RP_LOGP *SwTimeStr(), __FUNC_NAME__

// Globals
RPCOOP_API extern struct FRpLineBatcher	GLineBatcher;
RPCOOP_API extern class URpGameEngine*	GRpEngine;
RPCOOP_API extern UBOOL GRpFileOverride;
RPCOOP_API extern FString GRpCacheSrc;
RPCOOP_API extern FString GRpCacheDest;

// Functions
extern void RegisterRpCoopNames();


/*-----------------------------------------------------------------------------
	RpCoop includes
-----------------------------------------------------------------------------*/

// Base
#include "RpCoopScript.h"
#include "RpToStr.h"
#include "RpUtil.h"
#include "RpLineBatcher.h"
#include "RpDetours.h"

// Override
#include "RpFileTransfer.h"
#include "RpSaveGame.h"

// Classes
#include "RpCanvas.h"
#include "RpGameEngine.h"


/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
