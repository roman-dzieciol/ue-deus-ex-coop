// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	updTables.cpp
// ============================================================================
#include "upPrec.h"
#include "updTables.h"
#include "updObject.h"



// ============================================================================
//	updExportItem
// ============================================================================
updExportItem::~updExportItem()
{
	if( Object )
		delete Object;
}




// ============================================================================
//	The End.
// ============================================================================