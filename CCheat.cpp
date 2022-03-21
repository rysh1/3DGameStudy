#include "pch.h"
#include "CCheat.h"
#include "Helpers.h"
#include "Hooks.h"
#include "UserInterface.h"
#include "Render.h"
#include "Input.h"

ID3D11Device* pDevice = nullptr;
ID3D11DeviceContext* pContext = nullptr;
IDXGISwapChain* pSwapChain = nullptr;
ID3D11RenderTargetView* pRenderTargetView = nullptr;

CCheat::CCheat()
{
}

CCheat::~CCheat()
{
}

void CCheat::Initialize()
{
	/*AllocConsole();
	freopen("CON", "w", stdout);
	SetConsoleTitle(TEXT("Sniper3"));*/

	HWND hWindow = FindWindow(NULL, TEXT("Sniper3"));

	//创建D3D11交换链
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));

	scd.BufferCount = 1;

	scd.BufferDesc.Width = 1024;
	scd.BufferDesc.Height = 1280;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.RefreshRate.Numerator = 144;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	scd.OutputWindow = hWindow;

	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;

	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	scd.Windowed = ((GetWindowLongPtr(hWindow, GWL_STYLE) & WS_POPUP) != 0) ? FALSE : TRUE;

	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, &featureLevel, 1, D3D11_SDK_VERSION
		, &scd, &pSwapChain, &pDevice, NULL, &pContext);//创建设备和交换链条

	//从虚表获取Present()
	void** pSwapChainVT = *(void***)pSwapChain;
	Hooks::oPresent = (tPresent)(*(pSwapChainVT + 8));

	//获取DrawIndexed
	void** pContextVT = *(void***)pContext;
	Hooks::oDrawIndexed = (tDrawIndexed)(*(pContextVT + 12));

	Render::Instance().Initialize();
	UserInterface::GetInstance()->SetStyle();
	Input::GetInstance()->StartThread();

	//开始DetoursHook
	Helpers::Hook((PVOID*)&Hooks::oPresent, Hooks::HookD3D11Present);
	Helpers::Hook(reinterpret_cast<PVOID*>(&Hooks::oDrawIndexed), Hooks::HookD3D11DrawIndexed);
}

void CCheat::Reseale()
{
	Helpers::UnHook((PVOID*)&Hooks::oPresent, Hooks::HookD3D11Present);
	Helpers::UnHook(reinterpret_cast<PVOID*>(&Hooks::oDrawIndexed), Hooks::HookD3D11DrawIndexed);
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	Input::GetInstance()->StopThread();
	FreeConsole();
}