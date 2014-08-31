// RpLZMA_EXE.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	if( argc == 4 )
	{
		if( _tcscmp(argv[1],_T("c")) == 0 )
		{
			RpCompress(argv[2],argv[3]);
		}
		else if( _tcscmp(argv[1],_T("d")) == 0 )
		{
			RpDecompress(argv[2],argv[3]);
		}
	}

	return 0;
}

