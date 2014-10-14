#pragma once

namespace GraphicsEngineSpace {

	public ref class Material
	{
	internal:


	public:

		Material(String^name);
		~Material();

		property String^		Name;

		property List<MaterialLayer^>^		Layers;

	};

}
