// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	upExceptions.h
// ============================================================================
#ifndef UPEXCEPTIONS_H
#define UPEXCEPTIONS_H


// ============================================================================
// upexception
// ============================================================================
class upexception : public std::exception
{
public:
	upexception()
    : exception()
	{
	}

	upexception( const wxString& text )
    : exception( text.mb_str() )
    , Text(text)
	{
	}

	upexception( const upexception& rhs )
    : exception( rhs.Text.mb_str() )
    , Text(rhs.Text)
	{
	}

    upexception& operator=( const upexception& rhs )
	{
		exception::operator=( rhs ); 
		Text = rhs.Text; 
		return *this; 
	}

	virtual ~upexception()
	{
	}

    const wxString& wwhat() const { return Text; }

private:
    wxString Text; 
} ;


// ============================================================================
// guards
// ============================================================================
#define guard	try {
#define unguard } catch( upexception& e ) { upLogError( wxString::Format(wxT("%s"), e.wwhat())); }\
				  catch( exception& e ) { upLogError( wxString::Format(wxT("%hs"), e.what())); }



#endif UPEXCEPTIONS_H
// ============================================================================
//	The End.
// ============================================================================