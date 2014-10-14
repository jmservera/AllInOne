#include "stdafx.h"

#include "AllHeaders.h"

namespace GraphicsEngineSpace {

	Int32 GlobalTextureID=0;

	_Texture::_Texture(GraphicsDevice^ device,String^ name)
	{
		GlobalTextureID++;

		m_ID = GlobalTextureID;

		m_Device=device;

		m_Description=new D3D11_SHADER_RESOURCE_VIEW_DESC;

		SQL::Insert("INSERT INTO Texture(IDTexture,IDDevice,NTexture) VALUES(" + m_ID + "," + m_Device->ID + "," + SQL::ToString(name) + ")");

		// Add this texture to the textures list
		m_Device->m_Textures.Add(this);
	}

	_Texture::~_Texture()
	{
		Destroy();

		m_Device->m_Textures.Remove(this);
	}

	void _Texture::Destroy()
	{
		if(m_Description!=NULL)
		{
			delete m_Description;

			m_Description=NULL;
		}
		
		if(m_Texture)
		{
			ULONG counter = m_Texture->Release();

			SQL::Insert("UPDATE Texture Set Destroyed=Now(),RefCounter=" + counter+" WHERE IDTexture = " + m_ID);

			m_Texture=NULL;
		}
	}

	void _Texture::Release()
	{
		if(m_Texture)
		{
			m_Texture->Release();

			m_Texture=NULL;
		}
	}

	void _Texture::SetToDevice(UINT32 channel)
	{
		pin_ptr<ID3D11ShaderResourceView*> texture = &m_Texture;
		m_Device->GetInmediateContext()->PSSetShaderResources(channel, 1, texture);

	}
}
