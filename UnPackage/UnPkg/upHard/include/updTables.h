// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	updTables.h
// ============================================================================
#ifndef UPDTABLES_H
#define UPDTABLES_H

#include "updTypes.h"
#include "uppTables.h"

class updObject;



// ============================================================================
// updNameItem
// ============================================================================
class updNameItem : public uppTableItem
{
public:
	uppLASCIIZ	Name;
	dword		Flags;

public:
	friend void operator << ( upArchive& A, updNameItem& D )
	{
		A << D.Name;
		if( D.Name.Length() >= 0 )
			A << D.Flags;
	}
};


// ============================================================================
// updExportItem
// ============================================================================
class updExportItem : public uppTableItem
{
public:
    uppIndex		ObjectName;		
	uppIndex		Class;			
	uppIndex		Super;
    uppIndex		Package;
	dword			ObjectFlags;
	uppIndex		SerialSize;
	uppIndex		SerialOffset;
	uppIndex		DeltaSize;
	uppIndex		DeltaOffset;
	updObject*		Object;

public:
	updExportItem() : Object(NULL) {}
	~updExportItem();

	friend void operator << ( upArchive& A, updExportItem& D )
	{
		A << D.ObjectName;
		if( D.ObjectName >= 0 )
		{
			A << D.Class;
			A << D.Super;
			A << D.Package;
			A << D.ObjectFlags;
		}
		
		A << D.SerialSize;
		if( D.SerialSize > 0 )	
			A << D.SerialOffset;
		else					
			D.SerialOffset = 0;
		
		A << D.DeltaSize;
		if( D.DeltaSize > 0 )	
			A << D.DeltaOffset;
		else					
			D.DeltaOffset = 0;
	}
};


// ============================================================================
// updImportItem
// ============================================================================
class updImportItem : public uppTableItem
{
public:
	uppIndex		Class;
    uppIndex		Package;
    uppIndex		ObjectName;
	uppIndex		ClassPackage;

public:
	friend void operator << ( upArchive& A, updImportItem& D )
	{
		A << D.ObjectName;
		if( D.ObjectName >= 0 )
		{
			A << D.ClassPackage;
			A << D.Class;
			A << D.Package;	
		}
	}
};


// ============================================================================
// updGenerationItem
// ============================================================================
class updGenerationItem : public uppGenerationItem 
{
public:
	uppIndex	ExportCount;
	uppIndex	NameCount;

public:
	friend void operator << ( upArchive& A, updGenerationItem& D )
	{
		A << D.ExportCount;
		if( D.ExportCount >= 0 )
			A << D.NameCount;
	}
};


///////////////////////////////////////////////////////////////////////////////


// ============================================================================
// updNameTable
// ============================================================================
class updNameTable : public uppTable<updNameItem>
{
public:
};

typedef updNameTable::iterator			updNameTableItr;
typedef updNameTable::const_iterator	updNameTableCItr;


// ============================================================================
// updExportTable
// ============================================================================
class updExportTable : public uppTable<updExportItem>
{
public:
};

typedef updExportTable::iterator			updExportTableItr;
typedef updExportTable::const_iterator		updExportTableCItr;


// ============================================================================
// updImportTable
// ============================================================================
class updImportTable : public uppTable<updImportItem>
{
public:
};

typedef updImportTable::iterator			updImportTableItr;
typedef updImportTable::const_iterator		updImportTableCItr;


// ============================================================================
// updGenerationTable
// ============================================================================
class updGenerationTable : public uppTable<updGenerationItem>
{
public:
	friend void operator << ( upArchive& A, updGenerationTable& D )
	{
		if( A.IsLoading() )
		{
			A << D.Count;
			D.reserve( D.Count );
			D.resize( D.Count );
		}
		else
		{
			D.Offset = A.Tell();
			D.Count = static_cast<dword>(D.size());
			A << D.Count;
		}

		for( iterator it=D.begin(); it!=D.end(); ++it )
		{
			(*it).ItemOffset = static_cast<dword>(A.Tell());
			(*it).ItemIndex = it-D.begin();
			A << (*it);
		}

		D.Size = static_cast<dword>(A.Tell() - D.Offset);
	}
};

typedef updGenerationTable::iterator			updGenerationTableItr;
typedef updGenerationTable::const_iterator		updGenerationTableCItr;




#endif UPDTABLES_H
// ============================================================================
//	The End.
// ============================================================================