// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	uppStructObjs.h
// ============================================================================
#ifndef UPPSTRUCTOBJS_H
#define UPPSTRUCTOBJS_H

#include "uppPkg.h"
#include "updTran.h"

#pragma warning( push )
#pragma warning( disable : 4355 )


enum EDeltaType
{
	DT_None
,	DT_Delta
};


// ============================================================================
// upsObject
// ============================================================================
struct upsObject
{
	upsObject* Child;	// first class member, during construction last initialized class member
	upsObject* Next;	// pointer to next sibling

	upsObject( upsObject* parent=NULL, upsObject* first=NULL ) : Child(NULL), Next(NULL)
	{
		if( parent )
		{
			if( parent->Child )
				parent->Child->Next = this;
			parent->Child = first ? first : this;
		}
	}

	upsObject( const upsObject&, upsObject* parent=NULL, upsObject* first=NULL ) : Child(NULL), Next(NULL)
	{
		if( parent )
		{
			if( parent->Child )
				parent->Child->Next = this;
			parent->Child = first ? first : this;
		}
	}

	//void Link( upsObject* parent=NULL, upsObject* first=NULL )
	//{
	//	if( parent )
	//	{
	//		if( parent->Child )
	//			parent->Child->Next = this;
	//		parent->Child = first ? first : this;
	//	}
	//}

	//void AddChild( upsObject* child, upsObject*& last )
	//{
	//	if( last )
	//	{
	//		last->Next = child;
	//	}
	//	else
	//	{
	//		Child = child;
	//	}

	//	last = child;
	//}

	virtual ~upsObject()
	{
	}
	
	virtual void Serialize( upArchive& A ) = 0;
	virtual bool IsModified( upsObject& other, updTran& tran ) = 0;
	virtual void Translate( updTran& tran ) = 0;

	//virtual void SerializeDelta( upArchive& A, upsObject& other, updTran& tran )
	virtual void SerializeDelta( upArchive&, upsObject&, updTran& ) = 0;
	virtual void LoadBasis( upsObject&, updTran& ) = 0;

	friend void operator << ( upArchive& A, upsObject& D )
	{
		D.Serialize(A);
	}
};

// constructor initialisation list macros
#define UPS_INIT( var_name )					var_name(this)
#define UPS_INIT_LAST( var_name, first_name )	var_name(this,&first_name)
#define UPS_COPY( var_name )					var_name(base.var_name, this )
#define UPS_COPY_LAST( var_name, first_name )	var_name(base.var_name, this, &first_name)


// ============================================================================
//  upsoContainer :: contains several struct objects
// ============================================================================
class upsoContainer : public upsObject
{
public:
	upsoContainer( upsObject* parent=NULL, upsObject* first=NULL ) 
	: upsObject(parent,first)
	{
	}

	upsoContainer( const upsoContainer& base, upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsObject(base,parent,first)
	{
	}
	
	virtual void Serialize( upArchive& A ) 
	{ 
		for( upsObject* oa=Child; oa!=NULL; oa=oa->Next )
		{
			oa->Serialize(A);
		}
	}

	virtual void SerializeDelta( upArchive& A, upsObject& other, updTran& tran )
	{
		if( Child )
		{
			uppBitVector bits;

			bool bLoading = A.IsLoading();
			if( !bLoading )
			{	
				upsObject* ob = other.Child;
				for( upsObject* oa=Child; oa!=NULL; oa=oa->Next )
				{
					if( ob )
					{
						bits.push_back( oa->IsModified(*ob,tran) );
						ob = ob->Next;
					}
					else
					{
						bits.push_back( true );
					}
				}
			}

			A << bits;

			// Serialize modified members
			int i = -1;
			upsObject* ob = other.Child;
			for( upsObject* oa=Child; oa!=NULL; oa=oa->Next )
			{
				if( bits[++i] )
				{
					oa->SerializeDelta(A,*ob,tran);
				}
				else if( bLoading )
				{
					oa->LoadBasis(*ob,tran);
				}

				ob = ob->Next;
			}
		}
	}

	virtual void LoadBasis( upsObject& other, updTran& tran )
	{
		upsObject* ob = other.Child;
		for( upsObject* oa=Child; oa!=NULL; oa=oa->Next )
		{
			oa->LoadBasis(*ob,tran);
			ob = ob->Next;
		}
	}

	virtual bool IsModified( upsObject& other, updTran& tran )
	{
		upsObject* ob = other.Child;
		for( upsObject* oa=Child; oa!=NULL; oa=oa->Next )
		{
			if( !ob || oa->IsModified(*ob,tran) )
				return true;
			ob = ob->Next;
		}
		return false;
	}

	virtual void Translate( updTran& tran )
	{
		for( upsObject* oa=Child; oa!=NULL; oa=oa->Next )
		{
			oa->Translate(tran);
		}
	}
};


// ============================================================================
//  upstData :: generic data object container
// ============================================================================
template<class T>
class upstData : public upsObject
{
public:
	T Data;

public:
	upstData( upsObject* parent=NULL, upsObject* first=NULL ) 
	: upsObject(parent,first)
	{
	}

	upstData( const upstData<T>& base, upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsObject(base,parent,first)
	,	Data(base.Data)
	{
	}

	T& GetData() 
	{ 
		return Data; 
	}
	
	virtual void Serialize( upArchive& A ) 
	{ 
		A << Data;
	}

	virtual void SerializeDelta( upArchive& A, upsObject&, updTran& )
	{
		Serialize(A);
	}

	virtual void LoadBasis( upsObject& other, updTran& )
	{
		GetData() = static_cast<upstData<T>&>(other).GetData();
	}

	virtual bool IsModified( upsObject& other, updTran& )
	{ 
		return GetData() != static_cast<upstData<T>&>(other).GetData(); 
	}

	virtual void Translate( updTran& )
	{
	}
};


// typedefs
typedef upstData<uppIndex>			upsIndex;
typedef upstData<uppLASCIIZ>		upsLASCIIZ;
typedef upstData<int>				upsInt;
typedef upstData<byte>				upsByte;
typedef upstData<word>				upsWord;
typedef upstData<dword>				upsDword;
typedef upstData<qword>				upsQword;
typedef upstData<float>				upsFloat;


// ============================================================================
//  upstObj :: package object index
// ============================================================================
template<class T>
class upstObj : public T
{
public:
	upstObj( upsObject* parent=NULL, upsObject* first=NULL ) 
	: T(parent,first)
	{
	}

	upstObj( const upstObj& base, upsObject* parent=NULL, upsObject* first=NULL ) 
	:	T(base,parent,first)
	{
	}

	virtual void LoadBasis( upsObject& other, updTran& tran )
	{
		T::LoadBasis(other,tran);
		Translate(tran);
	}

	virtual bool IsModified( upsObject& other, updTran& tran )
	{
		int a = T::GetData();
		int b = static_cast<T&>(other).GetData();

		if( a == 0 )
		{
			return b != 0;
		}
		else
		{
			// modified if no translation or indexes don't match (redundant?)
			return !tran.HasBasisItem(a) || (tran.GetBasisItem(a) != b); 
		}
	}

	virtual void Translate( updTran& tran )
	{
		int a = T::GetData();
		T::GetData() = tran.HasTargetItem(a) ? tran.GetTargetItem(a) : 0;
	}
};

// typedefs
typedef upstObj<upsIndex>	upsObjIndex;
typedef upstObj<upsInt>		upsObjInt;
typedef upstObj<upsByte>	upsObjByte;
typedef upstObj<upsWord>	upsObjWord;
typedef upstObj<upsDword>	upsObjDword;
typedef upstObj<upsQword>	upsObjQword;
typedef upstObj<upsFloat>	upsObjFloat;


// ============================================================================
//  upstName :: name index
// ============================================================================
template<class T>
class upstName : public T
{
public:
	upstName( upsObject* parent=NULL, upsObject* first=NULL ) 
	: T(parent,first)
	{
	}

	upstName( const upstName& base, upsObject* parent=NULL, upsObject* first=NULL ) 
	:	T(base,parent,first)
	{
	}

	virtual void LoadBasis( upsObject& other, updTran& tran )
	{
		T::LoadBasis(other,tran);
		Translate(tran);
	}

	virtual bool IsModified( upsObject& other, updTran& tran )
	{
		int a = T::GetData();
		int b = static_cast<T&>(other).GetData();

		if( a == 0 )
		{
			return b != 0;
		}
		else
		{
			// modified if no translation or indexes don't match (redundant?)
			return !tran.HasBasisName(a) || (tran.GetBasisName(a) != b); 
		}
	}

	virtual void Translate( updTran& tran )
	{
		int a = T::GetData();
		T::GetData() = tran.HasTargetName(a) ? tran.GetTargetName(a) : 0;
	}
};

// typedefs
typedef upstName<upsIndex>	upsNameIndex;
typedef upstName<upsInt>	upsNameInt;
typedef upstName<upsByte>	upsNameByte;
typedef upstName<upsWord>	upsNameWord;
typedef upstName<upsDword>	upsNameDword;
typedef upstName<upsQword>	upsNameQword;
typedef upstName<upsFloat>	upsNameFloat;



// ============================================================================
//  upstArray :: abstract array
// ============================================================================
template<class T>
class upstArray : public upsObject, public vector<T>
{
public:
	upstArray( upsObject* parent=NULL, upsObject* first=NULL ) 
	: upsObject(parent,first)
	{
	}

	upstArray( const upstArray& base, upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upsObject(base,parent,first)
	,	vector<T>(base)
	{
	}
	
	virtual void Serialize( upArchive& A ) 
	{ 
		for( iterator it=begin(); it!=end(); ++it )
		{
			(*it).Serialize(A);
		}
	}

	virtual void SerializeDelta( upArchive& A, upsObject& other, updTran& tran )
	{
		uppBitVector bits;
		upstArray<T>& bother = static_cast<upstArray<T>&>(other);
		
		bool bLoading = A.IsLoading();
		if( !bLoading )
		{	
			iterator bi = bother.begin();
			for( iterator ai=begin(); ai!=end(); ++ai )
			{
				if( bi != bother.end() )
				{
					T& a = *ai;
					T& b = *bi;
					bits.push_back( a.IsModified(b,tran) );
					++bi;
				}
				else
				{
					break; 
				}
			}
		}

		A << bits;
		
		int i = -1;
		iterator ai = begin();
		iterator bi = bother.begin();
		while( ai != end() )
		{
			if( bi != bother.end() )
			{
				T& a = *ai;
				T& b = *bi;
				if( bits[++i] )
				{
					a.SerializeDelta(A,b,tran);
				}
				else if( bLoading )
				{
					a.LoadBasis(b,tran);
				}
				++ai;
				++bi;
			}
			else
			{
				// serialize remaining without delta
				T& a = *ai;
				a.Serialize(A);
				++ai;
			}
		}
	}

	virtual void LoadBasis( upsObject& other, updTran& tran )
	{
		upstArray<T>& bother = static_cast<upstArray<T>&>(other);
		if( bother.size() < size() )
			throw upexception( wxT("sizes don't match") );

		iterator bi = bother.begin();
		for( iterator ai=begin(); ai!=end(); ++ai )
		{
			if( bi != bother.end() )
			{
				T& a = *ai;
				T& b = *bi;
				a.LoadBasis(b,tran);
				++bi;
			}
		}
	}

	virtual bool IsModified( upsObject& other, updTran& tran )
	{
		upstArray<T>& ba = static_cast<upstArray<T>&>(other);
		if( size() != ba.size() )
			return true;

		iterator bi = ba.begin();
		for( iterator ai=begin(); ai!=end(); ++ai )
		{
			T& a = *ai;
			T& b = *bi;
			if( a.IsModified( b, tran )  )
				return true;
			++bi;
		}
		return false;
	}

	virtual void Translate( updTran& tran ) 
	{
		for( iterator ai=begin(); ai!=end(); ++ai )
		{
			(*ai).Translate( tran );
		}
	}

};


// ============================================================================
//  upstComboArray
// ============================================================================
template<class CT, class IT>
class upstComboArray : public upstArray<IT>
{
public:
	upstComboArray( upsObject* parent=NULL, upsObject* first=NULL ) 
	: upstArray<IT>(parent,first)
	{
	}

	upstComboArray( const upstComboArray& base, upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upstArray<IT>(base,parent,first)
	{
	}
	
	virtual void Serialize( upArchive& A ) 
	{ 
		CT count = static_cast<CT>(static_cast<int>(size()));
		A << count;

		if( A.IsLoading() )
		{
			reserve(count);
			resize(count);
		}

		upstArray<IT>::Serialize(A);
	}

	virtual void SerializeDelta( upArchive& A, upsObject& other, updTran& tran )
	{
		CT count = static_cast<CT>(static_cast<int>(size()));
		A << count;

		if( A.IsLoading() )
		{
			reserve(count);
			resize(count);
		}

		upstArray<IT>::SerializeDelta(A,other,tran);
	}

	virtual void LoadBasis( upsObject& other, updTran& tran )
	{
		upstComboArray<CT,IT>& bother = static_cast<upstComboArray<CT,IT>&>(other);
		CT count = static_cast<CT>(static_cast<int>(bother.size()));

		reserve(count);
		resize(count);

		upstArray<IT>::LoadBasis(other,tran);
	}
};

// ============================================================================
//  upstFixedArray
// ============================================================================
template<size_t Count, class IT>
class upstFixedArray : public upstArray<IT>
{
public:
	upstFixedArray( upsObject* parent=NULL, upsObject* first=NULL ) 
	: upstArray<IT>(parent,first)
	{
		reserve(Count);
		resize(Count);
	}

	upstFixedArray( const upstFixedArray& base, upsObject* parent=NULL, upsObject* first=NULL ) 
	:	upstArray<IT>(base,parent,first)
	{
	}
	
	virtual void Serialize( upArchive& A ) 
	{ 
		upstArray<IT>::Serialize(A);
	}

	virtual void SerializeDelta( upArchive& A, upsObject& other, updTran& tran )
	{
		upstArray<IT>::SerializeDelta(A,other,tran);
	}

	virtual void LoadBasis( upsObject& other, updTran& tran )
	{
		upstArray<IT>::LoadBasis(other,tran);
	}
};




#pragma warning( pop )

#endif UPPSTRUCTOBJS_H
// ============================================================================
//	The End.
// ============================================================================