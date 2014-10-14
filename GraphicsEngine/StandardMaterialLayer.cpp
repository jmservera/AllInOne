#include "stdafx.h"

#include "AllHeaders.h"

namespace GraphicsEngineSpace {

	StandardMaterialLayer::StandardMaterialLayer() : MaterialLayer()
	{
		DiffuseMap^ diffuse=gcnew DiffuseMap();
		
		m_Maps->Add(diffuse);
	}

	StandardMaterialLayer::~StandardMaterialLayer()
	{
	}


}
