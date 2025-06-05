#ifndef LUTOOL_COMMON
#define LUTOOL_COMMON

#ifdef _WIN32
#ifdef LUTOOL_EXPORTS_OPTION  
#define LUTOOL_EXPORTS __declspec(dllexport)  
#else
#define LUTOOL_EXPORTS __declspec(dllimport)  
#endif
#endif // _WIN32


#endif