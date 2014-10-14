#include "stdafx.h"

#include "AllHeaders.h"
/*
#if defined(DEBUG) || defined(PROFILE)
        pTex2D->SetPrivateData( WKPDID_D3DDebugObjectName, sizeof("DDSTextureLoader")-1, "DDSTextureLoader" );
#endif
 */

namespace GraphicsEngineSpace {

	_Texture2D::_Texture2D(GraphicsDevice^ device,String^ name):_Texture(device,name)
	{
		m_TextureDescription = new D3D11_TEXTURE2D_DESC;

		m_TextureInferface = NULL;
	}
	
	// This constructor is used only for the backbuffer
	_Texture2D::_Texture2D(GraphicsDevice^ device, String^ name, ID3D11Texture2D* pBackBuffer) :_Texture(device,name)
	{
		m_Texture=(ID3D11ShaderResourceView*)pBackBuffer;

		m_TextureDescription=new D3D11_TEXTURE2D_DESC;

		m_TextureInferface = NULL;

		GetDescription();

		UpdateSQLInfo("UseFor="+SQL::ToString("BackBuffer"));
	}

	void _Texture2D::Resize(ID3D11Texture2D* pBackBuffer)
	{
		m_Texture = (ID3D11ShaderResourceView*)pBackBuffer;

		GetDescription();

		UpdateSQLInfo(nullptr);
	}

	void _Texture2D::Destroy()
	{
		if(m_TextureDescription)
		{
			delete m_TextureDescription;

			m_TextureDescription=NULL;

		}
		/*
		We do not have to do Release()!!!!!
		if (m_TextureInferface)
		{
			m_TextureInferface->Release();

			m_TextureInferface = NULL;
		}
		*/
		_Texture::Destroy();
	}

	void _Texture2D::Create(int width,int height,DXGIFORMAT format,bool renderTarget)
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));

		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = desc.ArraySize = 1;
		desc.Format = (DXGI_FORMAT)format;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		if(renderTarget) desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags=0;

		pin_ptr<ID3D11ShaderResourceView*> texture=&m_Texture;
		
		ID3D11Texture2D *pTex2D = NULL;

		HRESULT hr=m_Device->GetDevice()->CreateTexture2D( &desc, NULL, &pTex2D );
		if( FAILED( hr ) )
		{
			char tmp[256];
			sprintf(tmp,"GraphicsDevice::Texture2D.Create(%d,%d,%s)",width,height,(renderTarget ? "RT": ""));
			FatalLog.WriteDXError(tmp,hr);
			
			m_Texture=NULL;

		}
	
		pTex2D->GetDesc( m_TextureDescription );

		m_TextureInferface=pTex2D;

		//create shader resource view:
		D3D11_SHADER_RESOURCE_VIEW_DESC srvdesc; 
		ZeroMemory( &srvdesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC) );
		srvdesc.Format = m_TextureDescription->Format;
		srvdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//srvdesc.Texture2D.MipLevels = texdesc.MipLevels;
		//srvdesc.Texture2D.MostDetailedMip = texdesc.MipLevels-1;
		srvdesc.Texture2D.MipLevels =(renderTarget ? 1 : -1);
		srvdesc.Texture2D.MostDetailedMip = 0;
 
		hr =m_Device->GetDevice()->CreateShaderResourceView( pTex2D, &srvdesc, texture );
		if( FAILED( hr ) )
		{
			FatalLog.WriteDXError("GraphicsDevice::Texture2D.CreateShaderResourceView",hr);
			
			m_Texture=NULL;

		}

		// This Release for the CreateTexture2D
		pTex2D->Release();

		UpdateSQLInfo("UseFor=" + SQL::ToString((renderTarget ? "RenderTarget" : "Texture")));
	}

	void _Texture2D::UpdateSQLInfo(String^ cad)
	{
		if (cad != nullptr) cad += ",";

		SQL::Insert("UPDATE Texture Set " + cad + "Width=" + m_TextureDescription->Width + ",Height=" + m_TextureDescription->Height + " WHERE IDTexture = " + m_ID);

	}

	void _Texture2D::LoadFromFile(String^ fullPath)
	{
		// Transform to pointer
		CString wideChar(fullPath);

		//const TCHAR* x = (LPCTSTR) wideChar;
		
		InternalLog.WriteLine(CString(L"Loading:")+wideChar);

		// Load the Texture
		pin_ptr<ID3D11ShaderResourceView*> texture=&m_Texture;

		//HRESULT hr = D3DX11CreateShaderResourceViewFromFile( m_Device->GetDevice(), wideChar, NULL, NULL, texture, NULL );

		ID3D11Resource *pTex2D = NULL;
 
		HRESULT hr =D3DX11CreateTextureFromFile( m_Device->GetDevice(), wideChar, NULL, NULL, &pTex2D, NULL );
 
		if( FAILED( hr ) )
		{
			FatalLog.WriteDXError(CString(L"GraphicsDevice::Texture2D.D3DX11CreateTextureFromFile: ")+wideChar,hr);
			
			m_Texture=NULL;

		}
		ID3D11Texture2D *pTex2D_tmp = NULL;
		pTex2D->QueryInterface( __uuidof(ID3D11Texture2D), (VOID**)&pTex2D_tmp );
		pTex2D_tmp->GetDesc( m_TextureDescription );

		m_TextureInferface=pTex2D_tmp;

		//create shader resource view:
		D3D11_SHADER_RESOURCE_VIEW_DESC srvdesc; 
		ZeroMemory( &srvdesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC) );
		srvdesc.Format = m_TextureDescription->Format;
		srvdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//srvdesc.Texture2D.MipLevels = texdesc.MipLevels;
		//srvdesc.Texture2D.MostDetailedMip = texdesc.MipLevels-1;
		srvdesc.Texture2D.MipLevels =-1;
		srvdesc.Texture2D.MostDetailedMip = 0;
 
		hr =m_Device->GetDevice()->CreateShaderResourceView( pTex2D, &srvdesc, texture );
 
		if( FAILED( hr ) )
		{
			FatalLog.WriteDXError("GraphicsDevice::Texture2D.CreateShaderResourceView",hr);
			
			m_Texture=NULL;

		}

		// This Release for the QueryInterface
		pTex2D->Release();
		// This Release for the D3DX11CreateTextureFromFile
		pTex2D->Release();

		UpdateSQLInfo("UseFor=" + SQL::ToString("Texture") + ",Path=" + SQL::ToString(fullPath));

	}

	void _Texture2D::GetDescription()
	{
		ULONG counter;

		m_Texture->GetDesc(m_Description);

		ID3D11Texture2D *pTextureInterface = 0;
		m_Texture->QueryInterface(__uuidof(ID3D11Texture2D),(void**)&pTextureInterface);
		pTextureInterface->GetDesc(m_TextureDescription);
		
		// TODO: Funciona igual poniendo esto que quitándolo.... :(
		//counter = pTextureInterface->Release();

		counter = m_Texture->Release();

		m_TextureInferface=pTextureInterface;
	}

	void _Texture2D::SetToDevice(UINT32 channel)
	{
		pin_ptr<ID3D11ShaderResourceView*> texture=&m_Texture;
		m_Device->GetInmediateContext()->PSSetShaderResources(channel, 1, texture );
	}

	void _Texture2D::Save(String^ fullPath)
	{
		CString fileName=CString(fullPath);

		//pin_ptr<ID3D11ShaderResourceView*> texture=&m_Texture;
		HRESULT hr=D3DX11SaveTextureToFile(m_Device->GetInmediateContext(),m_TextureInferface,D3DX11_IFF_PNG,fileName);

		if(FAILED(hr))
		{
			InternalLog.WriteDXError("Texture2D::Save."+fileName,hr);
		}
	}

	BYTE* _Texture2D::MapData([Out] int% rowPitch,bool canWrite)
	{ 
		// We need to copy the texture to a resource that can be readed by the CPU

		D3D11_TEXTURE2D_DESC td;
		td.ArraySize = 1;
		td.BindFlags = 0;
		td.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		
		if(canWrite) td.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
		
		td.Format = m_TextureDescription->Format;
		td.Height = Height;
		td.Width  = Width;
		td.MipLevels = 1;
		td.MiscFlags = 0;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_STAGING;
		pin_ptr<ID3D11Texture2D*> texture=&m_TemporalTextureForData;

		HRESULT hr=m_Device->GetDevice()->CreateTexture2D(&td, 0,texture);
		if(FAILED(hr))
		{
			FatalLog.WriteDXError("Texture2D::MapData.CreateTexture2D",hr);
		}
		// We want to access only to the first level of the texture ( the original texture, not the mipmapped)

		m_Device->GetInmediateContext()->CopySubresourceRegion(m_TemporalTextureForData,0,0,0,0,m_TextureInferface,0,NULL);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		
		hr=m_Device->GetInmediateContext()->Map(m_TemporalTextureForData, 0, (canWrite ? D3D11_MAP_READ_WRITE : D3D11_MAP_READ), 0, &mappedResource);
		if(FAILED(hr))
		{
			FatalLog.WriteDXError("Texture2D::MapData.Map",hr);
		}
		
		rowPitch=mappedResource.RowPitch;

		BYTE* mappedData = reinterpret_cast<BYTE*>(mappedResource.pData);

		return mappedData;
	}

	void _Texture2D::Unmap(bool copyBack)
	{
		m_Device->GetInmediateContext()->Unmap(m_TemporalTextureForData, 0);

		if(copyBack)
		{
			m_Device->GetInmediateContext()->CopySubresourceRegion(m_TextureInferface,0,0,0,0,m_TemporalTextureForData,0,NULL);
		}

		m_TemporalTextureForData->Release();
	}
}
