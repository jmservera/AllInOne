#pragma once

namespace GraphicsEngineSpace {


	public ref class _MeshBasic
	{
	private:
		_VertexBuffer^ VertexBuffer;
		_IndexBuffer^ IndexBuffer;
		int OffsetForPosition;
		int OffsetForNormal;
		int OffsetForTangent;
		int OffsetForColor;
		int OffsetForTexCoord;
		int BufferSizeInBytes;

		_InputLayout^ inputLayout;

		void ResetOffsets();

		GraphicsDevice^ m_Device;

		_Buffer^ buffer;

		void Update(int numVertices, float* position, float* color, float* normal, float *tangent, float* texcoord, int numChannelsTexCoord);

		_Texture^ FindTextureFromID(int textureID);

	public:
		
		_MeshBasic(GraphicsDevice^ device);

		void Destroy();
		
		void Update(array<UInt32>^ indices);

		void Update(array<Vector3>^ position, array<Vector4>^ color, array<Vector3>^ normal, array<Vector3>^ tangent, array<Vector2, 2>^ texCoord);

		void Draw(Matrix% world, Matrix% view, Matrix% projection, Material^ material, Camera^ camera,System::Collections::Generic::List<Light^>^ lights);

		void Draw(Matrix% world, Matrix% view, Matrix% projection, Material^ material, Camera^ camera,RenderableMeshPart% meshPart, System::Collections::Generic::List<Light^>^ lights);
			
	};
}