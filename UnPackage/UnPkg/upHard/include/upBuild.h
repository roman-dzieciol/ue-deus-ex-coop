// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	upBuild.h
// ============================================================================
#ifndef UPBUILD_H
#define UPBUILD_H


// vector accessor macro
//#ifdef _DEBUG
//	#define  UP_AT(num)	.at(num)
//#else
	#define  UP_AT(num)	[num]
//#endif


// adds file name and line number to memory leak reports
#ifdef _DEBUG
    #ifdef __WXMSW__
	    #undef WXDEBUG_NEW
	    #define WXDEBUG_NEW new(__FILE__,__LINE__)
	    #include <crtdbg.h>

	    // this define works around a bug with inline declarations of new, see
	    // http://support.microsoft.com/support/kb/articles/Q140/8/58.asp
        #define new  new( _NORMAL_BLOCK, __FILE__, __LINE__)
    #endif
#endif

//
#define STATIC_CREATOR(NAME,BASE) static BASE* Create() { return new NAME(); }
#define UP_CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

#define UP_BREAK _asm int 3;


#endif UPBUILD_H
// ============================================================================
//	The End.
// ============================================================================