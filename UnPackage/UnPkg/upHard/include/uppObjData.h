// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	uppObjData.h
// ============================================================================
#ifndef UPPOBJDATA_H
#define UPPOBJDATA_H

#include "uppPkg.h"
#include "updTran.h"

#pragma warning( push )
#pragma warning( disable : 4355 )

class uppObject;


// ============================================================================
// uppObjData
// ============================================================================
class uppObjData
{
public:
	uppObject* Object;

public:
	uppObjData() : Object(0)
	{
	}

	virtual ~uppObjData()
	{
	}

public:
	void SetObject( uppObject* object )
	{
		Object = object;
	}

	
public:
	virtual void Serialize( upArchive& A ) = 0;
	virtual void SerializeDelta( upArchive& A, uppObjData& base, updTran& tran ) = 0;
	virtual bool IsModified( uppObjData& other, updTran& tran ) = 0;
	virtual uppObjData* GetTranslated( updTran& tran ) = 0;
};


// ============================================================================
// uppObjDataFactory
// ============================================================================
class uppObjDataFactory
{
private:
    typedef uppObjData* (*uppObjDataCreator)();
    WX_DECLARE_STRING_HASH_MAP( uppObjDataCreator, upStrObjDataMap );
    upStrObjDataMap Map;

public:
    static uppObjDataFactory& Global();
    #define GObjDataFactory uppObjDataFactory::Global() 

public:
    uppObjDataFactory();

public:
    void Add( const wxString& name, uppObjDataCreator p );
    uppObjData* Create( const wxString& name );
};


// ============================================================================
// odUnknown
// ============================================================================
class odUnknown : public uppObjData
{
public:
	vector<byte> Data;

public:
	STATIC_CREATOR(odUnknown,uppObjData);

	void SetSize( dword size )
	{
		Data.reserve(size);
		Data.resize(size,0x66);
	}

	virtual void Serialize( upArchive& A )
	{
		A.Serialize(&Data UP_AT(0),(dword)Data.size());
	}
	
	virtual void SerializeDelta( upArchive& A, uppObjData&, updTran& )
	{
		Serialize(A);
	}

	virtual bool IsModified( uppObjData&, updTran& )
	{
		return true;
	}

	virtual uppObjData* GetTranslated( updTran& )
	{
		odUnknown* other = new odUnknown(*this);
		return other;
	}
};

#pragma warning( pop )

#endif UPPOBJDATA_H
// ============================================================================
//	The End.
// ============================================================================