// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	updObject.h
// ============================================================================
#ifndef UPDOBJECT_H
#define UPDOBJECT_H

#include "updTables.h"
#include "uppObject.h"
#include "uppProperty.h"
#include "uppObjData.h"


// ============================================================================
// updObject
// ============================================================================
class updObject
{

public:
	updPkg& Pkg;
	updExportItem& Item;
	uppObjData* ObjData;
	byte ObjInfo;

public:
	updObject( updPkg& pkg, updExportItem& item );
	~updObject();
	
public:
	void Serialize( upArchive& A, uppObject* basis, uppObject& target, updTran& tran );

};



#endif UPDOBJECT_H
// ============================================================================
//	The End.
// ============================================================================