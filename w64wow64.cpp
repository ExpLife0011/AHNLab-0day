
#include <Windows.h>
#include "w64wow64.h"
#include "internal.h"
#include "windef.h"

unsigned __int64 X64Call(DWORD64 lvpFunctionPtr,
	int nArgc, ... )
{
	va_list args;
	DWORD64 arg1, arg2, arg3, arg4, _nArgc, _lvpFunctionPtr, rest;
	DWORD dwEspBackup;
	union reg64 sRax;

	DWORD* dwArgsAddr = (DWORD*)&nArgc + 1;

	DWORD64 *dwArgc64 = (DWORD64*)dwArgsAddr;

	arg1 = (nArgc) ? nArgc--, dwArgc64++, *(dwArgc64-1) : 0;
	arg2 = (nArgc) ? nArgc--, dwArgc64++, *(dwArgc64 - 1) : 0;
	arg3 = (nArgc) ? nArgc--, dwArgc64++, *(dwArgc64 - 1) : 0;
	arg4 = (nArgc) ? nArgc--, dwArgc64++, *(dwArgc64 - 1) : 0;

	rest = (DWORD64)dwArgc64;

	_nArgc = nArgc;
	_lvpFunctionPtr = lvpFunctionPtr;
	dwEspBackup;
	sRax.v = 0;
	__asm {
		mov dwEspBackup, esp
		and sp, 0xFFF8
		X64_Start();
		push arg1
		X64_Pop(_RCX);
		push arg2
		X64_Pop(_RDX);
		push arg3
		X64_Pop(_R8);
		push arg4
		X64_Pop(_R9);
		
		push   edi
		push   rest
		X64_Pop(_RDI);
		push   _nArgc
		X64_Pop(_RAX);
		test   eax, eax
		jz     _ls_e
		lea    edi, dword ptr [edi + 8*eax - 8]
	_ls:
		test   eax, eax
		jz     _ls_e
		push   dword ptr [edi]
		sub    edi, 8
		sub    eax, 1
		jmp    _ls
	_ls_e:
		sub    esp, 0x20
		call   _lvpFunctionPtr
		push   _nArgc
		X64_Pop(_RCX);
		lea    esp, dword ptr [esp + 8*ecx + 0x20]
		pop    edi
		X64_Push(_RAX);
		pop    sRax.dw[0]
		X64_End();
		mov    esp, dwEspBackup
	}
}

DWORD64 WINAPI GetModuleBase64(wchar_t * lwcModuleName)
{

	reg64 dw64Ldr;

	DWORD dwHigh = 0;
	DWORD dwLow = 0;
	
	_asm {

		push 0
		push lwcModuleName
		
	}

	X64_Start();

		_asm {

			X64_Push(_RBX)
			X64_Push(_RCX)
			X64_Push(_RDX)
			X64_Push(_RSI)
			X64_Push(_RDI)
			X64_Push(_R8)
			X64_Push(_R9)
			X64_Push(_R10);
			X64_Push(_R11);
			X64_Push(_R12);
			X64_Push(_R13);
			X64_Push(_R14);
			X64_Push(_R15);
			


			dec    eax												// add rsp,0x68
			add    esp, 0x68										//

													
			X64_Pop(_RCX);											// RCX = lwcModuleName
			EMIT(0x65) EMIT(0x4c) EMIT(0x8b) EMIT(0x14) 
			EMIT(0x25) EMIT(0x30) EMIT(0x00)						
			EMIT(0x00) EMIT(0x00)									// mov    r10, QWORD PTR gs : 0x30 ; TEB
			
			EMIT(0x4d) EMIT(0x8b) EMIT(0x52) EMIT(0x60)             // mov    r10, QWORD PTR[r10 + 0x60] ; TEB->peb

			EMIT(0x4d) EMIT(0x8b) EMIT(0x52) EMIT(0x18)             // mov    r10, QWORD PTR[r10 + 0x18] ; PEB->Ldr

			EMIT(0x4d) EMIT(0x8b) EMIT(0x5a) EMIT(0x20)             // mov    r11, QWORD PTR[r10 + 0x20] ; LDR->InMemoryOrderLinks.Flink

			EMIT (0x4d) EMIT(0x89) EMIT(0xdc)						// mov    r12, r11 

again:
			EMIT(0x4d) EMIT(0x8d) EMIT(0x6b) EMIT(0x48)             // lea    r13, [r11 + 0x48]4d 8d 6b 58             lea    r13, [r11 + 0x58]
			EMIT(0x66) EMIT(0x41) EMIT(0x8b) EMIT(0x5d) EMIT(0x00)  // mov    bx, WORD PTR[r13 + 0x0]

			test bx,bx
			jz next_entry

			call AsmWcslen
			EMIT(0x48) EMIT(0x01) EMIT(0xc0)						// add    rax, rax
			cmp ax,bx
			jnz next_entry

			EMIT(0x49) EMIT(0x8b) EMIT(0x55) EMIT(0x08)             // mov    rdx, QWORD PTR[r13 + 0x8]
			EMIT(0x49) EMIT(0x89) EMIT(0xc0)						// mov    r8, rax

			EMIT(0x49) EMIT(0xd1) EMIT(0xf8)						// sar    r8, 1
			call CompareIcase
			test eax,eax
			jz next_entry

			EMIT(0x49) EMIT(0x8b) EMIT(0x43) EMIT(0x20)             // mov    rax, QWORD PTR[r11 + 0x20]
			jmp end
next_entry:
			EMIT(0x4d) EMIT(0x8b) EMIT(0x1b)						// mov r11,[r11]
			EMIT(0x4d) EMIT(0x39) EMIT(0xe3)						// cmp r11,r12
			jnz again
			EMIT(0x31) EMIT(0xc0)									// xor    eax, eax
end:
			dec    eax												// sub rsp,0x78
			sub    esp, 0x70										//

			X64_Pop(_R15);
			X64_Pop(_R14);
			X64_Pop(_R13);
			X64_Pop(_R12);
			X64_Pop(_R11);
			X64_Pop(_R10);
			X64_Pop(_R9);
			X64_Pop(_R8);
			X64_Pop(_RDI);
			X64_Pop(_RSI);
			X64_Pop(_RDX);
			X64_Pop(_RCX);
			X64_Pop(_RBX);

			X64_Push(_RAX)

		}
	X64_End();
	_asm {
		pop dwLow
		pop dwHigh
	}
	
	dw64Ldr.dw[0] = dwLow;
	dw64Ldr.dw[1] = dwHigh;

	return dw64Ldr.v;
}

DWORD64 WINAPI GetProcAddress64(DWORD64 lvpBaseAddress, char * lpszProcName )
{
	
	DWORD64 qwResult = 0;
	DWORD64 qwBase = lvpBaseAddress;

	DWORD64 qwNameLength = lstrlenA(lpszProcName);
	DWORD64 qwStr = (DWORD64)lpszProcName;


	X64_Start();
	_asm {

		X64_Push(_RBX)
		X64_Push(_RCX)
		X64_Push(_RDX)
		X64_Push(_RSI)
		X64_Push(_RDI)
		X64_Push(_R8)
		X64_Push(_R9)
		X64_Push(_R10)
		X64_Push(_R11)
		X64_Push(_R12)
		X64_Push(_R13)
		X64_Push(_R14)
		X64_Push(_R15)

		push qwBase
	
		X64_Pop(_RBX);

		push qwNameLength

		X64_Pop(_R15)

		push qwStr

		X64_Pop(_R14)

		EMIT(0x48) EMIT(0x89) EMIT(0xd9)                // mov    rcx, rbx
		call AsmGetModule64NtHeader						// rax = psNtHeader
		EMIT (0x49) EMIT (0x89) EMIT(0xc2)              // mov    r10, rax
		
		EMIT (0x48) EMIT(0x8d) EMIT (0x80) EMIT(0x88)
		EMIT (00) EMIT (00) EMIT (00)					// lea    rax, [rax + 0x88]; rax = DataDirectory[IMAGE_EXPORT_DIRECTORY]
		
		EMIT (0x8b) EMIT (0x10)							// mov    edx, DWORD PTR[rax]
		EMIT (0x48) EMIT(01) EMIT(0xda)                 // add    rdx, rbx

		/***************************************Parse export table address/names/ordinals RVA+Base******************************************************/
		EMIT(0x8b) EMIT(0x4a) EMIT(0x18)                // mov    ecx, DWORD PTR[rdx + 0x18] ; rcx = NumberOfNames
		EMIT(0xff) EMIT(0xc9)							// dec    ecx
		EMIT(0x44) EMIT(0x8b) EMIT(0x52) EMIT(0x1c)     // mov    r10d, DWORD PTR[rdx + 0x1c] 
		EMIT(0x49) EMIT(0x01) EMIT(0xda)                // add    r10, rbx                   ; r10 = AddressOfFunctions
	
		EMIT(0x44) EMIT(0x8b) EMIT(0x5a) EMIT(0x20)     // mov    r11d, DWORD PTR[rdx + 0x20]
		EMIT(0x49) EMIT(0x01) EMIT(0xdb)                // add    r11, rbx                   ; r11 = AddressOfNames

		EMIT(0x44) EMIT(0x8b) EMIT(0x62) EMIT(0x24)     // mov    r12d, DWORD PTR[rdx + 0x24]
		EMIT(0x49) EMIT(0x01) EMIT(0xdc)                // add    r12, rbx					 ; r12 = AddressOfNameOrdinals
	
repeat:
		EMIT(0x45) EMIT(0x8b) EMIT(0x04) EMIT(0x8b)    // mov    r8d, DWORD PTR[r11 + rcx * 4]
		EMIT(0x4d) EMIT(0x85) EMIT(0xc0)               // test   r8, r8
		jz next_name
		EMIT(0x49) EMIT(0x01) EMIT(0xd8)               // add    r8, rbx
		EMIT(0x4c) EMIT(0x87) EMIT(0xc1)               // xchg   rcx, r8
		call AsmStrlen
		EMIT(0x49) EMIT(0x87) EMIT(0xc8)               // xchg   r8, rcx
		EMIT(0x4c) EMIT(0x39) EMIT(0xf8)               // cmp    rax, r15
		jnz next_name
		EMIT(0x49) EMIT(0x89) EMIT(0xcd)               // mov    r13, rcx
		EMIT(0x4c) EMIT(0x89) EMIT(0xc1)               // mov    rcx, r8
		EMIT(0x4d) EMIT(0x89) EMIT(0xf8)               // mov    r8, r15
		EMIT(0x4c) EMIT(0x89) EMIT(0xf2)               // mov    rdx, r14
		call AsmStrcmp
	    EMIT(0x4c) EMIT(0x89) EMIT(0xe9)               // mov    rcx, r13
		EMIT(0x48) EMIT(0x85) EMIT(0xc0)               // test   rax, rax
		jnz next_name
		EMIT(0x31) EMIT(0xd2)						    // xor    edx, edx
		EMIT(0x66) EMIT(0x41) EMIT(0x8b) 
		EMIT(0x14) EMIT(0x4c)						    // mov    dx, WORD PTR[r12 + rcx * 2]
		EMIT(0x41) EMIT(0x8b) EMIT(0x04)  EMIT(0x92)    // mov    eax, DWORD PTR[r10 + rdx * 4]
		EMIT(0x48) EMIT(0x01) EMIT(0xd8)                // add    rax, rbx
		jmp end
next_name:
		EMIT(0x31) EMIT(0xc0)						   // xor    eax, eax
		EMIT(0x48) EMIT(0xff) EMIT(0xc9)               // dec    rcx
		jns repeat
end:

		X64_Pop(_R15)
		X64_Pop(_R14)
		X64_Pop(_R13)
		X64_Pop(_R12)
		X64_Pop(_R11)
		X64_Pop(_R10)
		X64_Pop(_R9)
		X64_Pop(_R8)
		X64_Pop(_RDI)
		X64_Pop(_RSI)
		X64_Pop(_RDX)
		X64_Pop(_RCX)
		X64_Pop(_RBX)

		X64_Push(_RAX)

		pop dword ptr[qwResult]
	}


	X64_End();

	return qwResult;
}


