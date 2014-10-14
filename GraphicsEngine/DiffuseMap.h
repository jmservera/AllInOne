#pragma once

namespace GraphicsEngineSpace {

	public ref class DiffuseMap:public TextureMap
	{
	public:

		DiffuseMap();

		virtual void SetToDevice(GraphicsDevice^,int Channel) override;

		property TextureSampler^	TextureSampler;

	};

}
