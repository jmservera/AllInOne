#include "stdafx.h"

#include "AllHeaders.h"
/*
#if defined(DEBUG) || defined(PROFILE)
        pTex2D->SetPrivateData( WKPDID_D3DDebugObjectName, sizeof("DDSTextureLoader")-1, "DDSTextureLoader" );
#endif
 */

namespace GraphicsEngineSpace {
	/*
	BlendDescription::BlendDescription()
	{
		RenderTarget=gcnew List<D3D11RENDER_TARGET_BLEND_DESC^>();

		for(int i=0;i<8;i++)
		{
			RenderTarget->Add(gcnew D3D11RENDER_TARGET_BLEND_DESC());
		}

		AlphaToCoverageEnable=false;
		IndependentBlendEnable=false;

		for(int i=0;i<8;i++)
		{
			RenderTarget[i]->BlendEnable=false;
			RenderTarget[i]->SrcBlend=D3D11BLEND::D3D11_BLEND_ONE;
			RenderTarget[i]->DestBlend=D3D11BLEND::D3D11_BLEND_ZERO;
			RenderTarget[i]->BlendOp=D3D11BLEND_OP::D3D11_BLEND_OP_ADD;
			RenderTarget[i]->SrcBlendAlpha=D3D11BLEND::D3D11_BLEND_ONE;
			RenderTarget[i]->DestBlendAlpha=D3D11BLEND::D3D11_BLEND_ZERO;
			RenderTarget[i]->BlendOpAlpha=D3D11BLEND_OP::D3D11_BLEND_OP_ADD;
			RenderTarget[i]->RenderTargetWriteMask=D3D11COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;
		}
	}	
	*/
	_BlendingState::_BlendingState(GraphicsDevice^ device)
	{
		m_Device=device;

		m_Blending=new D3D11_BLEND_DESC;

		m_State=NULL;

		// Default values
		/*
		memset(m_Blending, 0, sizeof(D3D11_BLEND_DESC));

		m_Blending->AlphaToCoverageEnable=FALSE;
		m_Blending->IndependentBlendEnable=FALSE;

		for(int i=0;i<8;i++)
		{
			m_Blending->RenderTarget[i].BlendEnable=FALSE;
			m_Blending->RenderTarget[i].SrcBlend=D3D11_BLEND_ONE;
			m_Blending->RenderTarget[i].DestBlend=D3D11_BLEND_ZERO;
			m_Blending->RenderTarget[i].BlendOp=D3D11_BLEND_OP_ADD;
			m_Blending->RenderTarget[i].SrcBlendAlpha=D3D11_BLEND_ONE;
			m_Blending->RenderTarget[i].DestBlendAlpha=D3D11_BLEND_ZERO;
			m_Blending->RenderTarget[i].BlendOpAlpha=D3D11_BLEND_OP_ADD;
			m_Blending->RenderTarget[i].RenderTargetWriteMask=D3D11_COLOR_WRITE_ENABLE_ALL;
		}
		*/
	}

	_BlendingState::~_BlendingState()
	{
		Destroy();
	}
	
	void _BlendingState::Convert(BlendDescription^ item,D3D11_BLEND_DESC *dest)
	{
		// We need to transform form BlendDescription to BlendDesc

		dest->AlphaToCoverageEnable=item->AlphaToCoverageEnable;
		dest->IndependentBlendEnable=item->IndependentBlendEnable;

        for (int i = 0; i < 8; i++)
        {
			D3D11_RENDER_TARGET_BLEND_DESC *d=&dest->RenderTarget[i];

			d->BlendEnable=item->RenderTarget[i]->BlendEnable;
			d->SrcBlend=(D3D11_BLEND)item->RenderTarget[i]->SrcBlend;
			d->DestBlend=(D3D11_BLEND)item->RenderTarget[i]->DestBlend;
			d->BlendOp=(D3D11_BLEND_OP)item->RenderTarget[i]->BlendOp;
			d->SrcBlendAlpha=(D3D11_BLEND)item->RenderTarget[i]->SrcBlendAlpha;
			d->DestBlendAlpha=(D3D11_BLEND)item->RenderTarget[i]->DestBlendAlpha;
			d->BlendOpAlpha=(D3D11_BLEND_OP)item->RenderTarget[i]->BlendOpAlpha;
			d->RenderTargetWriteMask=(UINT8)item->RenderTarget[i]->RenderTargetWriteMask;
        }

	}

	bool _BlendingState::CompareIftheSame(D3D11_BLEND_DESC *dest)
	{
		// Compare the values to check if it's the same

		if(memcmp(dest,m_Blending,sizeof(D3D11_BLEND_DESC))==0) return true;

		return false;
	}

	void _BlendingState::Create(D3D11_BLEND_DESC *dest)
	{
		memcpy(m_Blending,dest,sizeof(D3D11_BLEND_DESC));

		pin_ptr<ID3D11BlendState*> state=&m_State;

		HRESULT hr = m_Device->GetDevice()->CreateBlendState(m_Blending, state);
		
		if(FAILED(hr))
		{
			FatalLog.WriteDXError("_BlendingState::Create",hr);
		}
	}

	void _BlendingState::SetToDevice(float* blendFactor,UINT32 sampleMask)
	{
		m_Device->GetInmediateContext()->OMSetBlendState(m_State,blendFactor,sampleMask);
	}

	void _BlendingState::Destroy()
	{
		if(m_Blending)
		{
			delete m_Blending;
			m_Blending=NULL;
		}

		if(m_State)
		{
			m_State->Release();

			m_State=NULL;
		}

	}

	//--------------------------------------------------------------------------------------------------------------------------

	_RasterizerState::_RasterizerState(GraphicsDevice^ device)
	{
		m_Device=device;

		m_Rasterizer=new D3D11_RASTERIZER_DESC;

		m_State=NULL;

		ZeroMemory(m_Rasterizer, sizeof(D3D11_RASTERIZER_DESC));

		
	}

	void _RasterizerState::CreateDefault()
	{
		memset(m_Rasterizer, 0, sizeof(D3D11_RASTERIZER_DESC));

		m_Rasterizer->FillMode = D3D11_FILL_SOLID;
		m_Rasterizer->CullMode = D3D11_CULL_BACK;
		m_Rasterizer->FrontCounterClockwise = FALSE;
		m_Rasterizer->DepthBias = 0;
		m_Rasterizer->DepthBiasClamp = 0;
		m_Rasterizer->SlopeScaledDepthBias = 0;
		m_Rasterizer->DepthClipEnable = TRUE;
		m_Rasterizer->ScissorEnable = FALSE;
		m_Rasterizer->MultisampleEnable = FALSE;
		m_Rasterizer->AntialiasedLineEnable = FALSE;

		Create();
	}

	void _RasterizerState::CreateCullNone()
	{
		memset(m_Rasterizer, 0, sizeof(D3D11_RASTERIZER_DESC));

		m_Rasterizer->FillMode = D3D11_FILL_SOLID;
		m_Rasterizer->CullMode = D3D11_CULL_NONE;
		m_Rasterizer->FrontCounterClockwise = FALSE;
		m_Rasterizer->DepthBias = 0;
		m_Rasterizer->DepthBiasClamp = 0;
		m_Rasterizer->SlopeScaledDepthBias = 0;
		m_Rasterizer->DepthClipEnable = TRUE;
		m_Rasterizer->ScissorEnable = FALSE;
		m_Rasterizer->MultisampleEnable = FALSE;
		m_Rasterizer->AntialiasedLineEnable = FALSE;

		Create();
	}

	_RasterizerState::~_RasterizerState()
	{
		Destroy();
	}


	bool _RasterizerState::CompareIftheSame(D3D11_RASTERIZER_DESC *dest)
	{
		// Compare the values to check if it's the same

		if(memcmp(dest,m_Rasterizer,sizeof(D3D11_RASTERIZER_DESC))==0) return true;

		return false;
	}

	void _RasterizerState::Create(D3D11_RASTERIZER_DESC *dest)
	{
		memcpy(m_Rasterizer, dest, sizeof(D3D11_RASTERIZER_DESC));

		Create();
	}

	void _RasterizerState::Create()
	{
		pin_ptr<ID3D11RasterizerState*> state = &m_State;

		HRESULT hr = m_Device->GetDevice()->CreateRasterizerState(m_Rasterizer, state);
		
		if(FAILED(hr))
		{
			FatalLog.WriteDXError("_RasterizerState::Create",hr);
		}
	}

	void _RasterizerState::SetToDevice()
	{
		m_Device->GetInmediateContext()->RSSetState(m_State);
	}

	void _RasterizerState::Destroy()
	{
		if(m_State)
		{
			m_State->Release();

			m_State=NULL;
		}

		if(m_Rasterizer)
		{
			delete m_Rasterizer;

			m_Rasterizer=NULL;
		}
	}

	//--------------------------------------------------------------------------------------------------------------------------
	/*
	DepthStencilDescription::DepthStencilDescription()
	{
		DepthEnable = true;
		DepthWriteMask = D3D11DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		DepthFunc = D3D11COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		StencilEnable = false;
		StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		FrontFace.StencilFunc = D3D11COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
		FrontFace.StencilPassOp = D3D11STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		FrontFace.StencilFailOp = D3D11STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		FrontFace.StencilDepthFailOp = D3D11STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		BackFace.StencilFunc = D3D11COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
		BackFace.StencilPassOp = D3D11STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		BackFace.StencilFailOp = D3D11STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		BackFace.StencilDepthFailOp = D3D11STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	}
	*/
	_DepthStencilState::_DepthStencilState(GraphicsDevice^ device)
	{
		m_Device=device;

		m_DepthStencil=new D3D11_DEPTH_STENCIL_DESC;

		m_State=NULL;
		/*
		memset(m_DepthStencil, 0, sizeof(D3D11_DEPTH_STENCIL_DESC));

		m_DepthStencil->DepthEnable = TRUE;
		m_DepthStencil->DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		m_DepthStencil->DepthFunc = D3D11_COMPARISON_LESS;
		m_DepthStencil->StencilEnable = FALSE;
		m_DepthStencil->StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		m_DepthStencil->StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		m_DepthStencil->FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		m_DepthStencil->FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		m_DepthStencil->FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		m_DepthStencil->FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		m_DepthStencil->BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		m_DepthStencil->BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		m_DepthStencil->BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		m_DepthStencil->BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		*/
	}
	/*
	Build the Default state where Stencil is off, and Depth is enabled or disabled
	*/
	void _DepthStencilState::BuildDefault_Depth(bool enabled)
	{
		memset(m_DepthStencil, 0, sizeof(D3D11_DEPTH_STENCIL_DESC));

		m_DepthStencil->DepthEnable = (enabled ? TRUE : FALSE);
		m_DepthStencil->DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		m_DepthStencil->DepthFunc = D3D11_COMPARISON_LESS;
		m_DepthStencil->StencilEnable = FALSE;
		m_DepthStencil->StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		m_DepthStencil->StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		m_DepthStencil->FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		m_DepthStencil->FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		m_DepthStencil->FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		m_DepthStencil->FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		m_DepthStencil->BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		m_DepthStencil->BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		m_DepthStencil->BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		m_DepthStencil->BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

		Create();
	}

	_DepthStencilState::~_DepthStencilState()
	{
		Destroy();
	}

	bool _DepthStencilState::CompareIftheSame(D3D11_DEPTH_STENCIL_DESC *dest)
	{
		// Compare the values to check if it's the same

		if(memcmp(dest,m_DepthStencil,sizeof(D3D11_DEPTH_STENCIL_DESC))==0) return true;

		return false;
	}

	void _DepthStencilState::Create()
	{
		pin_ptr<ID3D11DepthStencilState*> state = &m_State;
		HRESULT hr = m_Device->GetDevice()->CreateDepthStencilState(m_DepthStencil, state);

		if (FAILED(hr))
		{
			FatalLog.WriteDXError("_DepthStencilState::Create", hr);
		}
	}	

	void _DepthStencilState::Create(D3D11_DEPTH_STENCIL_DESC *dest)
	{
		memcpy(m_DepthStencil,dest,sizeof(D3D11_DEPTH_STENCIL_DESC));

		Create();
	}

	void _DepthStencilState::SetToDevice(UINT stencilRef)
	{
		m_Device->GetInmediateContext()->OMSetDepthStencilState(m_State,stencilRef);
	}

	void _DepthStencilState::Destroy()
	{
		if(m_State)
		{
			m_State->Release();

			m_State=NULL;
		}

		if(m_DepthStencil)
		{
			delete m_DepthStencil;

			m_DepthStencil=NULL;
		}
	}

	//-----------------------------------------------------------------------------------------------
	// Sampler
	//-----------------------------------------------------------------------------------------------

	_SamplerState::_SamplerState(GraphicsDevice^ device)
	{
		m_Device=device;

		m_Sampler=new D3D11_SAMPLER_DESC;

		m_State = NULL;
	}

	_SamplerState::~_SamplerState()
	{
		Destroy();
	}

	bool _SamplerState::CompareIftheSame(D3D11_SAMPLER_DESC *dest)
	{
		// Compare the values to check if it's the same

		if(memcmp(dest,m_Sampler,sizeof(D3D11_SAMPLER_DESC))==0) return true;

		return false;
	}

	void _SamplerState::CreateDefault()
	{
		D3D11_SAMPLER_DESC desc;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.MinLOD = -FLT_MAX;
		desc.MaxLOD = FLT_MAX;
		desc.MipLODBias = 0.0f;
		desc.MaxAnisotropy = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.BorderColor[0] = 1.0f;
		desc.BorderColor[1] = 1.0f;
		desc.BorderColor[2] = 1.0f;
		desc.BorderColor[3] = 1.0f;

		Create(&desc);
	}

	void _SamplerState::Create(D3D11_SAMPLER_DESC *dest)
	{
		memcpy(m_Sampler,dest,sizeof(D3D11_SAMPLER_DESC));

		pin_ptr<ID3D11SamplerState*> state=&m_State;
		HRESULT hr = m_Device->GetDevice()->CreateSamplerState(m_Sampler, state);
		
		if(FAILED(hr))
		{
			FatalLog.WriteDXError("_SamplerState::Create",hr);
		}
	}

	void _SamplerState::SetToDevice(UINT channel)
	{
		pin_ptr<ID3D11SamplerState*> sampler=&m_State;
		m_Device->GetInmediateContext()->PSSetSamplers( channel, 1, sampler );
	}

	void _SamplerState::Destroy()
	{
		if(m_State)
		{
			m_State->Release();

			m_State=NULL;
		}

		if(m_Sampler)
		{
			delete m_Sampler;

			m_Sampler=NULL;
		}
	}

}

	/*
D3D11_BLEND_DESC bsd
	memset(&bsd, 0, sizeof(bsd));

	bsd.RenderTarget[0].BlendEnable = true;
	bsd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bsd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bsd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bsd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bsd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bsd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bsd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = m_dev->CreateBlendState(&bsd, &m_merge.bs);

	D3D11_RASTERIZER_DESC rd;

	memset(&rd, 0, sizeof(rd));

	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;
	rd.FrontCounterClockwise = false;
	rd.DepthBias = false;
	rd.DepthBiasClamp = 0;
	rd.SlopeScaledDepthBias = 0;
	rd.DepthClipEnable = false; // ???
	rd.ScissorEnable = true;
	rd.MultisampleEnable = true;
	rd.AntialiasedLineEnable = false;

	hr = m_dev->CreateRasterizerState(&rd, &m_rs);

	m_ctx->RSSetState(m_rs);

	
	D3D11_DEPTH_STENCIL_DESC dsd;

	memset(&dsd, 0, sizeof(dsd));

	m_DepthStencil->DepthEnable = false;
	m_DepthStencil->StencilEnable = true;
	m_DepthStencil->StencilReadMask = 1;
	m_DepthStencil->StencilWriteMask = 1;
	m_DepthStencil->FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	m_DepthStencil->FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	m_DepthStencil->FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	m_DepthStencil->FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	m_DepthStencil->BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	m_DepthStencil->BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	m_DepthStencil->BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	m_DepthStencil->BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	m_dev->CreateDepthStencilState(&dsd, &m_date.dss);
*/