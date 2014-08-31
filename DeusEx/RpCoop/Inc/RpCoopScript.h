// ============================================================================
//	RpCoopScript.h
//  Copyright 2007 Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	C++ class definitions exported from UnrealScript.
//	Update manually.
// ============================================================================
#if _MSC_VER
#pragma pack (push,4)
#endif

#ifndef RPCOOP_API
#define RPCOOP_API DLL_IMPORT
#endif


#ifndef NAMES_ONLY
#define AUTOGENERATE_NAME(name) extern RPCOOP_API FName RPCOOP_##name;
#define AUTOGENERATE_FUNCTION(cls,idx,name)
#endif


AUTOGENERATE_NAME(RpCoop)


#ifndef NAMES_ONLY


class RPCOOP_API URpCanvas : public UCanvas
{
public:
    DECLARE_CLASS(URpCanvas,UCanvas,0)
    NO_DEFAULT_CONSTRUCTOR(URpCanvas)
	virtual void Update( FSceneNode* Frame );
};

#endif


#ifndef NAMES_ONLY
#undef AUTOGENERATE_NAME
#undef AUTOGENERATE_FUNCTION
#endif NAMES_ONLY

#if _MSC_VER
#pragma pack (pop)
#endif

// ============================================================================
//  EOF
// ============================================================================