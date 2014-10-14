#include "stdafx.h"

#include "AllHeaders.h"

namespace GraphicsEngineSpace {

	_IndexBuffer::_IndexBuffer(GraphicsDevice^ device):_Buffer(device)
	{
	}

	void _IndexBuffer::BindIndices(UInt32 *indices,int count)
	{
		m_Indices=indices;
		m_Count=count;
	}

	void _IndexBuffer::Update()
	{
		if(m_Buffer)
		{
			m_Buffer->Release();

			m_Buffer=NULL;
		}

		m_BufferSizeInBytes=(Use32BitIndices ? 4 : 2) * m_Count;

		if(Use32BitIndices)
		{
			Create(m_Indices,D3D11_BIND_INDEX_BUFFER);
		}
		else
		{
			unique_ptr<char> destination(new char[BufferSizeInBytes]);
		
			char *destBase=destination.get();

			WORD *destWord=(WORD *)destBase;

			UINT* p=m_Indices;

			for(int i=0;i<m_Count;i++)
			{
				*destWord++=*(WORD *)p;

				p++;
			}

			Create(destBase,D3D11_BIND_INDEX_BUFFER);
		}
		

	}


	void _IndexBuffer::SetToDevice(int offsetToFirstIndex)
	{
		pin_ptr<ID3D11Buffer> ib=m_Buffer;

		m_Device->GetInmediateContext()->IASetIndexBuffer(ib, (Use32BitIndices ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT), offsetToFirstIndex );
	}
}
