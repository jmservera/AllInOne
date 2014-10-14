#pragma once

namespace GraphicsEngineCommonSpace {

	public enum class FILLMODE
    {	WIREFRAME	= 2,
		SOLID	= 3
    };

	public enum class CULLMODE
    {	
		NONE	= 1,
		FRONT	= 2,
		BACK	= 3
    };

	public ref class RasterizerDescription
    {
	public:
		FILLMODE FillMode;
		CULLMODE CullMode;
		bool FrontCounterClockwise;
		int DepthBias;
		float DepthBiasClamp;
		float SlopeScaledDepthBias;
		bool DepthClipEnable;
		bool ScissorEnable;
		bool MultisampleEnable;
		bool AntialiasedLineEnable;

		RasterizerDescription();

		int InternalIndex;

		void Changed() { InternalIndex=-1; }
    };
}
