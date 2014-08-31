// upCmd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "uppPkg.h"
#include "updPkg.h"
#include "upFile.h"
#include "sha1.h"


// ============================================================================
// upCmd
// ============================================================================
class upCmd : public wxAppConsole
{
public:
    virtual bool OnInit() { return true; }
    virtual int OnRun();

private:
    void ParseParams(const wxCmdLineParser& cmdline);
	void LoadPackages( const wxString& filename );
	void LoadPackage( const wxString& filename );
    
	// all the other stuff
};

IMPLEMENT_APP_CONSOLE(upCmd);


int upCmd::OnRun()
{
	try 
	{
		static const wxCmdLineEntryDesc cmdLineDesc[] =
		{
			{ wxCMD_LINE_PARAM,  NULL, NULL, _T("input file"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
			{ wxCMD_LINE_NONE, NULL, NULL, NULL, (wxCmdLineParamType)0, 0 }
		};

		wxCmdLineParser parser(cmdLineDesc, argc, argv);
		
		switch (parser.Parse())
		{
			case -1:
				return 0;

			case 0:
				ParseParams(parser);
				return 0;
		}

		//return 1;
		/*
		static const wxCmdLineEntryDesc cmdLineDesc[] =
		{
			{ wxCMD_LINE_SWITCH, _T("h"), _T("help"),  _T("show help message"), wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
			{ wxCMD_LINE_SWITCH, _T("v"), _T("verbose"), _T("be verbose"), (wxCmdLineParamType)0, 0 },
			{ wxCMD_LINE_SWITCH, _T("e"), _T("extra-cpp-code"),  _T("output C++ header file with XRC derived classes"), (wxCmdLineParamType)0, 0 },
			{ wxCMD_LINE_SWITCH, _T("c"), _T("cpp-code"),  _T("output C++ source rather than .rsc file"), (wxCmdLineParamType)0, 0 },
			{ wxCMD_LINE_SWITCH, _T("p"), _T("python-code"),  _T("output wxPython source rather than .rsc file"), (wxCmdLineParamType)0, 0 },
			{ wxCMD_LINE_SWITCH, _T("g"), _T("gettext"),  _T("output list of translatable strings (to stdout or file if -o used)"), (wxCmdLineParamType)0, 0 },
			{ wxCMD_LINE_OPTION, _T("n"), _T("function"),  _T("C++/Python function name (with -c or -p) [InitXmlResource]"), (wxCmdLineParamType)0, 0 },
			{ wxCMD_LINE_OPTION, _T("o"), _T("output"),  _T("output file [resource.xrs/cpp]"), (wxCmdLineParamType)0, 0 },
			{ wxCMD_LINE_PARAM,  NULL, NULL, _T("input file(s)"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_MULTIPLE | wxCMD_LINE_OPTION_MANDATORY },
			{ wxCMD_LINE_NONE, NULL, NULL, NULL, (wxCmdLineParamType)0, 0 }
		};
		*/
		return 0;

	} 
	catch( upexception& e ) { wxLogError(wxT("%s"), e.wwhat()); return 1; }		  
	catch( exception& e ) { wxLogError(wxT("%hs"), e.what()); return 1; }
}



void upCmd::ParseParams(const wxCmdLineParser& cmdline)
{
	if( cmdline.GetParamCount() == 0 )
	{
		wxArrayString files;
		wxDir::GetAllFiles( wxT("."), &files, wxEmptyString, wxDIR_FILES | wxDIR_HIDDEN | wxDIR_DIRS );

		for( size_t i=0; i!=files.Count(); ++i )
		{
			LoadPackages(files[i]);
		}
	}
	else
	{
		LoadPackage(cmdline.GetParam(0));
	}



/*    flagGettext = cmdline.Found(_T("g"));
    flagVerbose = cmdline.Found(_T("v"));
    flagCPP = cmdline.Found(_T("c"));
    flagPython = cmdline.Found(_T("p"));
    flagH = flagCPP && cmdline.Found(_T("e"));


    if (!cmdline.Found(_T("o"), &parOutput))
    {
        if (flagGettext)
            parOutput = wxEmptyString;
        else
        {
            if (flagCPP)
                parOutput = _T("resource.cpp");
            else if (flagPython)
                parOutput = _T("resource.py");
            else
                parOutput = _T("resource.xrs");
        }
    }
    if (!parOutput.empty())
    {
        wxFileName fn(parOutput);
        fn.Normalize();
        parOutput = fn.GetFullPath();
        parOutputPath = wxPathOnly(parOutput);
    }
    if (!parOutputPath) parOutputPath = _T(".");

    if (!cmdline.Found(_T("n"), &parFuncname))
        parFuncname = _T("InitXmlResource");

    for (size_t i = 0; i < cmdline.GetParamCount(); i++)
    {
#ifdef __WINDOWS__
        wxString fn=wxFindFirstFile(cmdline.GetParam(i), wxFILE);
        while (!fn.empty())
        {
            parFiles.Add(fn);
            fn=wxFindNextFile();
        }
#else
        parFiles.Add(cmdline.GetParam(i));
#endif
    }*/
}

/*int wmain(int argc, wchar_t *argv[], wchar_t *envp[] )
{
	return wxEntry(argc, argv);
	guard;
    wxInitializer initializer;
    if( !initializer )
    {
		throw wxexception( wxString::Format( wxT("Could not open file") ) );
        //fprintf(stderr, wxT("Failed to initialize the wxWidgets library, aborting."));
    }

	getchar();

    wxUnusedVar(argc);
    wxUnusedVar(argv);
    wxUnusedVar(envp);
    return 0;
	unguard;0x003B7EA8
}*/

void upCmd::LoadPackage( const wxString& filename )
{
	guard;

	wxLogMessage( wxT("") );
	wxLogMessage( wxT("LoadPackage() %s"), filename.c_str() );
	UP_TIMER(t);

	
	// create delta
	{

		wxString mapstr = filename + wxT(".dx"); // dx
		wxString savestr = filename + wxT(".dxs"); // dxs
		wxString deltastr = filename + wxT(".dxd"); // dxd
		

		// if delta needs to be created
		if( false )
		{
			// prepare files
			upFileReader mapfr( mapstr );
			upFileReader savefr( savestr );
			upFileWriter deltafw( deltastr, savefr.Length() );

			// load map
			uppPkg mappkg;
			mappkg.Serialize(mapfr);

			// load save
			uppPkg savepkg;
			savepkg.Serialize(savefr);

			// create delta
			updPkg deltapkg(mappkg,savepkg);
			deltapkg.Serialize(deltafw);
		}
		else
		{
			wxFile f(deltastr,wxFile::write);
			byte b = 1;
			f.Write(&b,1);
			f.Close();
		}
	}

	UP_TIMER_LOGF(t,1);

	// apply delta
	{

		wxString mapstr = filename + wxT(".dx"); // dx
		wxString savestr = filename + wxT(".dxs2"); // dxs
		wxString deltastr = filename + wxT(".dxd"); // dxd

		if( wxFileExists(deltastr) )
		{
			// get delta length
			wxFile f(deltastr,wxFile::read);
			int i = f.Length();
			f.Close();

			// if delta length == 1, copy the map
			if( i != 1 )
			{
				// prepare files
				upFileReader mapfr( mapstr );
				upFileWriter savefw( savestr, mapfr.Length() );
				upFileReader deltafr( deltastr );

				// load map
				uppPkg mappkg;
				mappkg.Serialize(mapfr);

				// apply delta
				uppPkg savepkg;
				updPkg deltapkg(mappkg,savepkg);
				deltapkg.Serialize(deltafr);

				// write save
				savepkg.Serialize(savefw);
			}
			else
			{
				wxCopyFile(mapstr,savestr,true);
			}

		}
	}

	UP_TIMER_LOGF(t,1);

	// compare hashes
	{
		upFileReader abasis( filename + wxT(".dxs") );
		upFileReader atemp( filename + wxT(".dxs2") );

		dword hash[2][5];

		SHA1 sha;

		sha.Reset();
		sha.Input(abasis.GetDataPtr(),abasis.Length());
		sha.Result(hash[0]);
		
		sha.Reset();
		sha.Input(atemp.GetDataPtr(),atemp.Length());
		sha.Result(hash[1]);

		for( int i=0; i!=5; ++i )
		{
			if( hash[0][i] != hash[1][i] )
				throw upexception( wxT("Hash Mismatch!") );
		}
	}


	UP_TIMER_LOGF(t,0);
	//wxLogMessage( wxT("%s"), UPG_Stats.c_str() );
	unguard;
}


void upCmd::LoadPackages( const wxString& filename )
{
	guard;
	if( filename.Find(wxT(".temp")) != -1 )
		return;

	wxLogMessage( wxT("") );
	wxLogMessage( wxT("LoadPackages() %s"), filename.c_str() );
	//UP_TIMER(t);

	// load and serialize to temp
	{
		upFileReader abasis( filename );
		upFileWriter atemp( filename + wxT(".temp"), abasis.Length() );

		uppPkg pbasis;

		pbasis.Serialize(abasis);
		pbasis.Serialize(atemp);
	}

	// compare hashes
	{
		upFileReader abasis( filename );
		upFileReader atemp( filename + wxT(".temp") );

		dword hash[2][5];

		SHA1 sha;

		sha.Reset();
		sha.Input(abasis.GetDataPtr(),abasis.Length());
		sha.Result(hash[0]);
		
		sha.Reset();
		sha.Input(atemp.GetDataPtr(),atemp.Length());
		sha.Result(hash[1]);

		for( int i=0; i!=5; ++i )
		{
			if( hash[0][i] != hash[1][i] )
				throw upexception( wxT("Hash Mismatch!") );
		}
	}

	//UP_TIMER_LOGF(t,0);
	//wxLogMessage( wxT("%s"), UPG_Stats.c_str() );
	unguard;
}