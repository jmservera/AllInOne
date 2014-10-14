#pragma once

namespace GraphicsEngineSpace {

	public ref class _RenderTarget
	{
	protected:

		GraphicsDevice^				m_Device;

		_Texture2D^					m_Texture;
		
		// If this RenderTarget has a SwapChain...
		IDXGISwapChain*				m_SwapChain;

		DXGI_SWAP_CHAIN_DESC*		m_SwapChainDesc;

	internal:
		
		bool						m_IsDX9BackBuffer;

		ID3D11RenderTargetView*		m_RenderTargetView;

		void OnCreate();

		_RenderTarget(GraphicsDevice^, _Texture2D^, IDXGISwapChain*	m_SwapChain, DXGI_SWAP_CHAIN_DESC*		m_SwapChainDesc);

	public:

		property bool IsDX9BackBuffer { bool get() { return m_IsDX9BackBuffer; }}
		property bool IsSwapChain { bool get() { return m_SwapChain!=nullptr; }}

		_Texture2D^ GetTexture() { return m_Texture; };

		_RenderTarget(GraphicsDevice^, _Texture2D^);

		void Destroy();

		void Resize(int width, int height);

		void Clear(float r,float g,float b,float a);

		void Present();
	};

}
