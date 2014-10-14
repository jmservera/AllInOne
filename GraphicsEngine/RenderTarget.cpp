#include "stdafx.h"

#include "AllHeaders.h"

namespace GraphicsEngineSpace {

	_RenderTarget::_RenderTarget(GraphicsDevice^device, _Texture2D^ texture, IDXGISwapChain*	swapChain,DXGI_SWAP_CHAIN_DESC*		swapChainDesc)
	{
		m_Device=device;

		m_Texture=texture;

		m_SwapChain = swapChain;

		m_SwapChainDesc = swapChainDesc;

		m_IsDX9BackBuffer = false;

		OnCreate();
	}

	_RenderTarget::_RenderTarget(GraphicsDevice^device, _Texture2D^ texture)
	{
		m_Device = device;

		m_Texture = texture;

		m_SwapChain = nullptr;

		m_SwapChainDesc = nullptr;

		m_IsDX9BackBuffer = false;

		OnCreate();
	}

	void _RenderTarget::Destroy()
	{
		if (m_RenderTargetView != nullptr)
		{
			InternalLog.WriteLine("_RenderTarget::Destroy");

			m_RenderTargetView->Release();

			m_RenderTargetView = nullptr;

			if (m_SwapChain != nullptr)
			{
				InternalLog.WriteLine("_RenderTarget::Destroy. SwapChain");

				m_SwapChain->Release();

				m_SwapChain = nullptr;

				delete m_SwapChainDesc;

				m_SwapChainDesc = nullptr;
			}
		}

	}

	void _RenderTarget::Resize(int width, int height)
	{
		// We only need to Release the backbuffer
		/*
		InternalLog.WriteLine("Releasing backBuffer");

		m_backBufferTexture->Destroy();

		delete m_backBufferTexture;

		InternalLog.WriteLine("Releasing renderTarget");

		m_renderTarget->Destroy();

		delete m_renderTarget;
		*/

		if (m_SwapChain != nullptr)
		{
			InternalLog.WriteLine("Resizing buffers");

			m_SwapChainDesc->BufferDesc.Width = width;
			m_SwapChainDesc->BufferDesc.Height = height;

			m_Device->GetInmediateContext()->OMSetRenderTargets(0, 0, 0);

			m_Texture->Release();

			ULONG counter=m_RenderTargetView->Release();

			HRESULT hr = m_SwapChain->ResizeBuffers(m_SwapChainDesc->BufferCount, width, height, m_SwapChainDesc->BufferDesc.Format, m_SwapChainDesc->Flags);
			//HRESULT hr = m_SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

			if (FAILED(hr))
			{
				FatalLog.WriteDXError("_RenderTarget::Resize()", hr);
			}

			ID3D11Texture2D* m_Texture2D11;

			// Get the backbuffer
			pin_ptr<ID3D11Texture2D*> backBuffer = &m_Texture2D11;
			hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)backBuffer);
			if (FAILED(hr))
			{
				FatalLog.WriteDXError("_RenderTarget::Resize. CreateSwapChainRenderTarget", hr);
			}

			m_Texture->Resize(m_Texture2D11);

			OnCreate();

		}
		else
		{
			/*
			CreateSharedTextureForDX9();

			m_backBufferTexture = gcnew _Texture2D(this, m_Texture2D11);
			m_renderTarget = gcnew _RenderTarget(this, m_backBufferTexture);
			*/
		}
		
	}

	void _RenderTarget::OnCreate()
	{
		InternalLog.WriteLine("_RenderTarget::OnCreate");

		pin_ptr<ID3D11RenderTargetView*> renderTargetView = &m_RenderTargetView;

		HRESULT hr = m_Device->GetDevice()->CreateRenderTargetView( m_Texture->m_TextureInferface, NULL, renderTargetView );
		
		if( FAILED( hr ) )
		{
			InternalLog.WriteDXError("_RenderTarget::OnCreate()",hr);
		}
	}

	void _RenderTarget::Clear(float r,float g,float b,float a)
	{
		float ClearColor[4] = { r, g, b, a };
		m_Device->GetInmediateContext()->ClearRenderTargetView( m_RenderTargetView, ClearColor );
	}

	void _RenderTarget::Present()
	{
		if (m_SwapChain == nullptr)
		{
			FatalLog.WriteLine("_RenderTarget::Present. m_SwapChain is null");
		}

		HRESULT hr = m_SwapChain->Present(0, 0);
		if (FAILED(hr))
		{
			FatalLog.WriteDXError("GraphicsDevice::Present", hr);
		}
	}
}
