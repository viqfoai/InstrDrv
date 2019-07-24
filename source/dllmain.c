#include<Windows.h>


BOOL APIENTRY DllMain(HMODULE hMoudle/* hModule */, DWORD ul_reason_for_call, LPVOID lParam/* lpReserved */)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}