// LzmaAlone.cpp

#include "StdAfx.h"
#include "RpLzma.h"

#include "../../../Common/MyWindows.h"
#include "../../../Common/MyInitGuid.h"

#include <stdio.h>

#if defined(_WIN32) || defined(OS2) || defined(MSDOS)
#include <fcntl.h>
#include <io.h>
#define MY_SET_BINARY_MODE(file) _setmode(_fileno(file), O_BINARY)
#else
#define MY_SET_BINARY_MODE(file)
#endif

#include "../../../Common/CommandLineParser.h"
#include "../../../Common/StringConvert.h"
#include "../../../Common/StringToInt.h"

#include "../../Common/FileStreams.h"
#include "../../Common/StreamUtils.h"

#include "../LZMA/LZMADecoder.h"
#include "../LZMA/LZMAEncoder.h"

#include "LzmaRam.h"

#ifdef COMPRESS_MF_MT
#include "../../../Windows/System.h"
#endif

extern "C"
{
#include "LzmaRamDecode.h"
}

using namespace NCommandLineParser;

#ifdef _WIN32
bool g_IsNT = false;
static inline bool IsItWindowsNT()
{
  OSVERSIONINFO versionInfo;
  versionInfo.dwOSVersionInfoSize = sizeof(versionInfo);
  if (!::GetVersionEx(&versionInfo)) 
    return false;
  return (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT);
}
#endif

static const char *kCantAllocate = "Can not allocate memory";
static const char *kReadError = "Read error";
static const char *kWriteError = "Write error";

namespace NKey {
enum Enum
{
  kHelp1 = 0,
  kHelp2,
  kMode,
  kDictionary,
  kFastBytes,
  kMatchFinderCycles,
  kLitContext,
  kLitPos,
  kPosBits,
  kMatchFinder,
  kMultiThread,
  kEOS,
  kStdIn,
  kStdOut,
  kFilter86
};
}

static const CSwitchForm kSwitchForms[] = 
{
  { L"?",  NSwitchType::kSimple, false },
  { L"H",  NSwitchType::kSimple, false },
  { L"A", NSwitchType::kUnLimitedPostString, false, 1 },
  { L"D", NSwitchType::kUnLimitedPostString, false, 1 },
  { L"FB", NSwitchType::kUnLimitedPostString, false, 1 },
  { L"MC", NSwitchType::kUnLimitedPostString, false, 1 },
  { L"LC", NSwitchType::kUnLimitedPostString, false, 1 },
  { L"LP", NSwitchType::kUnLimitedPostString, false, 1 },
  { L"PB", NSwitchType::kUnLimitedPostString, false, 1 },
  { L"MF", NSwitchType::kUnLimitedPostString, false, 1 },
  { L"MT", NSwitchType::kUnLimitedPostString, false, 0 },
  { L"EOS", NSwitchType::kSimple, false },
  { L"SI",  NSwitchType::kSimple, false },
  { L"SO",  NSwitchType::kSimple, false },
  { L"F86",  NSwitchType::kPostChar, false, 0, 0, L"+" }
};

static const int kNumSwitches = sizeof(kSwitchForms) / sizeof(kSwitchForms[0]);

static void PrintHelp()
{
  fprintf(stderr, "\nUsage:  LZMA <e|d> inputFile outputFile [<switches>...]\n"
             "  e: encode file\n"
             "  d: decode file\n"
             "  b: Benchmark\n"
    "<Switches>\n"
    "  -a{N}:  set compression mode - [0, 1], default: 1 (max)\n"
    "  -d{N}:  set dictionary - [0,30], default: 23 (8MB)\n"
    "  -fb{N}: set number of fast bytes - [5, 273], default: 128\n"
    "  -mc{N}: set number of cycles for match finder\n"
    "  -lc{N}: set number of literal context bits - [0, 8], default: 3\n"
    "  -lp{N}: set number of literal pos bits - [0, 4], default: 0\n"
    "  -pb{N}: set number of pos bits - [0, 4], default: 2\n"
    "  -mf{MF_ID}: set Match Finder: [bt2, bt3, bt4, hc4], default: bt4\n"
    "  -mt{N}: set number of CPU threads\n"
    "  -eos:   write End Of Stream marker\n"
    "  -si:    read data from stdin\n"
    "  -so:    write data to stdout\n"
    );
}

static void PrintHelpAndExit(const char *s)
{
  fprintf(stderr, "\nError: %s\n\n", s);
  PrintHelp();
  throw -1;
}

static void IncorrectCommand()
{
  PrintHelpAndExit("Incorrect command");
}

static void WriteArgumentsToStringList(int numArguments, const char *arguments[], 
    UStringVector &strings)
{
  for(int i = 1; i < numArguments; i++)
    strings.Add(MultiByteToUnicodeString(arguments[i]));
}

static bool GetNumber(const wchar_t *s, UInt32 &value)
{
  value = 0;
  if (MyStringLen(s) == 0)
    return false;
  const wchar_t *end;
  UInt64 res = ConvertStringToUInt64(s, &end);
  if (*end != L'\0')
    return false;
  if (res > 0xFFFFFFFF)
    return false;
  value = UInt32(res);
  return true;
}

int RpLzma(const wchar_t* infile, const wchar_t* outfile, int reverse)
{
  #ifdef _WIN32
  g_IsNT = IsItWindowsNT();
  #endif

  bool unsupportedTypes = (sizeof(Byte) != 1 || sizeof(UInt32) < 4 || sizeof(UInt64) < 4);
  if (unsupportedTypes)
  {
    fprintf(stderr, "Unsupported base types. Edit Common/Types.h and recompile");
    return 1;
  }   

  // options
  UInt32 numThreads = 1;
  bool encodeMode = reverse == 0;
  bool stdInMode = false;
  bool stdOutMode = false;

  // infile
  CMyComPtr<ISequentialInStream> inStream;
  CInFileStream *inStreamSpec = 0;
  if (stdInMode)
  {
    inStream = new CStdInFileStream;
    MY_SET_BINARY_MODE(stdin);
  }
  else
  {
    inStreamSpec = new CInFileStream;
    inStream = inStreamSpec;
    if (!inStreamSpec->Open(infile))
    {
      fprintf(stderr, "\nError: can not open input file %s\n", 
          infile);
      return 1;
    }
  }

  // outfile
  CMyComPtr<ISequentialOutStream> outStream;
  if (stdOutMode)
  {
    outStream = new CStdOutFileStream;
    MY_SET_BINARY_MODE(stdout);
  }
  else
  {
    COutFileStream *outStreamSpec = new COutFileStream;
    outStream = outStreamSpec;
    if (!outStreamSpec->Create(outfile, true))
    {
      fprintf(stderr, "\nError: can not open output file %s\n", 
        outfile);
      return 1;
    }
  }

  UInt64 fileSize;
  if (encodeMode)
  {
    NCompress::NLZMA::CEncoder *encoderSpec = 
      new NCompress::NLZMA::CEncoder;
    CMyComPtr<ICompressCoder> encoder = encoderSpec;

    UInt32 dictionary = 1 << 23;
    UInt32 posStateBits = 2;
    UInt32 litContextBits = 3; // for normal files
    // UInt32 litContextBits = 0; // for 32-bit data
    UInt32 litPosBits = 0;
    // UInt32 litPosBits = 2; // for 32-bit data
    UInt32 algorithm = 1;
    UInt32 numFastBytes = 128;
    UInt32 matchFinderCycles = 16 + numFastBytes / 2;
    bool matchFinderCyclesDefined = false;
    bool eos = false;
 

    PROPID propIDs[] = 
    {
      NCoderPropID::kDictionarySize,
      NCoderPropID::kPosStateBits,
      NCoderPropID::kLitContextBits,
      NCoderPropID::kLitPosBits,
      NCoderPropID::kAlgorithm,
      NCoderPropID::kNumFastBytes,
      NCoderPropID::kMatchFinder,
      NCoderPropID::kEndMarker,
      NCoderPropID::kNumThreads,
      NCoderPropID::kMatchFinderCycles,
    };
    const int kNumPropsMax = sizeof(propIDs) / sizeof(propIDs[0]);

    PROPVARIANT properties[kNumPropsMax];
    for (int p = 0; p < 6; p++)
      properties[p].vt = VT_UI4;

    properties[0].ulVal = (UInt32)dictionary;
    properties[1].ulVal = (UInt32)posStateBits;
    properties[2].ulVal = (UInt32)litContextBits;
    properties[3].ulVal = (UInt32)litPosBits;
    properties[4].ulVal = (UInt32)algorithm;
    properties[5].ulVal = (UInt32)numFastBytes;

    properties[6].vt = VT_BSTR;
    properties[6].bstrVal = (BSTR)(const wchar_t *)L"BT4";

    properties[7].vt = VT_BOOL;
    properties[7].boolVal = eos ? VARIANT_TRUE : VARIANT_FALSE;

    properties[8].vt = VT_UI4;
    properties[8].ulVal = (UInt32)numThreads;

    // it must be last in property list
    properties[9].vt = VT_UI4;
    properties[9].ulVal = (UInt32)matchFinderCycles;

    int numProps = kNumPropsMax;
    if (!matchFinderCyclesDefined)
      numProps--;

    if (encoderSpec->SetCoderProperties(propIDs, properties, numProps) != S_OK)
      IncorrectCommand();
    encoderSpec->WriteCoderProperties(outStream);

    if (eos || stdInMode)
      fileSize = (UInt64)(Int64)-1;
    else
      inStreamSpec->File.GetLength(fileSize);

    for (int i = 0; i < 8; i++)
    {
      Byte b = Byte(fileSize >> (8 * i));
      if (outStream->Write(&b, 1, 0) != S_OK)
      {
        fprintf(stderr, kWriteError);
        return 1;
      }
    }
    HRESULT result = encoder->Code(inStream, outStream, 0, 0, 0);
    if (result == E_OUTOFMEMORY)
    {
      fprintf(stderr, "\nError: Can not allocate memory\n");
      return 1;
    }   
    else if (result != S_OK)
    {
      fprintf(stderr, "\nEncoder error = %X\n", (unsigned int)result);
      return 1;
    }   
  }
  else
  {
    NCompress::NLZMA::CDecoder *decoderSpec = 
        new NCompress::NLZMA::CDecoder;
    CMyComPtr<ICompressCoder> decoder = decoderSpec;
    const UInt32 kPropertiesSize = 5;
    Byte properties[kPropertiesSize];
    UInt32 processedSize;
    if (ReadStream(inStream, properties, kPropertiesSize, &processedSize) != S_OK)
    {
      fprintf(stderr, kReadError);
      return 1;
    }
    if (processedSize != kPropertiesSize)
    {
      fprintf(stderr, kReadError);
      return 1;
    }
    if (decoderSpec->SetDecoderProperties2(properties, kPropertiesSize) != S_OK)
    {
      fprintf(stderr, "SetDecoderProperties error");
      return 1;
    }
    fileSize = 0;
    for (int i = 0; i < 8; i++)
    {
      Byte b;
      if (inStream->Read(&b, 1, &processedSize) != S_OK)
      {
        fprintf(stderr, kReadError);
        return 1;
      }
      if (processedSize != 1)
      {
        fprintf(stderr, kReadError);
        return 1;
      }
      fileSize |= ((UInt64)b) << (8 * i);
    }
    if (decoder->Code(inStream, outStream, 0, &fileSize, 0) != S_OK)
    {
      fprintf(stderr, "Decoder error");
      return 1;
    }   
  }
  return 0;
}

/*int main(int n, const char *args[])
{
  try { return main2(n, args); }
  catch(const char *s) 
  { 
    fprintf(stderr, "\nError: %s\n", s);
    return 1; 
  }
  catch(...) 
  { 
    fprintf(stderr, "\nError\n");
    return 1; 
  }
}*/

BOOL APIENTRY DllMain( HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved )
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		break;

		case DLL_THREAD_ATTACH:
		break;

		case DLL_THREAD_DETACH:
		break;

		case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

RPLZMA_API int RpCompress(const wchar_t* infile, const wchar_t* outfile)
{
  try 
  { 
	  return RpLzma(infile, outfile, 0); 
  }
  catch(const char *s) 
  { 
    fprintf(stderr, "\nRpCompress Error: %s\n", s);
    return 1; 
  }
  catch(...) 
  { 
    fprintf(stderr, "\nRpCompress Error\n");
    return 1; 
  }
}

RPLZMA_API int RpDecompress(const wchar_t* infile, const wchar_t* outfile)
{
  try 
  { 
	  return RpLzma(infile, outfile, 1);
  }
  catch(const char *s) 
  { 
    fprintf(stderr, "\nRpDecompress Error: %s\n", s);
    return 1; 
  }
  catch(...) 
  { 
    fprintf(stderr, "\nRpDecompress Error\n");
    return 1; 
  }
}