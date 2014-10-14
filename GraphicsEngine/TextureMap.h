#pragma once

namespace GraphicsEngineSpace {

	public ref class TextureMap
	{
	internal:

		const char*		m_VarsCode;
		const char*		m_VertexShaderOutput;
		const char*		m_VertexShaderCode;
		const char*		m_PixelShaderCode;
		const char*		m_InternalName;
	public:

		TextureMap();
		
		virtual void SetToDevice(int channel);

		property bool IsDiffuse;
		property bool IsSpecular;

		property const char* VarsCode { const char* get() { return m_VarsCode; } }
		property const char* VertexShaderOutput { const char* get() { return m_VertexShaderOutput; } }
		property const char* VertexShaderCode { const char* get() { return m_VertexShaderCode; } }
		property const char* PixelShaderCode { const char* get() { return m_PixelShaderCode; } }
		property const char* InternalName { const char* get() { return m_InternalName; } }

		property int	Channel;
	};
}
