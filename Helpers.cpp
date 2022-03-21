#include "Helpers.h"

VOID Helpers::Hook(PVOID* ppPointer, PVOID pDetour)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(ppPointer, pDetour);

	DetourTransactionCommit();
}

VOID Helpers::UnHook(PVOID* ppPointer, PVOID pDetour)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(ppPointer, pDetour);

	DetourTransactionCommit();
}