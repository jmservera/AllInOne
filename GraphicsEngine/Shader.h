#pragma once

namespace GraphicsEngineSpace {

	public ref class _Shader
	{
	private:

		GraphicsDevice^			m_Device;

		ID3DBlob*	m_Blob;
	
		ID3D11VertexShader*     m_VertexShader;
		ID3D11PixelShader*		m_PixelShader;
		
		UInt32 m_id;

		// The automatically created InputLayout for this shader
		_InputLayout ^m_InputLayout;

	internal:

		static HRESULT CompileShaderFromMemory( LPCSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut );


	private:
		
		void CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice);

	public:

		_Shader(GraphicsDevice^, UInt32 id);

		void SetToDevice();
		void SetDefaultInputLayoutToDevice();
		void Destroy();
		void CompileFromFile(String^ path);
		void Compile(String^ shader);
		void Compile(void *);

		UInt32 GetID();
	};

}