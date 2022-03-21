#pragma once
#include "pch.h"
#include "detours.h"

class Helpers
{
public:
	static VOID Hook(PVOID* ppPointer, PVOID pDetour);
	static VOID UnHook(PVOID* ppPointer, PVOID pDetour);
};
