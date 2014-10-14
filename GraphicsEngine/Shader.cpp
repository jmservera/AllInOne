#include "stdafx.h"

#include "AllHeaders.h"
#include <vcclr.h>
using namespace System::Runtime::InteropServices;
using namespace System::IO;

namespace GraphicsEngineSpace {

	_Shader::_Shader(GraphicsDevice^ device,UInt32 id)
	{
		m_Device=device;

		m_VertexShader=NULL;

		m_PixelShader=NULL;

		m_id = id;
	}
	
	UInt32 _Shader::GetID() 
	{ 
		return m_id; 
	}

	void _Shader::Destroy()
	{
		InternalLog.WriteLine("_Shader::Destroy");

		if (m_VertexShader)
		{
			m_VertexShader->Release();

			m_VertexShader=NULL;
		}

		if(m_PixelShader)
		{
			m_PixelShader->Release();

			m_PixelShader=NULL;
		}
		// The inputLayout must not be destroyed, because it colud be used by another shader.
	}
	
	void _Shader::SetDefaultInputLayoutToDevice()
	{
		m_InputLayout->SetToDevice();
	}

	HRESULT _Shader::CompileShaderFromMemory( LPCSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
	{
		HRESULT hr = S_OK;

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	#if defined( DEBUG ) || defined( _DEBUG )
		// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DCOMPILE_DEBUG;
	#endif
	
		ID3DBlob* pErrorBlob;
		hr = D3DX11CompileFromMemory( szFileName,strlen(szFileName),NULL,
			NULL, NULL, szEntryPoint, szShaderModel, 
			dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL );
		if( FAILED(hr) )
		{
			InternalLog.WriteLine(szFileName);

			if( pErrorBlob != NULL )
				OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
			if( pErrorBlob ) pErrorBlob->Release();
			return hr;
		}
		if( pErrorBlob ) pErrorBlob->Release();

		return S_OK;
	}

	void _Shader::CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice)
	{
		_InputLayout ^ inputLayout = gcnew _InputLayout(m_Device);

		// Reflect shader info
		ID3D11ShaderReflection* pVertexShaderReflection = NULL;
		if (FAILED(D3DReflect(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
		{
			return;
		}

		// Get shader info
		D3D11_SHADER_DESC shaderDesc;
		pVertexShaderReflection->GetDesc(&shaderDesc);

		// Read input layout description from shader info
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
		for (int i = 0; i< shaderDesc.InputParameters; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

			// fill out input element desc
			//D3D11_INPUT_ELEMENT_DESC elementDesc;
			InputLayoutElement^ elementDesc = gcnew InputLayoutElement();
			elementDesc->SemanticName = (InputLayoutSemantic)_InputLayout::GetSemanticFromName(gcnew String(paramDesc.SemanticName));
			elementDesc->SemanticIndex = paramDesc.SemanticIndex;
			elementDesc->InputSlot = 0;
			elementDesc->AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			elementDesc->InputSlot = D3D11_INPUT_PER_VERTEX_DATA;
			//elementDesc->InstanceDataStepRate = 0;

			// determine DXGI format
			if (paramDesc.Mask == 1)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc->Format = DXGI_FORMAT_R32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc->Format = DXGI_FORMAT_R32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc->Format = DXGI_FORMAT_R32_FLOAT;
			}
			else if (paramDesc.Mask <= 3)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc->Format = DXGI_FORMAT_R32G32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc->Format = DXGI_FORMAT_R32G32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc->Format = DXGI_FORMAT_R32G32_FLOAT;
			}
			else if (paramDesc.Mask <= 7)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc->Format = DXGI_FORMAT_R32G32B32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc->Format = DXGI_FORMAT_R32G32B32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else if (paramDesc.Mask <= 15)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc->Format = DXGI_FORMAT_R32G32B32A32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc->Format = DXGI_FORMAT_R32G32B32A32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc->Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}
			inputLayout->Items.Add(elementDesc);
			//save element desc
			//inputLayoutDesc.push_back(elementDesc);
		}
		m_InputLayout = m_Device->AddInputLayoutIfNeeded(inputLayout);

		//Free allocation shader reflection memory
		pVertexShaderReflection->Release();
	}

	void _Shader::SetToDevice()
	{
		/*
		for each(TextureMap^ map in m_MaterialLayer->m_Maps)
		{
			map->SetToDevice(map->Channel);
			
		}
		*/
		m_Device->GetInmediateContext()->VSSetShader( m_VertexShader, NULL, 0 );
		m_Device->GetInmediateContext()->PSSetShader( m_PixelShader, NULL, 0 );
	}

	void _Shader::CompileFromFile(String^ path)
	{
		InternalLog.WriteLine("_Shader::CompileFromFile" + path);

		StreamReader^ reader = gcnew StreamReader(path);
		String^ data=reader->ReadToEnd();
		reader->Close();

		Compile(data);

	}
	void _Shader::Compile(String^ shader)
	{
		
		// Transform to pointer
		CString wideChar(shader);
		const TCHAR *w=(LPCTSTR) wideChar;
		char *shaderASCII=UnicodeToAnsi((BSTR)(LPCTSTR) wideChar);

		Compile((void *)shaderASCII);

		delete[] shaderASCII;
	}

	void _Shader::Compile(void *shaderASCIIParam)
	{
		const char* shaderASCII = static_cast<const char*>(shaderASCIIParam);

		int l=(int)strlen(shaderASCII);

		pin_ptr<ID3DBlob *>b=&m_Blob;
	    HRESULT hr = CompileShaderFromMemory(shaderASCII, "VS", "vs_4_0", b );
		if( FAILED( hr ) )
		{
			InternalLog.WriteDXError("CompileShaderFromMemory VS",hr);
		}

		// Create the vertex shader
		pin_ptr<ID3D11VertexShader *>p=&m_VertexShader;
		hr = m_Device->GetDevice()->CreateVertexShader( m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), NULL, p );
		if( FAILED( hr ) )
		{	
			InternalLog.WriteDXError("CreateVertexShader",hr);
			m_Blob->Release();
		}

		CreateInputLayoutDescFromVertexShaderSignature(m_Blob, m_Device->GetDevice());

		m_Blob->Release();

	    hr = CompileShaderFromMemory( shaderASCII, "PS", "ps_4_0", b );
		if( FAILED( hr ) )
		{
			InternalLog.WriteDXError("CompileShaderFromMemory PS",hr);
		}

		// Create the pixel shader
		pin_ptr<ID3D11PixelShader *>pp=&m_PixelShader;
		hr = m_Device->GetDevice()->CreatePixelShader( m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), NULL, pp );
		if( FAILED( hr ) )
		{	
			InternalLog.WriteDXError("CreatePixelShader",hr);
			m_Blob->Release();
		}
		
		m_Blob->Release();

	}
}
