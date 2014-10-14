#include "stdafx.h"

#include "AllHeaders.h"

namespace GraphicsEngineSpace {

	_InputLayout::_InputLayout(GraphicsDevice^ device)
	{
		m_Device=device;

		m_Description=NULL;
		m_Layout=NULL;
		m_NumElements=0;

		m_SimpleVertexshader=new char[1024];
	}

	void _InputLayout::Destroy()
	{
		if(m_Description!=NULL)
		{
			delete[] m_Description;

			m_Description=NULL;
		}

		if(m_Layout!=NULL)
		{
			m_Layout->Release();

			m_Layout=NULL;
		}

		m_NumElements=0;

		if(m_SimpleVertexshader)
		{
			delete m_SimpleVertexshader;

			m_SimpleVertexshader=NULL;
		}
	}

	//--------------------------------------------------------------------------------------
	// Set the Input layout to the Device
	//--------------------------------------------------------------------------------------
	void _InputLayout::SetToDevice()
	{
		 m_Device->GetInmediateContext()->IASetInputLayout( m_Layout );
	}

	// Used for create the simplevertexshader. VERY IMPORTANT THAT THE ORDER MATCH THE InputLayoutSemantic ENUM
	char* semanticNames[]={ "POSITION","COLOR","NORMAL","TANGENT","TEXCOORD" };

	int _InputLayout::GetSemanticFromName(String^ name)
	{
		if (name == "POSITION") return 0;
		if (name == "COLOR") return 1;
		if (name == "NORMAL") return 2;
		if (name == "TANGENT") return 3;
		if (name == "TEXCOORD") return 4;

		return -1;
	}

	//--------------------------------------------------------------------------------------
	// Builds the Input layout based on the Items field
	//--------------------------------------------------------------------------------------
	void _InputLayout::Build()
	{
		char tmpforString[100];
		
		m_NumElements = Items.Count;

		m_Description=new D3D11_INPUT_ELEMENT_DESC[m_NumElements];

		int i=0;
		
		char *simpleVertexshader=m_SimpleVertexshader;

		// Reset the simpleVertexshader
		strcpy(simpleVertexshader,"float4 VS( ");

		for each(InputLayoutElement^ il in Items)
		{
			m_Description[i].SemanticName=semanticNames[(int)il->SemanticName];
			m_Description[i].SemanticIndex=il->SemanticIndex;
			m_Description[i].Format=(DXGI_FORMAT)il->Format;
			m_Description[i].InputSlot=il->InputSlot;
			m_Description[i].AlignedByteOffset=il->AlignedByteOffset;
			m_Description[i].InputSlotClass=D3D11_INPUT_PER_VERTEX_DATA;
			m_Description[i].InstanceDataStepRate=0;

			int slot=m_Description[i].InputSlot;

			switch(il->SemanticName)
			{
				case InputLayoutSemantic::Position:
					sprintf(tmpforString,"float4 Pos%d : POSITION%d",slot,slot);
					break;
				case InputLayoutSemantic::Color:
					sprintf(tmpforString,"float4 color%d : COLOR%d",slot,slot);
					break;
				case InputLayoutSemantic::Normal:
					sprintf(tmpforString,"float3 normal%d : NORMAL%d",slot,slot);
					break;
				case InputLayoutSemantic::Tangent:
					sprintf(tmpforString, "float3 tangent%d : TANGENT%d", slot, slot);
					break;
				case InputLayoutSemantic::TexCoord:
					sprintf(tmpforString,"float2 texCoord%d : TEXCOORD%d",slot,slot);
					break;
			}


			strcat(simpleVertexshader,tmpforString);

			if(i!=m_NumElements-1) strcat(simpleVertexshader,",");

			i++;
		}

	}

	//--------------------------------------------------------------------------------------
	// Compares if two InputLayout match
	//--------------------------------------------------------------------------------------
	bool _InputLayout::CompareWith(_InputLayout^ inputLayout)
	{
		int numElementsThisObject = GetNumElements();
		
		// We can not use GetNumElements() because Build has not been called yet!!

		int numElementsNewObject = inputLayout->Items.Count;

		if (numElementsThisObject == numElementsNewObject)
		{
			int index = 0;

			for each (InputLayoutElement^ element in Items)
			{
				if (!element->CompareWith(inputLayout->Items[index++])) return false;
			}

			return true;
		}

		return false;
	}


	//--------------------------------------------------------------------------------------
	// Builds & Creates the InputLayout
	//--------------------------------------------------------------------------------------
	void _InputLayout::Update()
	{
		ID3D11VertexShader*     g_pVertexShader = NULL;

		char *simpleVertexshader=m_SimpleVertexshader;

		Build();

		strcat(simpleVertexshader," ) : SV_POSITION\n { return Pos0; };");

		// I need to create a very basic vertex shader for the signature

		// Compile the vertex shader
		ID3DBlob* pVSBlob = NULL;
		HRESULT hr = _Shader::CompileShaderFromMemory( simpleVertexshader, "VS", "vs_4_0", &pVSBlob );
		if( FAILED( hr ) )
		{
			InternalLog.WriteDXError("InputLayout.CompileShaderFromMemory",hr);
		}

		// Create the vertex shader
		hr = m_Device->GetDevice()->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader );
		if( FAILED( hr ) )
		{	
			InternalLog.WriteDXError("InputLayout.CreateVertexShader",hr);
			pVSBlob->Release();
		}

		// Create the input layout
		pin_ptr<ID3D11InputLayout*> layout=&m_Layout;
		hr = m_Device->GetDevice()->CreateInputLayout( m_Description, m_NumElements,pVSBlob->GetBufferPointer(),
                                          pVSBlob->GetBufferSize(), layout );

		if(FAILED(hr))
		{
			InternalLog.WriteLine(simpleVertexshader);
			InternalLog.WriteDXError("InputLayout.CreateInputLayout",hr);
		}
		pVSBlob->Release();
		g_pVertexShader->Release();
		
		InternalLog.WriteLine("Created a new Layout:");
		
		InternalLog.WriteLine(simpleVertexshader);
		

	}
}
