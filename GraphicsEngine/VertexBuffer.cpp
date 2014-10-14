#include "stdafx.h"

#include "AllHeaders.h"

namespace GraphicsEngineSpace {

	_VertexBuffer::_VertexBuffer(GraphicsDevice^ device,bool dynamic):_Buffer(device,dynamic)
	{
		m_Position = nullptr;
		m_Color = nullptr;
		m_TexCoord = nullptr;
		m_Normal = nullptr;
		m_Tangent = nullptr;

		ResetValues();
	}

	void _VertexBuffer::ResetValues()
	{
		m_NumElements=0;

		m_OffsetForPosition=-1;
		m_OffsetForColor=-1;
		m_OffsetForTexCoord=-1;
		m_OffsetForNormal=-1;
		m_OffsetForTangent=-1;

	}

	void _VertexBuffer::BindPosition(float* p,int numElements,int offset,int semanticSize)
	{
		m_Position=(float *)p;

		m_Count=(int)numElements;

		m_OffsetForPosition=offset;

		// We must start bindind the position for this value be correct
		m_VertexSizeInBytes=semanticSize;
	}

	void _VertexBuffer::BindColor(float* p,int offset,int semanticSize)
	{
		m_Color=(float *)p;

		m_OffsetForColor=offset;

		m_VertexSizeInBytes+=semanticSize;
	}

	void _VertexBuffer::BindNormal(float* p,int offset,int semanticSize)
	{
		m_Normal=(float *)p;

		m_OffsetForNormal=offset;

		m_VertexSizeInBytes+=semanticSize;
	}

	void _VertexBuffer::BindTangent(float* p,int offset,int semanticSize)
	{
		m_Tangent=(float *)p;

		m_OffsetForTangent=offset;

		m_VertexSizeInBytes+=semanticSize;
	}

	void _VertexBuffer::BindTexCoord(float* p,int numChannels,int offset,int semanticSize)
	{
		m_TexCoord=(float *)p;
		m_NumChannels=numChannels;

		m_OffsetForTexCoord=offset;

		m_VertexSizeInBytes+=semanticSize;
	}

	void _VertexBuffer::Update()
	{
		if(m_Buffer)
		{
			m_Buffer->Release();

			m_Buffer=NULL;
		}

		// First we need to know the bufferSize

		m_BufferSizeInBytes=m_VertexSizeInBytes * m_Count;

		// Unique_ptr will free the pointer
		unique_ptr<char> destination(new char[m_BufferSizeInBytes]);
		
		int ByteWidth=m_VertexSizeInBytes;

		int offsetInBytes=0;

		if(m_Position)
		{
			char *destBase=destination.get();

			for(int i=0;i<m_Count;i++)
			{
				float *dest=(float *)destBase;

				*dest++=*m_Position++;
				*dest++=*m_Position++;
				*dest++=*m_Position++;

				destBase+=ByteWidth;
			}
		}

		if(m_Color)
		{
			char *destBase=destination.get();

			destBase+=m_OffsetForColor;

			for(int i=0;i<m_Count;i++)
			{
				float *dest=(float *)destBase;

				*dest++=*m_Color++;
				*dest++=*m_Color++;
				*dest++=*m_Color++;
				*dest++=*m_Color++;

				destBase+=ByteWidth;
			}
		}

		if(m_Normal)
		{
			char *destBase=destination.get();

			destBase+=m_OffsetForNormal;

			for(int i=0;i<m_Count;i++)
			{
				float *dest=(float *)destBase;

				*dest++=*m_Normal++;
				*dest++=*m_Normal++;
				*dest++=*m_Normal++;

				destBase+=ByteWidth;
			}
		}

		if(m_Tangent)
		{
			char *destBase=destination.get();

			destBase+=m_OffsetForTangent;

			for(int i=0;i<m_Count;i++)
			{
				float *dest=(float *)destBase;

				*dest++=*m_Tangent++;

				destBase+=ByteWidth;
			}
		}

		if(m_TexCoord)
		{
			char *destBase=destination.get();

			destBase+=m_OffsetForTexCoord;

			int numChannels=m_NumChannels;

			for(int channel=0;channel<numChannels;channel++)
			{			
				for(int i=0;i<m_Count;i++)
				{
					float *dest=(float *)destBase;

					*dest++=*m_TexCoord++;
					*dest++=*m_TexCoord++;

					destBase+=ByteWidth;
				}
			}
		}

		Create(destination.get(),D3D11_BIND_VERTEX_BUFFER);

	}

	void _VertexBuffer::SetToDevice(int slot)
	{
		UINT stride =VertexSizeInBytes;
		UINT offset = 0;

		pin_ptr<ID3D11Buffer*> vb=&m_Buffer;
		m_Device->GetInmediateContext()->IASetVertexBuffers( slot, 1, vb, &stride, &offset );

	}
}
