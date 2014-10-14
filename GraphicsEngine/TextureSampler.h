#pragma once

namespace GraphicsEngineSpace {

	public ref class TextureSampler
	{
	public:

		TextureSampler();

		property _Texture^	Texture;
		property Sampler^	Sampler;
	};

}
