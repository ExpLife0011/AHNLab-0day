#ifndef __W64WOW64_H_
#define __W64WOW64_H_

extern "C" void ToLower();
extern "C" void AsmWcslen();
extern "C" void CompareIcase();
extern "C" void AsmGetModule64NtHeader();
extern "C" void AsmStrlen();
extern "C" void AsmStrcmp();


unsigned __int64 X64Call( DWORD64, 
	int nArgc, ... );
DWORD64 WINAPI GetProcAddress64( DWORD64 lvpBaseAddress, 
	char * lpszProcName );
void * LoadLibrary64A( char * lpcLibraryName );
DWORD64 WINAPI GetModuleBase64(wchar_t * lwcModuleName);

#endif