#pragma once

namespace GraphicsEngineSpace {

	public ref class _DepthStencil
	{
	protected:

		GraphicsDevice^				m_Device;

		_Texture2D^					m_Texture;

		ID3D11DepthStencilView*		m_DepthStencilView;

	internal:

		ID3D11DepthStencilView* GetInternal() { return  m_DepthStencilView; }


	public:

		_DepthStencil(GraphicsDevice^, String^ name, int w, int h);

		_Texture2D^ GetTexture() { return m_Texture; }
			
		void Destroy();

		void Release();

		void Clear();

		void Clear(float z,int stencil);
	};

}
