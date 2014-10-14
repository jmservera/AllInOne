#include "stdafx.h"

#include "AllHeaders.h"

namespace GraphicsEngineSpace {

	_DepthStencil::_DepthStencil(GraphicsDevice^device, String^ name, int width, int height)
	{
		m_Device=device;
		
		ID3D11Texture2D* texture2D;

		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory( &descDepth, sizeof(descDepth) );
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		pin_ptr<ID3D11Texture2D*> depthStencil=&texture2D;

		HRESULT hr = m_Device->GetDevice()->CreateTexture2D( &descDepth, NULL, depthStencil );
		if( FAILED( hr ) )
		{
			InternalLog.WriteDXError("_DepthStencil::_DepthStencil()",hr);
		}

		m_Texture=gcnew _Texture2D(m_Device,name,texture2D);

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory( &descDSV, sizeof(descDSV) );
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		pin_ptr<ID3D11DepthStencilView*> depthStencilView=&m_DepthStencilView;

		hr = m_Device->GetDevice()->CreateDepthStencilView( texture2D, &descDSV, depthStencilView );
		
		if( FAILED( hr ) )
		{
			InternalLog.WriteDXError("_DepthStencil::_DepthStencil()",hr);
		}

		m_Texture->UpdateSQLInfo("UseFor=" + SQL::ToString("DepthStencil"));
	}

	void _DepthStencil::Destroy()
	{
		if(m_DepthStencilView)
		{
			m_Texture->Destroy();

			m_DepthStencilView->Release();

			m_DepthStencilView=NULL;
		}

	}

	void _DepthStencil::Release()
	{
		if(m_DepthStencilView)
		{
			m_DepthStencilView->Release();

			m_DepthStencilView=NULL;
		}

	}

	void _DepthStencil::Clear()
	{
	    Clear(1.0f,0);
	}

	void _DepthStencil::Clear(float z,int stencil)
	{
	    m_Device->GetInmediateContext()->ClearDepthStencilView( m_DepthStencilView, D3D11_CLEAR_DEPTH, z, stencil );
	}

}
