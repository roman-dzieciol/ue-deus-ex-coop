// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	uppTypes.h
// ============================================================================
#ifndef UPPTYPES_H
#define UPPTYPES_H

#include "upArchive.h"

class uppPkg;


// ============================================================================
// uppArray
// ============================================================================
template<class CT, class IT>
class uppArray : public vector<IT>
{
public:
	void Serialize( upArchive& A, uppPkg& P )
	{
		CT count = static_cast<CT>(static_cast<int>(size()));
		A << count;
		if( A.IsLoading() )
		{
			reserve(count);
			resize(count);
		}
		for( iterator it=begin(); it!=end(); ++it )
			(*it).Serialize(A,P);
	}

	friend void operator << ( upArchive& A, uppArray& D )
	{
		CT count = static_cast<CT>(static_cast<int>(D.size()));
		A << count;
		if( A.IsLoading() )
		{
			D.reserve(count);
			D.resize(count);
		}
		for( iterator it=D.begin(); it!=D.end(); ++it )
			A << (*it);
	}
};


// ============================================================================
// uppIndex -- int stored with minimal amount of bytes
// ============================================================================
// byte 0:
//   0x80 = sign
//   0x40 = next byte marker
//   0x3F = value
// bytes 1-4:
//   0x80 = next byte marker
//   0x7F = value
// ============================================================================
class uppIndex
{
protected:
	int32	Value;

public:
	uppIndex(){}
	explicit uppIndex( int32 d ) : Value(d){}

public:
	uppIndex& operator =	( int32 d )	{ Value = d; return *this; }
	operator int32			() const	{ return Value; }
	int32	GetValue		() const	{ return Value; }

public:
	friend bool operator == ( const uppIndex& left, const uppIndex& right ) { return left.Value == right.Value; }
	friend bool operator == ( const uppIndex& left, int right ) { return left.Value == right; }

public:
	friend void operator << ( upArchive& A, uppIndex& D )
	{
       	int32 Original = D.Value;
		dword V = labs(D.Value);
       	byte B0 = ((D.Value>=0) ? 0 : 0x80) + ((V < 0x40) ? V : ((V & 0x3f)+0x40));
        D.Value = 0;
        A << B0;
        if( B0 & 0x40 )
        {
            V >>= 6;
            byte B1 = (V < 0x80) ? V : ((V & 0x7f)+0x80);
            A << B1;
            if( B1 & 0x80 )
            {
                V >>= 7;
                byte B2 = (V < 0x80) ? V : ((V & 0x7f)+0x80);
                A << B2;
                if( B2 & 0x80 )
                {
                    V >>= 7;
                    byte B3 = (V < 0x80) ? V : ((V & 0x7f)+0x80);
                    A << B3;
                    if( B3 & 0x80 )
                    {
                        V >>= 7;
                        byte B4 = V;
                        A << B4;
                        D.Value = B4;
                    }
                    D.Value = (D.Value << 7) + (B3 & 0x7f);
                }
                D.Value = (D.Value << 7) + (B2 & 0x7f);
            }
            D.Value = (D.Value << 7) + (B1 & 0x7f);
        }
        D.Value = (D.Value << 6) + (B0 & 0x3f);

        if( B0 & 0x80 )
            D.Value = -D.Value;

        if( !A.IsLoading() && D.Value!=Original )
			throw upexception( wxString::Format(wxT("Mismatch: %08X %08X"),D.Value,Original) );
	}
}; 


// ============================================================================
// uppDeltaIndexA -- special delta index
// ============================================================================
// byte 0:
//   0x80 = bDelta
//   0x40 = next byte marker
//   if delta
//     0x20 = delta sign
//     0x1F = value
//   else
//     0x3F = value
// bytes 1-4:
//   0x80 = next byte marker
//   0x7F = value
// ============================================================================
class uppDeltaIndexA
{
protected:
	int32	Value;
	bool	bIsDelta;

public:
	uppDeltaIndexA() : bIsDelta(false)
	{
	}

	explicit uppDeltaIndexA( int32 d ) : Value(d), bIsDelta(false)
	{
		if( !bIsDelta && Value < 0 )
			throw upexception( wxT("uppDeltaIndex: !bIsDelta && Value < 0") );
	}

	explicit uppDeltaIndexA( int32 d, bool bisdelta ) : Value(d), bIsDelta(bisdelta)
	{
		if( !bIsDelta && Value < 0 )
			throw upexception( wxT("uppDeltaIndex: !bIsDelta && Value < 0") );
	}

public:
	uppDeltaIndexA& operator = ( int32 d )	
	{ 
		Value = d; 
		if( !bIsDelta && Value < 0 )
			throw upexception( wxT("uppDeltaIndex: !bIsDelta && Value < 0") );
		return *this; 
	}
	operator int32			() const	{ return Value; }
	int32	GetValue		() const	{ return Value; }
	
public:
	void SetDelta( bool b ) { bIsDelta = b; }
	bool IsDelta() const { return bIsDelta; }

public:
	friend bool operator == ( const uppDeltaIndexA& left, const uppDeltaIndexA& right ) { return left.Value == right.Value; }
	friend bool operator == ( const uppDeltaIndexA& left, int right ) { return left.Value == right; }

public:
	friend void operator << ( upArchive& A, uppDeltaIndexA& D )
	{
		if( A.IsLoading() ) 
		{
			D.Value = 0;
       		byte B0;
			A << B0;
			if( B0 & 0x80 )
			{
				D.bIsDelta = true;

				if( B0 & 0x40 ) {
					byte B1;
					A << B1;
					if( B1 & 0x80 ) {
						byte B2;
						A << B2;
						if( B2 & 0x80 ) {
							byte B3;
							A << B3;
							if( B3 & 0x80 ) {
								byte B4;
								A << B4;
								D.Value = B4;
							} D.Value = (D.Value << 7) + (B3 & 0x7f);
						} D.Value = (D.Value << 7) + (B2 & 0x7f);
					} D.Value = (D.Value << 7) + (B1 & 0x7f);
				} D.Value = (D.Value << 5) + (B0 & 0x1f);
				
				if( B0 & 0x20 )
					D.Value = -D.Value;
			}
			else
			{
				D.bIsDelta = false;

				if( B0 & 0x40 ) {
					byte B1;
					A << B1;
					if( B1 & 0x80 ) {
						byte B2;
						A << B2;
						if( B2 & 0x80 ) {
							byte B3;
							A << B3;
							if( B3 & 0x80 ) {
								byte B4;
								A << B4;
								D.Value = B4;
							} D.Value = (D.Value << 7) + (B3 & 0x7f);
						} D.Value = (D.Value << 7) + (B2 & 0x7f);
					} D.Value = (D.Value << 7) + (B1 & 0x7f);
				} D.Value = (D.Value << 6) + (B0 & 0x3f);
			}
		}
		else
		{
			if( D.bIsDelta )
			{
				dword V = labs(D.Value);
       			byte B0 = 0x80 + ((D.Value>=0) ? 0 : 0x20) + ((V < 0x20) ? V : ((V & 0x1f)+0x40));
				A << B0;
				if( B0 & 0x40 )
				{
					V >>= 5;
					byte B1 = (V < 0x80) ? V : ((V & 0x7f)+0x80);
					A << B1;
					if( B1 & 0x80 )
					{
						V >>= 7;
						byte B2 = (V < 0x80) ? V : ((V & 0x7f)+0x80);
						A << B2;
						if( B2 & 0x80 )
						{
							V >>= 7;
							byte B3 = (V < 0x80) ? V : ((V & 0x7f)+0x80);
							A << B3;
							if( B3 & 0x80 )
							{
								V >>= 7;
								byte B4 = V;
								A << B4;
							}
						}
					}
				}
			}
			else
			{
				if( D.Value < 0 )
					throw upexception( wxT("uppDeltaIndex: !bIsDelta && Value < 0") );

				dword V = labs(D.Value);
       			byte B0 = ((D.Value>=0) ? 0 : 0x80) + ((V < 0x40) ? V : ((V & 0x3f)+0x40));
				A << B0;
				if( B0 & 0x40 )
				{
					V >>= 6;
					byte B1 = (V < 0x80) ? V : ((V & 0x7f)+0x80);
					A << B1;
					if( B1 & 0x80 )
					{
						V >>= 7;
						byte B2 = (V < 0x80) ? V : ((V & 0x7f)+0x80);
						A << B2;
						if( B2 & 0x80 )
						{
							V >>= 7;
							byte B3 = (V < 0x80) ? V : ((V & 0x7f)+0x80);
							A << B3;
							if( B3 & 0x80 )
							{
								V >>= 7;
								byte B4 = V;
								A << B4;
							}
						}
					}
				}
			}
		}
	}
}; 


// ============================================================================
// uppDeltaIndexB -- special delta index
// ============================================================================
class uppDeltaIndexB
{
protected:
	int32	Value;
	bool	bIsDelta;

public:
	uppDeltaIndexB() : bIsDelta(false)
	{
	}

	explicit uppDeltaIndexB( int32 d ) : Value(d), bIsDelta(false)
	{
		if( !bIsDelta && Value < 0 )
			throw upexception( wxT("uppDeltaIndex: !bIsDelta && Value < 0") );
	}

	explicit uppDeltaIndexB( int32 d, bool bisdelta ) : Value(d), bIsDelta(bisdelta)
	{
		if( !bIsDelta && Value < 0 )
			throw upexception( wxT("uppDeltaIndex: !bIsDelta && Value < 0") );
	}

public:
	uppDeltaIndexB& operator = ( int32 d )	
	{ 
		Value = d; 
		if( !bIsDelta && Value < 0 )
			throw upexception( wxT("uppDeltaIndex: !bIsDelta && Value < 0") );
		return *this; 
	}
	operator int32			() const	{ return Value; }
	int32	GetValue		() const	{ return Value; }
	
public:
	bool IsDelta() const { return Value < 0; }

public:
	friend bool operator == ( const uppDeltaIndexB& left, const uppDeltaIndexB& right ) { return left.Value == right.Value; }
	friend bool operator == ( const uppDeltaIndexB& left, int right ) { return left.Value == right; }

public:
	friend void operator << ( upArchive& A, uppDeltaIndexB& D )
	{
		if( A.IsLoading() ) 
		{
			D.Value = 0;
       		byte B0;
			A << B0;

			if( B0 & 0x40 ) {
				byte B1;
				A << B1;
				if( B1 & 0x80 ) {
					byte B2;
					A << B2;
					if( B2 & 0x80 ) {
						byte B3;
						A << B3;
						if( B3 & 0x80 ) {
							byte B4;
							A << B4;
							D.Value = B4;
						} D.Value = (D.Value << 7) + (B3 & 0x7f);
					} D.Value = (D.Value << 7) + (B2 & 0x7f);
				} D.Value = (D.Value << 7) + (B1 & 0x7f);
			} D.Value = (D.Value << 6) + (B0 & 0x3f);

			if( B0 & 0x80 )
				D.Value = -D.Value;
		}
		else
		{
			dword V = labs(D.Value);
   			byte B0 = ((D.Value>=0) ? 0 : 0x80) + ((V < 0x40) ? V : ((V & 0x3f)+0x40));
			A << B0;
			if( B0 & 0x40 )
			{
				V >>= 6;
				byte B1 = (V < 0x80) ? V : ((V & 0x7f)+0x80);
				A << B1;
				if( B1 & 0x80 )
				{
					V >>= 7;
					byte B2 = (V < 0x80) ? V : ((V & 0x7f)+0x80);
					A << B2;
					if( B2 & 0x80 )
					{
						V >>= 7;
						byte B3 = (V < 0x80) ? V : ((V & 0x7f)+0x80);
						A << B3;
						if( B3 & 0x80 )
						{
							V >>= 7;
							byte B4 = V;
							A << B4;
						}
					}
				}
			}
		}
	}
}; 


// ============================================================================
// uppDeltaIndex
// ============================================================================
typedef uppDeltaIndexA uppDeltaIndex;


// ============================================================================
// uppLASCIIZ
// ============================================================================
class uppLASCIIZ
{
protected:
	uppIndex Index;
	wxString Data;

public:
	uppLASCIIZ(){}
	explicit uppLASCIIZ( int i ) : Index(i){}
	explicit uppLASCIIZ( uppIndex i ) : Index(i){}
	explicit uppLASCIIZ( const wxString& s ) : Data(s)
	{
		Index = static_cast<int>(Data.Length()) + 1; // Zero
	}

	wxString str() const { return Data; }
	int Length() const { return Index.GetValue(); }
	
	uppIndex GetIndex() const { return Index; }

	friend bool operator == ( const uppLASCIIZ& left, const uppLASCIIZ& right ) { return left.Data.IsSameAs(right.Data,true); }
	friend bool operator != ( const uppLASCIIZ& left, const uppLASCIIZ& right ) { return !left.Data.IsSameAs(right.Data,true); }

	friend void operator << ( upArchive& A, uppLASCIIZ& D )
	{
		if( A.IsLoading() )
		{
			A << D.Index;
			if( D.Index > 0 )
			{
				wxCharBuffer buffer(D.Index);
				A.Serialize( buffer.data(), D.Index );
				wxWCharBuffer wbuffer = wxConvLocal.cMB2WC(buffer);
				D.Data = wxString(wbuffer);
			}
			else
			{
				D.Data.Clear();
			}
		}
		else
		{
			A << D.Index;
			if( D.Index > 0 )
			{
				//wxWCharBuffer wbuffer = D.Data.wc_str();
				//wxCharBuffer buffer = wxConvLocal.cWC2MB(wbuffer);
				wxCharBuffer buffer = D.Data.mb_str();
				A.Serialize( buffer.data(), D.Index );
				//A << static_cast<byte>(0); // Zero
			}
		}
	}
};




// ============================================================================
// uppGUID
// ============================================================================
class uppGUID
{
public:
	dword	A;
	dword	B;
	dword	C;
	dword	D;

public:
	friend void operator << ( upArchive& A, uppGUID& D )
	{
		A << D.A;
		A << D.B;
		A << D.C;
		A << D.D;
	}

	wxString str() { return wxString::Format(wxT("%X-%X-%X-%X"),A,B,C,D); }
};


// ============================================================================
// uppBitVector -- bit vector, serializes 7 bool values per byte
// ============================================================================
class uppBitVector : public vector<int>
{
public:
	uppBitVector()
	{
		reserve(32);
	}

	~uppBitVector()
	{
	}

	void push_back( bool b )
	{
		vector<int>::push_back( static_cast<int>(b) );
	}

	friend void operator << ( upArchive& A, uppBitVector& D )
	{
		if( A.IsLoading() )
		{
			D.clear();
			byte b;

			// NOTICE: loaded size is always a multiply of 7
			do	{
				A << b;
				D.push_back((b & 0x01) != 0);
				D.push_back((b & 0x02) != 0);
				D.push_back((b & 0x04) != 0);
				D.push_back((b & 0x08) != 0);
				D.push_back((b & 0x10) != 0);
				D.push_back((b & 0x20) != 0);
				D.push_back((b & 0x40) != 0);
			} while( b & 0x80 );
		}
		else
		{
			//wxString s;
			byte b = 0;
			dword ptr = 0x01;

			for( vector<int>::iterator it=D.begin(); it!=D.end(); ++it )
			{
				// last bit is reserved for 'read next byte' marker
				if( ptr == 0x80 )
				{
					// set bit, save, reset
					b |= ptr;
					A << b;
					//s += wxString::Format( wxT("%x"), b );
					b = 0;	
					ptr = 0x01;
				}

				// set bit
				if( (*it) == 1 )
					b |= ptr;

				// increment bit pointer
				ptr <<= 1;
			}

			// save byte
			A << b;		

			//s += wxString::Format( wxT("%x"), b );
			//wxLogMessage( wxT("BITS: %s"), s.c_str() );
		}
	}
}; 


#endif UPPTYPES_H
// ============================================================================
//	The End.
// ============================================================================