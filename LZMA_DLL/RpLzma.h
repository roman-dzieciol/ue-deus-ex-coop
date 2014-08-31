#ifdef RPLZMA_EXPORTS
#define RPLZMA_API __declspec(dllexport)
#else
#define RPLZMA_API __declspec(dllimport)
#endif


extern "C" RPLZMA_API int RpCompress(const wchar_t* infile, const wchar_t* outfile);
extern "C" RPLZMA_API int RpDecompress(const wchar_t* infile, const wchar_t* outfile);
