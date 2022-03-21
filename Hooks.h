#pragma once
#include "CCheat.h"

using tPresent = HRESULT(WINAPI*)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

using tDrawIndexed = void(STDMETHODCALLTYPE*)(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT  BaseVertexLocation);

namespace Hooks
{
	extern tPresent oPresent;
	extern tDrawIndexed oDrawIndexed;
	HRESULT WINAPI HookD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

	void STDMETHODCALLTYPE HookD3D11DrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT  BaseVertexLocation);
}
