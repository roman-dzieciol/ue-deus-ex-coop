// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	uppProperty.cpp
// ============================================================================
#include "upPrec.h"
#include "uppProperty.h"
#include "updPkg.h"

#define UPP_PROPDATA( cname ) \
public: \
	virtual uppPropData* Update( uppProperty* ) \
	{ \
		return this; \
	}


// ============================================================================
// uppByteProperty
// ============================================================================
class uppByteProperty : public uppPropData
{
public:
	byte  Data;

public:
	UPP_PROPDATA( uppByteProperty );

	virtual void Serialize( upArchive& A )
	{
		A << Data;
	}
};


// ============================================================================
// uppIntegerProperty
// ============================================================================
class uppIntegerProperty : public uppPropData
{
public:
	int  Data;

public:
	UPP_PROPDATA( uppByteProperty );

	virtual void Serialize( upArchive& A )
	{
		A << Data;
	}

};


// ============================================================================
// uppBooleanProperty
// ============================================================================
class uppBooleanProperty : public uppPropData
{
	UPP_PROPDATA( uppByteProperty );

};


// ============================================================================
// uppFloatProperty
// ============================================================================
class uppFloatProperty : public uppPropData
{
public:
	float  Data;

public:
	UPP_PROPDATA( uppByteProperty );

	virtual void Serialize( upArchive& A )
	{
		A << Data;
	}

};


// ============================================================================
// uppObjectProperty
// ============================================================================
class uppObjectProperty : public uppPropData
{
public:
	uppIndex  Data;

public:
	UPP_PROPDATA( uppByteProperty );

	virtual void Serialize( upArchive& A )
	{
		A << Data;
	}

};


// ============================================================================
// uppNameProperty
// ============================================================================
class uppNameProperty : public uppPropData
{
public:
	uppIndex  Data;

public:
	UPP_PROPDATA( uppByteProperty );

	virtual void Serialize( upArchive& A )
	{
		A << Data;
	}

};


// ============================================================================
// uppStringProperty
// ============================================================================
class uppStringProperty : public uppPropData
{
	UPP_PROPDATA( uppByteProperty );

};


// ============================================================================
// uppClassProperty
// ============================================================================
class uppClassProperty : public uppPropData
{
	UPP_PROPDATA( uppByteProperty );

};


// ============================================================================
// uppArrayProperty
// ============================================================================
class uppArrayProperty : public uppPropData
{
	UPP_PROPDATA( uppByteProperty );

};


// ============================================================================
// uppStructProperty
// ============================================================================
class uppStructProperty : public uppPropData
{
	UPP_PROPDATA( uppByteProperty );

};


// ============================================================================
// uppVectorProperty
// ============================================================================
class uppVectorProperty : public uppPropData
{
	UPP_PROPDATA( uppByteProperty );

};


// ============================================================================
// uppRotatorProperty
// ============================================================================
class uppRotatorProperty : public uppPropData
{
	UPP_PROPDATA( uppByteProperty );

};


// ============================================================================
// uppStrProperty
// ============================================================================
class uppStrProperty : public uppPropData
{
public:
	uppLASCIIZ Data;

public:
	UPP_PROPDATA( uppByteProperty );

	virtual void Serialize( upArchive& A )
	{
		A << Data;
	}

};


// ============================================================================
// uppMapProperty
// ============================================================================
class uppMapProperty : public uppPropData
{
	UPP_PROPDATA( uppByteProperty );

};


// ============================================================================
// uppFixedArrayProperty
// ============================================================================
class uppFixedArrayProperty : public uppPropData
{
	UPP_PROPDATA( uppByteProperty );

};


// ============================================================================
// uppUnknownProperty
// ============================================================================
class uppUnknownProperty : public uppPropData
{
public:
	uppProperty* Prop;
	vector<byte> Data;

public:
	explicit uppUnknownProperty( uppProperty* prop ) : Prop(prop)
	{
	}

	virtual uppPropData* Update( uppProperty* prop )
	{
		Prop = prop;
		return this;
	}

	virtual void Serialize( upArchive& A )
	{
		const dword size = Prop->Size;
		if( size > 0 )
		{
			if( A.IsLoading() )
			{
				Data.reserve(size);
				Data.resize(size,0);
			}
			A.Serialize( &Data UP_AT(0), size );
		}
	}
};



// ============================================================================
// uppProperty
// ============================================================================
void uppProperty::Serialize( upArchive& A, uppPkg& pkg )
{

	//if( A.IsLoading() )
	//{
		Offset = A.Tell();
		A << NameIndex;

		if( bTerminator )
			return;

		// Stop if "None"
		//Name = pkg.GetNameString(NameIndex);
		const static wxString TerminatorName = wxString(wxT("None"));
		if( TerminatorName.IsSameAs( pkg.GetNameString(NameIndex), false ) )
		{
			bTerminator = true;
			return;
		}

		// Info byte
		A << Info;
		Type = Info & UP_PROP_TYPE;
		SizeType = Info & UP_PROP_SIZE;
		IsArray = (Info & UP_PROP_ARRAY) == UP_PROP_ARRAY;

		// Struct name
		if( Type == 10 )
			A << IndexProperty;

		// Size
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

		// Array index
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

		// Data


		// Data
		switch(Type)
		{
			case 1: // wxT("ByteProperty");
			{
				A << ByteProperty;
				break;
			}
			case 2: // wxT("IntegerProperty");
			{
				A << IntegerProperty;
				break;
			}
			case 3: // wxT("BooleanProperty");
			{
				BooleanProperty = IsArray;
				break;
			}
			case 4: // wxT("FloatProperty");
			{
				A << FloatProperty;
				break;
			}
			case 5: // wxT("ObjectProperty");
			{
				A << IndexProperty;
				break;
			}
			case 6: // wxT("NameProperty");
			{
				A << IndexProperty;
				break;
			}
		//case 7:  wxT("StringProperty");
		//case 8:  wxT("ClassProperty");
		//case 9:  wxT("ArrayProperty");
		//case 10:  wxT("StructProperty");
		//case 11:  wxT("VectorProperty");
		//case 12:  wxT("RotatorProperty");
			case 13: // wxT("StrProperty");
			{
				A << StrProperty;
				break;
			}
		//case 14:  wxT("MapProperty");
		//case 15:  wxT("FixedArrayProperty");
			default:
			{
				if( Size > 0 )
				{
					if( A.IsLoading() )
					{
						UnknownProperty.reserve(Size);
						UnknownProperty.resize(Size,0);
					}
					A.Serialize( &UnknownProperty UP_AT(0), Size );
				}
				break;
			}
		}

		//switch(Type)
		//{
		//	case 1: Data = new uppByteProperty(); break;
		//	case 2: Data = new uppIntegerProperty(); break;
		//	case 3: Data = new uppBooleanProperty(); break;
		//	case 4: Data = new uppFloatProperty(); break;
		//	case 5: Data = new uppObjectProperty(); break;
		//	case 6: Data = new uppNameProperty(); break;
		//	//case 7: Data = new uppStringProperty(); break;
		//	//case 8: Data = new uppClassProperty(); break;
		//	//case 9: Data = new uppArrayProperty(); break;
		//	//case 10: Data = new uppStructProperty(); break;
		//	//case 11: Data = new uppVectorProperty(); break;
		//	//case 12: Data = new uppRotatorProperty(); break;
		//	case 13: Data = new uppStrProperty(); break;
		//	//case 14: Data = new uppMapProperty(); break;
		//	//case 15: Data = new uppFixedArrayProperty(); break;
		//	default: Data = new uppUnknownProperty(this); break;
		//}
		//Data->Serialize(A);
	//}
	//else
	//{
	//	A << NameIndex;
	//	if( bTerminator )
	//		return;

	//	// Info byte
	//	A << Info;

	//	// Struct name
	//	if( Type == 10 )
	//		A << StructIndex;

	//	// Size
	//	SerializeSize(A);

	//	// Array index
	//	SerializeArray(A);

	//	// Data
	//	//Data->Serialize(A);
	//}
}




// ============================================================================
//	uppProperties
// ============================================================================
void uppProperties::Serialize( upArchive& A, uppPkg& pkg )
{
	if( A.IsLoading() )
	{
		// Read first property
		push_back(uppProperty());
		back().Serialize(A,pkg);

		// if not terminator reserve some space and continue
		if( !back().bTerminator )
		{
			reserve(16);
			do	{
				push_back(uppProperty());
				back().Serialize(A,pkg);
			}while( !back().bTerminator );
		}
		
		/*// Map properties
		for( iterator it=begin(); it!=end(); ++it )
		{
			if( (*it).bTerminator )
				continue;

			wxString name = wxString::Format( wxT("%s [%d]"), (*it).Name.MakeUpper().c_str(), (*it).ArrayIndex );
			const upStrDwordMap::Insert_Result ir = Map.insert( upStrDwordMap::value_type( name, it-begin() ) );
			if( ir.second == false )
				throw upexception( wxString::Format( wxT("Property already mapped: %s"), name.c_str() ) );
		}*/
	}
	else
	{
		for( iterator it=begin(); it!=end(); ++it )
		{
			(*it).Serialize(A,pkg);
		}
	}
}


void uppProperties::SerializeDelta( upArchive& A, updPkg& pkg, uppProperties& other, updTran& tran )
{
	Serialize(A,pkg.Target);
	//if( A.IsLoading() )
	//{
	//	// Read first property
	//	push_back(uppProperty());
	//	back().SerializeDelta(A,pkg,*this,other,tran);

	//	// if not terminator reserve some space and continue
	//	if( !back().bTerminator )
	//	{
	//		reserve(16);
	//		do	{
	//			push_back(uppProperty());
	//			back().SerializeDelta(A,pkg,*this,other,tran);
	//		}while( !back().bTerminator );
	//	}
	//}
	//else
	//{
	//	for( iterator it=begin(); it!=end(); ++it )
	//	{
	//		(*it).SerializeDelta(A,pkg,*this,other,tran);
	//	}
	//}
}





// ============================================================================
//	The End.
// ============================================================================