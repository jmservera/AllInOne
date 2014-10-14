#include "stdafx.h"

#include "AllHeaders.h"

namespace GraphicsEngineSpace {

	char *diffuseVertexShaderOutput="float2 Tex%Channel% : TEXCOORD%Channel%";
	char *diffuseVarsShader="Texture2D txDiffuse%Channel% : register( t%Channel% );\nSamplerState samDiffuse%Channel% : register( s%Channel% );\n";
	char *diffuseVertexShader="output.Tex%Channel% = Tex%Channel%;";
	char *diffusePixelShader="diffuseColor*=txDiffuse%Channel%.Sample( samDiffuse%Channel%, input.Tex%Channel% );";

	//txDiffuse.Sample( samLinear, input.Tex )
	DiffuseMap::DiffuseMap():TextureMap()
	{
		m_InternalName="DiffuseMap";

		TextureSampler=gcnew GraphicsEngineSpace::TextureSampler();
		
		IsDiffuse=true;

		m_VertexShaderCode=diffuseVertexShader;

		m_VertexShaderOutput=diffuseVertexShaderOutput;

		m_VarsCode=diffuseVarsShader;

		m_PixelShaderCode=diffusePixelShader;
	}
	
	void DiffuseMap::SetToDevice(GraphicsDevice^ device,int channel)
	{
		TextureSampler->Texture->SetToDevice(channel);
		TextureSampler->Sampler->SetToDevice(device,channel);

	}
}
