#include "pch.h"
#include "Hooks.h"
#include "CCheat.h"
#include ".\ImGui\imgui.h"
#include ".\ImGui\imgui_impl_dx11.h"

namespace Hooks
{
	bool bOnce = false;
	bool bShowWindow = true;
	bool g_bColouring = false;
	bool g_bClose_ZEnable = false;
	tPresent oPresent = nullptr;
	tDrawIndexed oDrawIndexed = nullptr;

	//顶点信息
	ID3D11Buffer* Buff = nullptr;
	UINT Strides = 0;
	UINT Offsets = 0;
	D3D11_BUFFER_DESC vedesc;

	//index
	ID3D11Buffer* inBuffer;
	DXGI_FORMAT inFormat;
	UINT inOffset;
	D3D11_BUFFER_DESC indesc;

	enum eDepthStencilState
	{
		ENABLE,
		DISABLE,
		READ_NO_WRITE,
		NO_READ_NO_WRITE,
		_DEPTH_COUNT
	};

	ID3D11DepthStencilState* DepthStencilState[static_cast<int>(eDepthStencilState::_DEPTH_COUNT)];

	ID3D11PixelShader* psRed = NULL;
	ID3D11PixelShader* psGreen = NULL;
	ID3D11ShaderResourceView* view = nullptr;

	void SetDepthStencilState(eDepthStencilState State)
	{
		pContext->OMSetDepthStencilState(DepthStencilState[State], 1);
	}

	HRESULT GenerateShader(ID3D11Device* pD3DDevice, ID3D11PixelShader** pShader, float r, float g, float b)
	{
		char szCast[] = "struct VS_OUT"
			"{"
			"    float4 Position   : SV_Position;"
			"    float4 Color    : COLOR0;"
			"};"

			"float4 main( VS_OUT input ) : SV_Target"
			"{"
			"    float4 fake;"
			"    fake.a = 1.0;"
			"    fake.r = %f;"
			"    fake.g = %f;"
			"    fake.b = %f;"
			"    return fake;"
			"}";
		ID3D10Blob* pBlob;
		ID3D10Blob* Error;
		char szPixelShader[1000];

		sprintf(szPixelShader, szCast, r, g, b);

		HRESULT hr = D3DCompile(szPixelShader, sizeof(szPixelShader), "shader", NULL, NULL, "main", "ps_4_0", NULL, NULL, &pBlob, &Error);

		if (FAILED(hr))
			return hr;

		hr = pD3DDevice->CreatePixelShader((DWORD*)pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, pShader);

		if (FAILED(hr))
			return hr;

		return S_OK;
	}

	HRESULT WINAPI HookD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
	{
		if (!bOnce)
		{
			//第一次会进来  初始化一些内容
			//得到游戏窗体
			HWND hWindow = FindWindow(NULL, TEXT("Sniper3"));
			if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)(&pDevice))))
			{
				pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)(&pDevice));
				pDevice->GetImmediateContext(&pContext);
			}
			ID3D11Texture2D* renderTargetTexture = nullptr;
			if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (PVOID*)(&renderTargetTexture))))
			{
				pDevice->CreateRenderTargetView(renderTargetTexture, NULL, &pRenderTargetView);
				renderTargetTexture->Release();
			}

			D3D11_DEPTH_STENCIL_DESC stencilDesc;
			stencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
			stencilDesc.StencilEnable = TRUE;
			stencilDesc.StencilReadMask = 0xff;
			stencilDesc.StencilWriteMask = 0xff;
			stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
			stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
			stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			//启用  （启用深度缓冲就是不透视）
			stencilDesc.DepthEnable = true;
			stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			pDevice->CreateDepthStencilState(&stencilDesc, &DepthStencilState[static_cast<int>(eDepthStencilState::ENABLE)]);

			//关闭
			stencilDesc.DepthEnable = false;
			stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			pDevice->CreateDepthStencilState(&stencilDesc, &DepthStencilState[static_cast<int>(eDepthStencilState::DISABLE)]);

			//不读不写
			stencilDesc.DepthEnable = false;
			stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			stencilDesc.StencilEnable = FALSE;
			stencilDesc.StencilReadMask = (UINT8)0xFF;
			stencilDesc.StencilWriteMask = 0x0;
			pDevice->CreateDepthStencilState(&stencilDesc, &DepthStencilState[static_cast<int>(eDepthStencilState::NO_READ_NO_WRITE)]);

			//读  不写
			stencilDesc.DepthEnable = TRUE;
			stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			stencilDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
			stencilDesc.StencilEnable = FALSE;
			stencilDesc.StencilWriteMask = (UINT8)0xFF;
			stencilDesc.StencilReadMask = 0x0;

			stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
			stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
			stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

			stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
			stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
			stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
			stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
			pDevice->CreateDepthStencilState(&stencilDesc, &DepthStencilState[static_cast<int>(eDepthStencilState::READ_NO_WRITE)]);

			//着色器
			GenerateShader(pDevice, &psRed, 1.0f, 0.0f, 0.0f);
			GenerateShader(pDevice, &psGreen, 0.0f, 1.0f, 0.0f);

			//初始化ImGui
			ImGui_ImplDX11_Init(hWindow, pDevice, pContext);
			ImGui_ImplDX11_CreateDeviceObjects();
			ImGui::StyleColorsDark();

			bOnce = true;
		}
		pContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);
		//不断地画界面
		ImGui_ImplDX11_NewFrame();

		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(300, 500));
		ImGui::Begin("3DGame", &bShowWindow);
		if (ImGui::CollapsingHeader("Function"))
		{
			ImGui::Checkbox("Clairvoyance", &g_bClose_ZEnable);
			ImGui::Checkbox("Character Color", &g_bColouring);
		}
		if (g_bClose_ZEnable)
		{
			//开启透视
		}

		if (g_bColouring)
		{
			//开启人物上色
		}
		ImGui::End();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		return oPresent(pSwapChain, SyncInterval, Flags);
	}

	void STDMETHODCALLTYPE HookD3D11DrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
	{
		pContext->IAGetVertexBuffers(0, 1, &Buff, &Strides, &Offsets);
		if (Buff)
		{
			Buff->GetDesc(&vedesc);
			Buff->Release();
			Buff = nullptr;
		}
		else
		{
			goto end;
		}

		pContext->IAGetIndexBuffer(&inBuffer, &inFormat, &inOffset);
		if (inBuffer)
		{
			inBuffer->GetDesc(&indesc);
			inBuffer->Release();
			inBuffer = nullptr;
		}
		else
		{
			goto end;
		}

		if (Strides == 40)
		{
			if (indesc.ByteWidth == 1146 || indesc.ByteWidth == 1488 || indesc.ByteWidth == 1404 || indesc.ByteWidth == 1848 || indesc.ByteWidth == 1836 || indesc.ByteWidth == 1878 || indesc.ByteWidth == 2154 || indesc.ByteWidth == 3018 || indesc.ByteWidth == 3024 || indesc.ByteWidth == 3624 || indesc.ByteWidth == 3720 || indesc.ByteWidth == 5154 || indesc.ByteWidth == 6300 || indesc.ByteWidth == 12288 || indesc.ByteWidth == 12708 || indesc.ByteWidth == 13980 || indesc.ByteWidth == 19524 || indesc.ByteWidth == 20940 || indesc.ByteWidth == 26982 || indesc.ByteWidth == 36012 || indesc.ByteWidth == 64170 || indesc.ByteWidth == 115434)
			{
				SetDepthStencilState(DISABLE);//禁用Z轴缓冲
				pContext->PSSetShader(psRed, nullptr, 0);
				oDrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
				SetDepthStencilState(ENABLE);
				pContext->PSSetShader(psGreen, nullptr, 0);
				pContext->PSSetShaderResources(0, 1, &view);
			}
		}
	end:
		return oDrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
	}
}