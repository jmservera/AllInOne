#include "stdafx.h"

#include "AllHeaders.h"

namespace GraphicsEngineSpace {

	MaterialLayer::MaterialLayer()
	{
		m_Maps=gcnew List<TextureMap^>();
	}

	MaterialLayer::~MaterialLayer()
	{
		for each(TextureMap^ map in m_Maps)
		{
			delete map;
		}

		m_Maps->Clear();
	}


}
