/*=============================================================================
	RpDtMacros.h
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/
#pragma once


/*-----------------------------------------------------------------------------
	Static Function Detours.
-----------------------------------------------------------------------------*/

#define RP_DETOUR_LOAD(name) name##TR = (name##TY) DetourFunction( (PBYTE)name##TG, (PBYTE)name##DE );
#define RP_DETOUR_UNLOAD(name) DetourRemove((PBYTE)name##TR,(PBYTE)name##DE);

#define RP_DETOUR_TRAMPOLINE(name) name##TY name##TR = NULL;
#define RP_DETOUR_TARGET(name,ptr) name##TY name##TG = ptr;
#define RP_DETOUR_POINTERS(name,ptr) \
	RP_DETOUR_TRAMPOLINE(name) \
	RP_DETOUR_TARGET(name,ptr)


#define RP_DETOUR_LOAD_CP(ignore,name) name##TR = (name##TY) DetourFunction( (PBYTE)name##TG, (PBYTE)name##DE );
#define RP_DETOUR_UNLOAD_CP(ignore,name) DetourRemove((PBYTE)name##TR,(PBYTE)name##DE);
#define RP_DETOUR_POINTERS_CP(class,name) \
	RP_DETOUR_TRAMPOLINE(name) \
	RP_DETOUR_TARGET(name,class::name)


#define RP_DETOUR2_LOAD_MF(ignore,name) name##TR = (name##TY) DetourFunction( (PBYTE)name##TG, (PBYTE)name##DE );
#define RP_DETOUR2_UNLOAD_MF(ignore,name) DetourRemove((PBYTE)name##TR,(PBYTE)name##DE);

#define RP_DETOUR2_TRAMPOLINE_MF(name,ptr) name##_TY name##_TR = ptr;
#define RP_DETOUR2_TARGET_MF(name,ptr) name##_TY name##_TG = ptr;

#define RP_DETOUR2_POINTERS_MF(class,name) \
	RP_DETOUR2_TRAMPOLINE_MF(name,class::name) \
	RP_DETOUR2_TARGET_MF(name,class::name)



	


/*-----------------------------------------------------------------------------
	Member Function Detours.
-----------------------------------------------------------------------------*/

// C/P helper
#define RP_DETOUR_TP(name) TP_##name

//
#define RP_DETOUR_CDEF(cname,rval,name,params) \
class CL_##cname##name \
{ \
public: \
	rval DE_##name params; \
	rval TP_##name params; \
};

//
#define RP_DETOUR_CDEF_SUPER(cname,rval,name,params) \
	class CL_##cname##name : public cname \
{ \
public: \
	rval DE_##name params; \
	rval TP_##name params; \
};

//
#define RP_DETOUR_CFUNC(cname,rval,name,params) \
rval CL_##cname##name::DE_##name params \
{ static const TCHAR __FUNC_NAME__[] = L#cname L"::" L#name L"()";

// Detour/Trampoline class
#define RP_DETOUR_CLASS(cname,rval,name,params) \
RP_DETOUR_CDEF(cname,rval,name,params) \
DETOUR_TRAMPOLINE_EMPTY ( rval CL_##cname##name::TP_##name params ); \
RP_DETOUR_CFUNC(cname,rval,name,params)

// Detour/Trampoline class
#define RP_DETOUR_CLASS_SUPER(cname,rval,name,params) \
RP_DETOUR_CDEF_SUPER(cname,rval,name,params) \
DETOUR_TRAMPOLINE_EMPTY ( rval CL_##cname##name::TP_##name params ); \
RP_DETOUR_CFUNC(cname,rval,name,params)




// Detour loader
template<class A, class B> void RpDetour(A target, B detour, B trampoline){
	DetourFunctionWithEmptyTrampoline( *(PBYTE*)&trampoline, *(PBYTE*)&target, *(PBYTE*)&detour);}

// Detour member function
#define RP_DETOUR_MF(target,name) RpDetour( &target::name, &CL_##target##name::DE_##name, &CL_##target##name::TP_##name );


/*-----------------------------------------------------------------------------
	Static Function Detours.
-----------------------------------------------------------------------------*/


// C/P helper
#define RP_DETOUR_STC(name) TP_##name

// Detour/Trampoline class
#define RP_DETOUR_STATIC(cname,rval,name,params) \
class CL_##cname##name \
{ \
public: \
	static rval DE_##name params; \
	static rval TP_##name params; \
}; \
DETOUR_TRAMPOLINE_EMPTY ( rval CL_##cname##name::TP_##name params ); \
rval CL_##cname##name::DE_##name params \
{ rpguard(cname::name);

#define RP_DETOUR_ST(target,name) RpDetour( &target::name, &CL_##target##name::DE_##name, &CL_##target##name::TP_##name );

#define RPDT_FINISH } unguard;


/*-----------------------------------------------------------------------------
	Virtual Function Detours.
-----------------------------------------------------------------------------*/

// Traverse vtable jmp's
inline PBYTE RpVTableDeref( PBYTE pbCode )
{
	if( pbCode[0] == 0xE9 )	pbCode += 5 + *(LONG*)&pbCode[1];
	if( pbCode[0] == 0xE9 )	pbCode += 5 + *(LONG*)&pbCode[1];
	if( pbCode[0] == 0xE9 )	pbCode += 5 + *(LONG*)&pbCode[1];
	if( pbCode[0] == 0xE9 )	pbCode += 5 + *(LONG*)&pbCode[1];
	if( pbCode[0] == 0xE9 )	pbCode += 5 + *(LONG*)&pbCode[1];
	if( pbCode[0] == 0xE9 )	pbCode += 5 + *(LONG*)&pbCode[1];
	if( pbCode[0] == 0xE9 )	pbCode += 5 + *(LONG*)&pbCode[1];
	if( pbCode[0] == 0xE9 )	pbCode += 5 + *(LONG*)&pbCode[1];

	// Too many jumps!
	if( pbCode[0] == 0xE9 )	
		_asm int 3;

	return pbCode;
}

// Get virtual function address 
inline PBYTE RpVTableFunc( PBYTE base, int v_offset, int f_offset )
{
	return RpVTableDeref(*(PBYTE*)(*(PBYTE*)(base + v_offset) + f_offset));
}

// Detour virtual function
#define RP_DETOUR_VR(cptr,vofs,fofs,name) RpDetour( RpVTableFunc(cptr,vofs,fofs), &CL_##cptr##name::DE_##name, &CL_##cptr##name::TP_##name );
	



/*-----------------------------------------------------------------------------
	UObject
-----------------------------------------------------------------------------*/


//typedef UBOOL				(*SavePackageTY)			( UObject*, UObject*, DWORD, const TCHAR*, FOutputDevice*, ULinkerLoad* );
//typedef UObject*			(*LoadPackageTY)			( UObject*, const TCHAR*, DWORD );
//typedef void				(*BeginLoadTY)				(void);
//typedef void				(*EndLoadTY)				(void);

//RP_DETOUR_POINTERS_CP(UObject,SavePackage)
//RP_DETOUR_POINTERS_CP(UObject,LoadPackage)
//RP_DETOUR_POINTERS_CP(UObject,BeginLoad)
//RP_DETOUR_POINTERS_CP(UObject,EndLoad)



/*
	_asm int 3;
	void* x = p;
	_asm int 3;
	_asm int 3;


	GLevel->Tick(LEVELTICK_TimeOnly,0);
	5c
	_asm nop;
	_asm int 3;
	//PBYTE mlvl = (PBYTE)GLevel;			// ULevel data
	PBYTE pvtbl = mlvl + 0x28;			// pointer to FNetworkNotify vtable
	_asm nop;
	PBYTE mvtbl = *(PBYTE*)pvtbl;		// vtable data
	PBYTE pfunc = mvtbl + 0x0C;			// pointer to NotifyGetLevel stub
	_asm nop;
	PBYTE mstub = *(PBYTE*)pfunc;		// NotifyGetLevel stub data
	//PBYTE pfinal = DetourGetFinalCode(mstub,TRUE);
	//PBYTE pfunc = mvtbl + 0x0C;			// pointer to NotifyGetLevel stub
	_asm nop;
	//int* pstub = *(int**)pvtbl + 0x0C;
	_asm nop;
	//GLevel->NotifyGetLevel();
	_asm nop;
*/

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/