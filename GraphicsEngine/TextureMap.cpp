#include "stdafx.h"

#include "AllHeaders.h"

namespace GraphicsEngineSpace {

	TextureMap::TextureMap()
	{
		IsDiffuse=false;
		IsSpecular=false;

		Channel=-1;
	}

	void TextureMap::SetToDevice(int channel)
	{
	}
}
