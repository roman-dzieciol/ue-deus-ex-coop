// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	uppObjData.cpp
// ============================================================================
#include "upPrec.h"
#include "uppObjData.h"
#include "uppPkg.h"
#include "uppStructs.h"
#include "uppObject.h"


#pragma warning ( push )
#pragma warning( disable : 4355 )


// ============================================================================
// uptObjData
// ============================================================================
template<class T>
class uptObjData : public uppObjData
{
public:
	T Data;

public:
	STATIC_CREATOR(uptObjData<T>,uppObjData);

public:
	uptObjData()
	{
	}

	uptObjData( const uptObjData& base )
	: uppObjData(base)
	, Data(base.Data)
	{
	}

	virtual void Serialize( upArchive& A )
	{
		Data.Serialize(A);
	}

	virtual void SerializeDelta( upArchive& A, uppObjData& other, updTran& tran )
	{
		T& data = static_cast<uptObjData<T>&>(other).Data;
		Data.SerializeDelta(A,data,tran);
	}

	virtual bool IsModified( uppObjData& other, updTran& tran )
	{
		uptObjData<T>& cother = static_cast<uptObjData<T>&>(other);
		return Data.IsModified(cother.Data,tran);
	}

	virtual uppObjData* GetTranslated( updTran& tran )
	{
		uptObjData<T>* other = new uptObjData<T>(*this);
		other->Data.Translate(tran);
		return other;
	}
};



// ============================================================================
// uppObjDataFactory - Static
// ============================================================================
uppObjDataFactory& uppObjDataFactory::Global()
{
	// Create on first use
    static uppObjDataFactory p;
    return p;
}


// ============================================================================
// uppObjDataFactory
// ============================================================================
uppObjDataFactory::uppObjDataFactory()
{
	Add( wxT("Polys"), &uptObjData<upsPolys>::Create );
	Add( wxT("Model"), &uptObjData<upsModel>::Create );
	//Add( wxT("EventManager"), &uptObjData<upsEventManager>::Create );
	//Add( wxT("AIEventType"), &uptObjData<upsAIEventType>::Create );
	//Add( wxT("AISenderEvent"), &uptObjData<upsAISenderEvent>::Create );
	//Add( wxT("AIReceiverEvent"), &uptObjData<upsAIReceiverEvent>::Create );
	Add( wxT("Level"), &uptObjData<upsLevel>::Create );
	//Add( wxT("Palette"), &uptObjData<upsPalette>::Create );
	//Add( wxT("TextBuffer"), &uptObjData<upsTextBuffer>::Create );
	Add( wxT("Unknown"), &odUnknown::Create );
	
}

void uppObjDataFactory::Add( const wxString& name, uppObjDataCreator p )
{
    Map[name] = p;
}

uppObjData* uppObjDataFactory::Create( const wxString& name )
{
    upStrObjDataMap::iterator it = Map.find(name);
    if( it != Map.end() )
        return ((*it).second)();

	return 0;
	//throw upexception(wxString::Format( wxT("Unknown uppObjData requested: <%s>"), name.c_str() ));
}


#pragma warning ( pop )

// ============================================================================
//	The End.
// ============================================================================