#include "DX11Renderer.h"

namespace OGE
{
	namespace render
	{
		bool DX11Renderer::initialize(HWND hWnd)
		{
			DXGI_SWAP_CHAIN_DESC swapChaidDesc;
			ZeroMemory(&swapChaidDesc, sizeof(swapChaidDesc));

			DXGI_MODE_DESC bestMode;
			ZeroMemory(&bestMode, sizeof(bestMode));

			DXGI_MODE_DESC* modeDescs = NULL;
			IDXGIFactory* pFactory = NULL;
			IDXGIAdapter* pAdapter = NULL;
			IDXGIOutput* pAdapterOutput = NULL;
			UINT numModes = 0;

			HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
			if(FAILED(result))
			{
				error_msg("Cannot create DXGIFactory");
				return false;
			}

			// Use adapter factory for create adapter for primary graphics card
			result = pFactory->EnumAdapters(0, &pAdapter);
			if(FAILED(result))
			{
				error_msg("Cannot create adapter for primary graphics card");
				return false;
			}

			result = pAdapter->EnumOutputs(0, &pAdapterOutput);
			if(FAILED(result))
			{
				error_msg("Cannot get adapter output");
				return false;
			}

			result = pAdapterOutput->GetDisplayModeList(defaultSurfaceFormat,
				DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);

			if(FAILED(result) || numModes <= 0)
			{
				error_msg("Cannot get display mode list");
				return false;
			}

			modeDescs = new DXGI_MODE_DESC[numModes];
		
			result = pAdapterOutput->GetDisplayModeList(defaultSurfaceFormat,
				DXGI_ENUM_MODES_INTERLACED, &numModes, modeDescs);

			if(FAILED(result))
			{
				error_msg("Cannot get display mode list");
				return false;
			}
		
			for(uint32 i = 0; i < numModes; i++)
			{
				if(modeDescs[i].Width > bestMode.Width || 
					modeDescs[i].Height > bestMode.Height ||
					modeDescs[i].RefreshRate.Numerator > bestMode.RefreshRate.Numerator)
				{
					bestMode = modeDescs[i];
				}
			}

			swapChaidDesc.BufferCount = 2;              
			swapChaidDesc.BufferDesc.Width = bestMode.Width;        
			swapChaidDesc.BufferDesc.Height = bestMode.Height;            
			swapChaidDesc.BufferDesc.Format = bestMode.Format; 
			swapChaidDesc.BufferDesc.RefreshRate.Numerator = bestMode.RefreshRate.Numerator;             
			swapChaidDesc.BufferDesc.RefreshRate.Denominator = bestMode.RefreshRate.Denominator;
			swapChaidDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
			swapChaidDesc.OutputWindow = hWnd;           
			swapChaidDesc.SampleDesc.Count = 1;
			swapChaidDesc.SampleDesc.Quality = 0;
			swapChaidDesc.Windowed = TRUE;
			swapChaidDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

			D3D_FEATURE_LEVEL featureLevel= D3D_FEATURE_LEVEL_11_0;
			UINT flags = 0;

			#ifdef DEBUG_ENABLE

			flags |= D3D11_CREATE_DEVICE_DEBUG;

			#endif

			result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
				0, &featureLevel, 1, D3D11_SDK_VERSION,
				&swapChaidDesc, &pSwapChain, &pDevice, NULL, &pContext);

			pAdapterOutput->Release();
			pAdapter->Release();
			pFactory->Release();
		
			if(FAILED(result))
			{
				switch (result)
				{
					case DXGI_ERROR_NOT_CURRENTLY_AVAILABLE :
						error_msg("By creating device was obtained error : DXGI_ERROR_NOT_CURRENTLY_AVAILABLE"); break;
					case E_INVALIDARG : 
						error_msg("By creating device was obtained error : E_INVALIDARG"); break;
				}

				return false;
			}

			// create default render target view
			ID3D11Texture2D* pBackBuffer = nullptr;
			result = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
			if(FAILED(result))
			{
				error_msg("Cannot get swap chain back buffer");
				return false;
			}

			result = pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pDefaultRenderTargetView);
			if(FAILED(result))
			{
				error_msg("Cannot create render target view");
				return false;
			}

			D3D11_TEXTURE2D_DESC depthBufferDesc;
			ID3D11Texture2D* pDepthStencilBuffer;
			// Initialize the description of the depth buffer.
			ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

			// Set up the description of the depth buffer.
			depthBufferDesc.Width = bestMode.Width;
			depthBufferDesc.Height = bestMode.Height;
			depthBufferDesc.MipLevels = 1;
			depthBufferDesc.ArraySize = 1;
			depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthBufferDesc.SampleDesc.Count = 1;
			depthBufferDesc.SampleDesc.Quality = 0;
			depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			depthBufferDesc.CPUAccessFlags = 0;
			depthBufferDesc.MiscFlags = 0;

			// Create the texture for the depth buffer using the filled out description.
			result = pDevice->CreateTexture2D(&depthBufferDesc, NULL, &pDepthStencilBuffer);
			if(FAILED(result))
			{
				error_msg("Cannot create 2d texture ");
				return false;
			}

			// Initialize the description of the stencil state.
			D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
			D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

			ID3D11DepthStencilState* pDepthStencilState = NULL;

			ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

			// Set up the description of the stencil state.
			depthStencilDesc.DepthEnable =	true;
			depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;

			depthStencilDesc.StencilEnable = false;
			depthStencilDesc.StencilReadMask = 0xFF;
			depthStencilDesc.StencilWriteMask = 0xFF;

			// Stencil operations if pixel is front-facing.
			depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
			depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			// Stencil operations if pixel is back-facing.
			depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
			depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			// Create the depth stencil state.
			result = pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState);
			if(FAILED(result))
			{
				error_msg("Cannot create depth - stencil state");
				return false;
			}

			// Set the depth stencil state.
			pContext->OMSetDepthStencilState(pDepthStencilState, 1);

			// Initialize the depth stencil view.
			ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

			// Set up the depth stencil view description.
			depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			depthStencilViewDesc.Texture2D.MipSlice = 0;

			// Create the depth stencil view.
			result = pDevice->CreateDepthStencilView(pDepthStencilBuffer,
				&depthStencilViewDesc, &
				pDepthStencilView);

			if(FAILED(result))
			{
				error_msg("Cannot create depth - stencil view");
				return false;
			}
			
			pContext->OMSetRenderTargets(1, &pDefaultRenderTargetView, pDepthStencilView);

			D3D11_RASTERIZER_DESC rasterDesc;
			ID3D11RasterizerState* pRasterState = NULL;

			ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
			// Setup the raster description which will determine how and what polygons will be drawn.
			rasterDesc.AntialiasedLineEnable = false;
			rasterDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
			rasterDesc.DepthBias = 0;
			rasterDesc.DepthBiasClamp = 0.0f;
			rasterDesc.DepthClipEnable = true;
			rasterDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
			rasterDesc.FrontCounterClockwise = false;
			rasterDesc.MultisampleEnable = false;
			rasterDesc.ScissorEnable = false;
			rasterDesc.SlopeScaledDepthBias = 0.0f;

			// Create the rasterizer state from the description we just filled out.
			result = pDevice->CreateRasterizerState(&rasterDesc, &pRasterState);
			if(FAILED(result))
			{
				error_msg("Cannot create rasterizer state");
				return false;
			}

			pContext->OMSetBlendState(NULL, NULL, 0xffffffff);

			// Now set the rasterizer state.
			pContext->RSSetState(pRasterState);
			
			D3D11_VIEWPORT viewport;
			viewport.Width = (FLOAT)bestMode.Width;
			viewport.Height = (FLOAT)bestMode.Height;
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;

			pContext->RSSetViewports( 1, &viewport );

			pPipelineProcessor = DX11PipelineProcessorConfigurator::createPipelineProcessor(pipelineProcessor, 
				pDevice, pContext, pSwapChain, pDefaultRenderTargetView, pDepthStencilView, bestMode);
			pPipelineProcessor->start();

			return true;
		}

		void DX11Renderer::shutdown()
		{
			if(pDevice != NULL)
			{
				pDevice->Release();
				pDevice = NULL;
			}
			if(pContext != NULL)
			{
				pContext->Release();
				pContext = NULL;
			}
			if(pSwapChain != NULL)
			{
				pSwapChain->Release();
				pSwapChain = NULL;
			}
			if(pDefaultRenderTargetView != NULL)
			{
				pDefaultRenderTargetView->Release();
				pDefaultRenderTargetView = NULL;
			}
			if(pDepthStencilView != NULL)
			{
				pDepthStencilView->Release();
				pDepthStencilView = NULL;
			}

			pPipelineProcessor->shutdown();
			DX11ShaderManager::getInstance()->clearCache();
		}
	}
}