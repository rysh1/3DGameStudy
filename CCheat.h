#pragma once
#include <d3dcompiler.h>
#include <d3d11.h>

extern ID3D11Device* pDevice;
extern ID3D11DeviceContext* pContext;
extern IDXGISwapChain* pSwapChain;
extern ID3D11RenderTargetView* pRenderTargetView;
class CCheat
{
public:
	CCheat();
	~CCheat();
	void Initialize();
	void Reseale();

private:
};
