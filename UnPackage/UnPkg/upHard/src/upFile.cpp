// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	upFile.cpp
// ============================================================================
#include "upPrec.h"
#include "upFile.h"


// ============================================================================
//	upFile
// ============================================================================
upFile::upFile( const wxFileName& filename ) : FileName(filename)
{
}

upFile::~upFile()
{
    if( File.IsOpened() )
        File.Close();
}

wxString upFile::DebugInfo()
{
	wxString S;
	S += wxString::Format( wxT(" N:<%s>"), FileName.GetName() );
	S += wxString::Format( wxT(" SP:<0x%.8x 0x%.8x>"), static_cast<dword>(Length()), static_cast<dword>(Tell()) );
	S += wxString::Format( wxT( "GR:<0x%.8x-0x%.8x>")
	, static_cast<dword>(GetGuard().From) 
	, static_cast<dword>(GetGuard().To) 
	);
	return S;	
}

// ============================================================================
//	upFileReader
// ============================================================================
upFileReader::upFileReader( const wxFileName& filename ) : upFile(filename)
{
	//UP_TIMER(t);

	File.Open( FileName.GetFullPath(), wxT("rb") );
	if( !File.IsOpened() || File.Error() )
		throw upexception( wxString::Format( wxT("Could not open file %s"), FileName.GetFullPath() ) );
		
	Buf.Init(File);
	
	//UP_TIMER_LOGF(t,0);
}

void upFileReader::DumpByteInfo()
{
	Buf.DumpByteInfo();
}


// ============================================================================
//	upFileWriter
// ============================================================================
upFileWriter::~upFileWriter()
{
	Buf.Save(File);
}

upFileWriter::upFileWriter( const wxFileName& filename ) : upFile(filename)
{
	//UP_TIMER(t);

	File.Open( FileName.GetFullPath(), wxT("w+b") );
	if( !File.IsOpened() || File.Error() )
		throw upexception( wxString::Format( wxT("Could not open file %s"), FileName.GetFullPath() ) );
		
	Buf.Init(File);
	
	//UP_TIMER_LOGF(t,0);
}

upFileWriter::upFileWriter( const wxFileName& filename, dword maxsize ) : upFile(filename), Buf(maxsize)
{
	//UP_TIMER(t);

	File.Open( FileName.GetFullPath(), wxT("w+b") );
	if( !File.IsOpened() || File.Error() )
		throw upexception( wxString::Format( wxT("Could not open file %s"), FileName.GetFullPath() ) );
		
	Buf.Init(File);
	
	//UP_TIMER_LOGF(t,0);
}




// ============================================================================
//	The End.
// ============================================================================