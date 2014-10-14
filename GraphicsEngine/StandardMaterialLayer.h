#pragma once

namespace GraphicsEngineSpace {

	public ref class StandardMaterialLayer: public MaterialLayer
	{
	public:

		StandardMaterialLayer();
		~StandardMaterialLayer();

		property DiffuseMap^ Diffuse { DiffuseMap^ get() { return (DiffuseMap^)m_Maps[0]; } }

	};

}
