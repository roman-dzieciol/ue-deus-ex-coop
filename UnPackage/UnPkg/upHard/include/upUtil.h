// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	upUtil.h
// ============================================================================
#ifndef UPUTIL_H
#define UPUTIL_H


// ============================================================================
// upObjRef
// ============================================================================
//struct upObjRef
//{
//	int32 Data;
//	upObjRef() : Data(0) {}
//	upObjRef( int32 i ) : Data(i) {}
//	upObjRef& operator = ( int32 i ) { Data = i; return *this; }
//	operator int32() {return Data;}
//};


// ============================================================================
// ptrvector - holds pointers to objects, deletes objects in dtor
// ============================================================================
template <class T>
class ptrvector : public vector<T>
{
public:
    ~ptrvector()
    {
        for( iterator it=begin(); it!=end(); ++it )
            delete static_cast<value_type>(*it);
    }

    value_type push_existing( value_type p )
    {
        if( p != NULL )
            push_back(p);
        return p;
    }
};


// ============================================================================
// namedvector - vector with a name
// ============================================================================
template <class T>
class namedvector : public vector<T>
{
public:
    // Accessors
    wxString GetName() const { return Name; }

    // Mutators
    void SetName( const wxString& d ) { Name = d; }

protected:
    wxString Name;
};


// ============================================================================
// Conversion
// ============================================================================
inline dword upStringToDword( const wxString& text )
{
    unsigned long temp;
    if( !text.ToULong(&temp,0) )
        temp = 0;
    return static_cast<dword>(temp);
}



#endif UPUTIL_H
// ============================================================================
//	The End.
// ============================================================================