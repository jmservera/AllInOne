#include "stdafx.h"

#include "AllHeaders.h"

namespace GraphicsEngineSpace {

	Material::Material(String^name)
	{
		Name=name;

		Layers=gcnew List<MaterialLayer^>();
	}

	Material::~Material()
	{
		for each(MaterialLayer^ layer in Layers)
		{
			delete layer;
		}

		Layers->Clear();
	}


}
