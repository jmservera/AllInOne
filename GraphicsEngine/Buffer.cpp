#include "stdafx.h"

#include "AllHeaders.h"

namespace GraphicsEngineSpace {

	_Buffer::_Buffer(GraphicsDevice^ device)
	{
		Initialize(device,false);
	}

	_Buffer::_Buffer(GraphicsDevice^ device,bool dynamic)
	{
		Initialize(device,dynamic);
	}

	void _Buffer::Initialize(GraphicsDevice^ device,bool dynamic)
	{
		m_Dynamic=dynamic;

		m_Device=device;

		m_Buffer=NULL;

		m_Description=new D3D11_BUFFER_DESC;

		// We need to create enough room for the possible incoming data

		m_Data=(char *)new float[MaxData];

	}

	_Buffer::~_Buffer()
	{
		Destroy();
	}

	void _Buffer::Destroy()
	{
		if(m_Buffer)
		{
			m_Buffer->Release();
			m_Buffer=NULL;
		}

		if(m_Description)
		{
			delete m_Description;

			m_Description=NULL;
		}

		if(m_Data)
		{
			delete[] m_Data;

			m_Data=NULL;
		}
	}

	void _Buffer::Create(_D3D11_BIND_FLAG bindFlag,int bufferSizeInBytes)
	{
		m_BufferSizeInBytes=bufferSizeInBytes;
		Create(NULL,(D3D11_BIND_FLAG)bindFlag);
	}

	void _Buffer::UpdateData(void *data)
	{
		m_Device->GetInmediateContext()->UpdateSubresource( m_Buffer, 0, NULL, data, 0, 0 );
	}

	void _Buffer::BeginAddData()
	{
		m_BufferSizeForAddData=0;
	}

	void _Buffer::AddFloat(float data)
	{
		AddData(&data, sizeof(float));
	}

	void _Buffer::Add(Vector3% data)
	{
		pin_ptr<float> pointer = &data.X;

		AddData(pointer, sizeof(float)* 3);
		
		float tmp = 0;

		AddData(&tmp, sizeof(float));
	}

	void _Buffer::Add(Vector4% data)
	{
		pin_ptr<float> pointer = &data.X;

		AddData(pointer, sizeof(float)* 4);
	}

	void _Buffer::AddMatrix(Matrix% data)
	{
		pin_ptr<float> pointer = &data.M11;

		AddData(pointer, sizeof(float)* 4 * 4);
	}

	void _Buffer::AddData(void *data, int dataSize)
	{
		memcpy(&m_Data[m_BufferSizeForAddData],data,dataSize);

		m_BufferSizeForAddData+=dataSize;

		if(m_BufferSizeForAddData>=MaxData)
		{
			FatalLog.WriteLine("_Buffer::AddData. Not enough room in Data!!");
		}
	}

	void _Buffer::EndAddData()
	{
		// if the size it's not the same, destroy the old buffer
		if(m_BufferSizeForAddData!=m_BufferSizeInBytes)
		{
			if(m_Buffer)
			{
				m_Buffer->Release();
				m_Buffer=NULL;
			}

			m_BufferSizeInBytes=m_BufferSizeForAddData;

			Create(m_Data,D3D11_BIND_CONSTANT_BUFFER);
		}
		else
		{
			// Same size!! Do not need to destroy it!!
			// Take care that you can only UpdateSubResource using D3D11_USAGE_DEFAULT.
			// IT WILL NOT WORK WITH D3D11_USAGE_DYNAMIC
			UpdateData(m_Data);
		}

	}

	HRESULT _Buffer::Create(void *vertices,D3D11_BIND_FLAG bindFlag)
	{
		ZeroMemory( m_Description, sizeof(D3D11_BUFFER_DESC) );
		m_Description->Usage = D3D11_USAGE_DEFAULT;
		// The buffer must be multiple of 16
		m_Description->ByteWidth = ((m_BufferSizeInBytes/16)+1)*16;
		m_Description->BindFlags = bindFlag;
		m_Description->CPUAccessFlags = 0;

		if(m_Dynamic)
		{
			m_Description->Usage = D3D11_USAGE_DYNAMIC;
			m_Description->CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory( &InitData, sizeof(InitData) );
		InitData.pSysMem = vertices;

		pin_ptr<ID3D11Buffer*> buffer=&m_Buffer;

		HRESULT hr = m_Device->GetDevice()->CreateBuffer( m_Description, &InitData, buffer );
		if( FAILED( hr ) )
			FatalLog.WriteDXError("_Buffer::Create()",hr);

		return hr;
	}

	void _Buffer::SetToDevice(int numBuffer)
	{
		pin_ptr<ID3D11Buffer*> buffer=&m_Buffer;
		m_Device->GetInmediateContext()->VSSetConstantBuffers( numBuffer, 1, buffer );
		m_Device->GetInmediateContext()->PSSetConstantBuffers( numBuffer, 1, buffer );
	}

}
