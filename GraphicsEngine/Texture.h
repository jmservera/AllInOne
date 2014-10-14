#pragma once

namespace GraphicsEngineSpace {

	public ref class _Texture
	{
	protected:

		GraphicsDevice^				m_Device;

		ID3D11ShaderResourceView*	m_Texture;

		D3D11_SHADER_RESOURCE_VIEW_DESC *m_Description;

		Int32 m_ID;

	internal:

		ID3D11ShaderResourceView*	GetTexture() { return m_Texture; }
		void SetTexture(ID3D11ShaderResourceView* v) { m_Texture = v; }

	public:

		_Texture(GraphicsDevice^, String^ name);
		~_Texture();

		virtual void SetToDevice(UINT32 channel);

		virtual void Destroy();

		void Release();

		Int32 GetID() { return m_ID; }
	
	};

}
