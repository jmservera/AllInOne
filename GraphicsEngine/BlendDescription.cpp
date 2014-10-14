#include "stdafx.h"

#include "AllHeadersCommon.h"

namespace GraphicsEngineCommonSpace {

	BlendDescription::BlendDescription()
	{
		RenderTarget=gcnew List<RenderTargetBlendDescription^>();

		for(int i=0;i<8;i++)
		{
			RenderTarget->Add(gcnew RenderTargetBlendDescription());
		}

		AlphaToCoverageEnable=false;
		IndependentBlendEnable=false;

		for(int i=0;i<8;i++)
		{
			RenderTarget[i]->BlendEnable=false;
			RenderTarget[i]->SrcBlend=BLEND::ONE;
			RenderTarget[i]->DestBlend=BLEND::ZERO;
			RenderTarget[i]->BlendOp=BLENDOP::ADD;
			RenderTarget[i]->SrcBlendAlpha=BLEND::ONE;
			RenderTarget[i]->DestBlendAlpha=BLEND::ZERO;
			RenderTarget[i]->BlendOpAlpha=BLENDOP::ADD;
			RenderTarget[i]->RenderTargetWriteMask=COLORWRITEENABLE::ALL;
		}

		InternalIndex=-1;
	}	
}
