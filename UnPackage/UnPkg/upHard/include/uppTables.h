// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	uppTables.h
// ============================================================================
#ifndef UPPTABLES_H
#define UPPTABLES_H

#include "upArchive.h"
#include "uppTypes.h"

class uppObject;


// ============================================================================
// EObjectFlags
// ============================================================================
enum EObjectFlags
{
	RF_Transactional    = 0x00000001,   // Object is transactional.
	RF_Unreachable		= 0x00000002,	// Object is not reachable on the object graph.
	RF_Public			= 0x00000004,	// Object is visible outside its package.
	RF_TagImp			= 0x00000008,	// Temporary import tag in load/save.
	RF_TagExp			= 0x00000010,	// Temporary export tag in load/save.
	RF_SourceModified   = 0x00000020,   // Modified relative to source files.
	RF_TagGarbage		= 0x00000040,	// Check during garbage collection.
	//
	//
	RF_NeedLoad			= 0x00000200,   // During load, indicates object needs loading.
	RF_HighlightedName  = 0x00000400,	// A hardcoded name which should be syntax-highlighted.
	RF_EliminateObject  = 0x00000400,   // NULL out references to this during garbage collecion.
	RF_InSingularFunc   = 0x00000800,	// In a singular function.
	RF_RemappedName     = 0x00000800,   // Name is remapped.
	RF_Suppress         = 0x00001000,	//warning: Mirrored in UnName.h. Suppressed log name.
	RF_StateChanged     = 0x00001000,   // Object did a state change.
	RF_InEndState       = 0x00002000,   // Within an EndState call.
	RF_Transient        = 0x00004000,	// Don't save object.
	RF_Preloading       = 0x00008000,   // Data is being preloaded from file.
	RF_LoadForClient	= 0x00010000,	// In-file load for client.
	RF_LoadForServer	= 0x00020000,	// In-file load for client.
	RF_LoadForEdit		= 0x00040000,	// In-file load for client.
	RF_Standalone       = 0x00080000,   // Keep object around for editing even if unreferenced.
	RF_NotForClient		= 0x00100000,	// Don't load this object for the game client.
	RF_NotForServer		= 0x00200000,	// Don't load this object for the game server.
	RF_NotForEdit		= 0x00400000,	// Don't load this object for the editor.
	RF_Destroyed        = 0x00800000,	// Object Destroy has already been called.
	RF_NeedPostLoad		= 0x01000000,   // Object needs to be postloaded.
	RF_HasStack         = 0x02000000,	// Has execution stack.
	RF_Native			= 0x04000000,   // Native (UClass only).
	RF_Marked			= 0x08000000,   // Marked (for debugging).
	RF_ErrorShutdown    = 0x10000000,	// ShutdownAfterError called.
	RF_DebugPostLoad    = 0x20000000,   // For debugging Serialize calls.
	RF_DebugSerialize   = 0x40000000,   // For debugging Serialize calls.
	RF_DebugDestroy     = 0x80000000,   // For debugging Destroy calls.
	RF_ContextFlags		= RF_NotForClient | RF_NotForServer | RF_NotForEdit, // All context flags.
	RF_LoadContextFlags	= RF_LoadForClient | RF_LoadForServer | RF_LoadForEdit, // Flags affecting loading.
	RF_Load  			= RF_ContextFlags | RF_LoadContextFlags | RF_Public | RF_Standalone | RF_Native | RF_SourceModified | RF_Transactional | RF_HasStack, // Flags to load from Unrealfiles.
	RF_Keep             = RF_Native | RF_Marked, // Flags to persist across loads.
	RF_ScriptMask		= RF_Transactional | RF_Public | RF_Transient | RF_NotForClient | RF_NotForServer | RF_NotForEdit // Script-accessible flags.
};


// ============================================================================
// uppTableItem
// ============================================================================
class uppTableItem
{
public:
	dword	ItemIndex;
	dword	ItemOffset;
};


// ============================================================================
// uppNameItem
// ============================================================================
class uppNameItem : public uppTableItem
{
public:
	uppLASCIIZ	Name;
	dword		Flags;

public:
	friend void operator << ( upArchive& A, uppNameItem& D )
	{
		A << D.Name;
		A << D.Flags;
	}
};

// ============================================================================
// uppObjectItem
// ============================================================================
class uppObjectItem : public uppTableItem 
{
public:
	uppIndex		Class;
    int32			Package;
    uppIndex		ObjectName;
};


// ============================================================================
// uppExportItem
// ============================================================================
class uppExportItem : public uppObjectItem
{
public:
	uppIndex		Super;
	dword			ObjectFlags;
	uppIndex		SerialSize;
	uppIndex		SerialOffset;
	uppObject*		Object;

public:
	uppExportItem() : Object(NULL) {}
	~uppExportItem();

	friend void operator << ( upArchive& A, uppExportItem& D )
	{
		A << D.Class;
		A << D.Super;
		A << D.Package;
		A << D.ObjectName;
		A << D.ObjectFlags;
		A << D.SerialSize;

		if( D.SerialSize > 0 )	
			A << D.SerialOffset;
		else					
			D.SerialOffset = 0;		
	}
	
	bool IsNull()
	{
		return	(	Class == 0 
				&&	Super == 0 
				&&	Package == 0
				&&	ObjectFlags == 0
				&&	SerialSize == 0 
				&&	SerialOffset == 0 );
	}
};


// ============================================================================
// uppImportItem
// ============================================================================
class uppImportItem : public uppObjectItem
{
public:
	uppIndex		ClassPackage;

public:
	friend void operator << ( upArchive& A, uppImportItem& D )
	{
		A << D.ClassPackage;
		A << D.Class;
		A << D.Package;
		A << D.ObjectName;
		
	}
};


// ============================================================================
// uppGenerationItem
// ============================================================================
class uppGenerationItem : public uppTableItem 
{
public:
	dword	ExportCount;
	dword	NameCount;

public:
	friend void operator << ( upArchive& A, uppGenerationItem& D )
	{
		A << D.ExportCount;
		A << D.NameCount;
		
	}
};


///////////////////////////////////////////////////////////////////////////////


// ============================================================================
// uppTableHeader
// ============================================================================
class uppTableHeader
{
protected:
	dword		Size;
	dword		Count;
	dword		Offset;

public:
	uppTableHeader() : Size(0), Count(0), Offset(0)
	{
	}

	friend void operator << ( upArchive& A, uppTableHeader& D )
	{
		A << D.Count;
		A << D.Offset;
	}

public:
	dword		GetSize() const			{ return Size; }
	dword		GetCount() const		{ return Count; }
	dword		GetOffset() const		{ return Offset; }

};


// ============================================================================
// uppTable
// ============================================================================
template< class T> 
class uppTable : public vector<T>, public uppTableHeader
{
public:
	uppTable()
	{
		reserve(8192);
	}

	void SetSize( size_t s )
	{
		reserve(s);
		resize(s);
	}

	friend void operator << ( upArchive& A, uppTable<T>& D )
	{
		//UP_TIMER(t);

		if( A.IsLoading() )
		{
			A.Seek( D.Offset );
			D.reserve( D.Count );
			D.resize( D.Count );
		}
		else
		{
			D.Offset = A.Tell();
			D.Count = static_cast<dword>(D.size());
		}

		for( iterator it=D.begin(); it!=D.end(); ++it )
		{
			(*it).ItemOffset = static_cast<dword>(A.Tell());
			(*it).ItemIndex = it-D.begin();
			A << (*it);
		}

		D.Size = static_cast<dword>(A.Tell() - D.Offset);
		

		//UP_TIMER_LOGF(t,0);
		
	}
};


// ============================================================================
// uppNameTable
// ============================================================================
class uppNameTable : public uppTable<uppNameItem>
{
public:
};

typedef uppNameTable::iterator		uppNameTableItr;
typedef uppNameTable::const_iterator	uppNameTableCItr;


// ============================================================================
// uppExportTable
// ============================================================================
class uppExportTable : public uppTable<uppExportItem>
{
public:
};

typedef uppExportTable::iterator			uppExportTableItr;
typedef uppExportTable::const_iterator	uppExportTableCItr;


// ============================================================================
// uppImportTable
// ============================================================================
class uppImportTable : public uppTable<uppImportItem>
{
public:
};

typedef uppImportTable::iterator			uppImportTableItr;
typedef uppImportTable::const_iterator	uppImportTableCItr;


// ============================================================================
// uppGenerationTable
// ============================================================================
class uppGenerationTable : public uppTable<uppGenerationItem>
{
public:
	friend void operator << ( upArchive& A, uppGenerationTable& D )
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

typedef uppGenerationTable::iterator			uppGenerationTableItr;
typedef uppGenerationTable::const_iterator	uppGenerationTableCItr;



#endif UPPTABLES_H
// ============================================================================
//	The End.
// ============================================================================