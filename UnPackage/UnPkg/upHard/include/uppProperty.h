// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	uppProperty.h
// ============================================================================
#ifndef UPPPROPERTY_H
#define UPPPROPERTY_H

#include "uppPkg.h"
#include "updTran.h"

class uppObject;
class updPkg;
class uppProperties;
class uppProperty;


// ============================================================================
// Property bits
// ============================================================================
#define UP_PROP_TYPE    0x0f
#define UP_PROP_SIZE    0x70
#define UP_PROP_ARRAY   0x80


// ============================================================================
// uppPropData
// ============================================================================
class uppPropData
{
public:
	virtual ~uppPropData()
	{
	}

	virtual void Serialize( upArchive& )
	{
	}

	virtual uppPropData* Update( uppProperty* p ) = 0;

};


// ============================================================================
// uppProperty
// ============================================================================
class uppProperty
{
public:
	//uppPropData* Data;
	uppIndex NameIndex;
	byte Info;
	byte Type;
	byte SizeType;
	union  {
		byte IsArray;
		byte BooleanProperty;
	};
	dword ArrayIndex;
	bool bTerminator;

	//wxString Name;
	dword Size;
	wxFileOffset Offset;


public:
	// Property value
	union  {
		byte  ByteProperty;
		int32 IntegerProperty;
		float FloatProperty;
	};
	uppIndex IndexProperty;
	uppLASCIIZ StrProperty;
	vector<byte> UnknownProperty;

public:
	uppProperty() 
	: bTerminator(false)
	, ArrayIndex(0)
	{
	}

	/*uppProperty( const uppProperty& p ) 
	: NameIndex(p.NameIndex)
	, StructIndex(p.StructIndex)
	, Info(p.Info)
	, Type(p.Type)
	, SizeType(p.SizeType)
	, IsArray(p.IsArray)
	, ArrayIndex(p.ArrayIndex)
	, bTerminator(p.bTerminator)
	, Name(p.Name)
	, Size(p.Size)
	, Offset(p.Offset)
	{
		if( Data )
			Data->Update(this);
	}*/

	~uppProperty() 
	{
	}

public:
	void Serialize( upArchive& A, uppPkg& pkg );
	//void SerializeDelta( upArchive& A, updPkg& pkg, uppProperties& alltarget, uppProperties& allbasis, updTran& tran );
	//bool IsModified( uppProperty& other, updTran& tran );
		
public:

	void SerializeSize( upArchive& A )
	{
		switch( SizeType ) 
		{
		case	0x00:	Size = 1; break; 	
		case	0x10:	Size = 2; break; 											
		case	0x20:	Size = 4; break; 											
		case	0x30:	Size = 12; break; 										
		case	0x40:	Size = 16; break; 	
		case	0x50:	{byte  psizeb = Size; A << psizeb; Size = psizeb; break;}      
		case	0x60:	{word  psizew = Size; A << psizew; Size = psizew; break;}	      
		case	0x70:	{dword psized = Size; A << psized; Size = psized; break;}
		default:		
			throw upexception( wxString::Format( wxT("Unknown size type: %d"), (dword)SizeType ) );
		}
	}

	void SerializeArray( upArchive& A )
	{
		if( Type != 3 && IsArray == 1 )
		{
			byte b0;

			if		( ArrayIndex <= 0x7F )		b0 = ArrayIndex;
			else if	( ArrayIndex <= 0x3FFF )	b0 =(ArrayIndex >> 8) + 0x80;
			else								b0 =(ArrayIndex >> 24) + 0xC0;

			A << b0;

			if( (b0 & 0x80) == 0 ) // <= 0x7F
			{
				ArrayIndex = static_cast<dword>(b0);
			}
			else if( (b0 & 0xC0) == 0x80 ) // <= 0x3FFF
			{
				byte b1 = ArrayIndex & 0xFF;
				A << b1;
				ArrayIndex = (static_cast<dword>(b0 & 0x7F) << 0x08) 
						   +  static_cast<dword>(b1);
			}
			else
			{
				byte b1 = ArrayIndex >> 16;
				byte b2 = ArrayIndex >> 8;
				byte b3 = ArrayIndex;
				A << b1;
				A << b2;
				A << b3;
				ArrayIndex = (static_cast<dword>(b0 & 0x3F) << 24) 
						   + (static_cast<dword>(b1) << 16) 
						   + (static_cast<dword>(b2) << 8) 
						   +  static_cast<dword>(b3);
			}
		}
		else
		{
			ArrayIndex = 0;
		}
	}

	dword CalcPropertySerialSize( byte sizetype )
	{
		switch( sizetype ) 
		{
		case	0x00:	
		case	0x10:										
		case	0x20:									
		case	0x30:									
		case	0x40:	return 0;	
		case	0x50:	return sizeof(byte);	 
		case	0x60:	return sizeof(word);	      
		case	0x70:	return sizeof(dword);	 
		default:		
			throw upexception( wxString::Format( wxT("Unknown size type: %d"), (dword)sizetype ) );
		}
	}

	wxString GetPropertyName( byte b )
	{
		switch(b)
		{
		case 1: return wxT("ByteProperty");
		case 2: return wxT("IntegerProperty");
		case 3: return wxT("BooleanProperty");
		case 4: return wxT("FloatProperty");
		case 5: return wxT("ObjectProperty");
		case 6: return wxT("NameProperty");
		case 7: return wxT("StringProperty");
		case 8: return wxT("ClassProperty");
		case 9: return wxT("ArrayProperty");
		case 10: return wxT("StructProperty");
		case 11: return wxT("VectorProperty");
		case 12: return wxT("RotatorProperty");
		case 13: return wxT("StrProperty");
		case 14: return wxT("MapProperty");
		case 15: return wxT("FixedArrayProperty");
		default: return wxT("UnknownProperty");
		}
	}
};



// ============================================================================
//	uppProperties
// ============================================================================
class uppProperties : public vector<uppProperty>
{
private:
	upStrDwordMap Map;

public:


	~uppProperties() 
	{
		//for( iterator it=begin(); it!=end(); ++it )
		//{
		//	if( (*it).Data )
		//	{
		//		delete (*it).Data;
		//		(*it).Data = NULL;
		//	}
		//}
	}

	void Serialize( upArchive& A, uppPkg& pkg );
	void SerializeDelta( upArchive& A, updPkg& pkg, uppProperties& other, updTran& tran );
};




#endif UPPPROPERTY_H
// ============================================================================
//	The End.
// ============================================================================