#include "stdafx.h"

#include "AllHeaders.h"

namespace GraphicsEngineSpace {

	_MeshBasic::_MeshBasic(GraphicsDevice^ device)
	{
		m_Device = device;

		VertexBuffer = gcnew _VertexBuffer(device, false);

		IndexBuffer = gcnew _IndexBuffer(m_Device);

		buffer = gcnew _Buffer(m_Device, false);
	}

	//--------------------------------------------------------------------------------------
	// Releases the resources and frees memory
	//--------------------------------------------------------------------------------------
	void _MeshBasic::Destroy()
	{
		if (VertexBuffer != nullptr)
		{
			VertexBuffer->Destroy();
			VertexBuffer = nullptr;

			buffer->Destroy();

			buffer = nullptr;
		}

		if (IndexBuffer != nullptr)
		{
			IndexBuffer->Destroy();
			IndexBuffer = nullptr;

		}
		// The input layout has to bee destroyed when the device is destroyed, because the layout must be shared with other objects
	}

	void _MeshBasic::ResetOffsets()
	{
		OffsetForPosition = -1;
		OffsetForNormal = -1;
		OffsetForTangent = -1;
		OffsetForColor = -1;
		OffsetForTexCoord = -1;
		BufferSizeInBytes = 0;
	}
	
	//--------------------------------------------------------------------------------------
	// Updates the Mesh with information about the indices
	//--------------------------------------------------------------------------------------
	void _MeshBasic::Update(array<UInt32>^ indices)
	{
		pin_ptr<UInt32> position_pointer = &indices[0];

		IndexBuffer->BindIndices(position_pointer,indices->Length);

		IndexBuffer->Update();

	}
	//--------------------------------------------------------------------------------------
	// Updates the Mesh with information about the vertices
	//--------------------------------------------------------------------------------------
	void _MeshBasic::Update(array<Vector3>^ position, array<Vector4>^ color, array<Vector3>^ normal, array<Vector3>^ tangent, array<Vector2, 2>^ texCoord)
	{
		pin_ptr<float> position_pointer = nullptr;
		pin_ptr<float> color_pointer = nullptr;
		pin_ptr<float> normal_pointer = nullptr;
		pin_ptr<float> tangent_pointer = nullptr;
		pin_ptr<float> texCoord_pointer = nullptr;

		position_pointer = &position[0].X;
		if (color!=nullptr) color_pointer = &color[0].X;
		if (normal != nullptr) 
			normal_pointer = &normal[0].X;
		if (tangent != nullptr) tangent_pointer = &tangent[0].X;
		if (texCoord != nullptr) texCoord_pointer = &texCoord[0, 0].X;

		int numChannelsTexCoord = texCoord->GetUpperBound(0)+1;

		Update(position->Length, position_pointer, color_pointer, normal_pointer, tangent_pointer, texCoord_pointer, numChannelsTexCoord);
	}

	//--------------------------------------------------------------------------------------
	// private update. Creates or updates the VertexBuffer with the Mesh information. It also creates the input layout based in the Vertices format
	//--------------------------------------------------------------------------------------
	void _MeshBasic::Update(int numVertices, float* position, float* color, float* normal, float *tangent, float* texCoord, int numChannelsTexCoord)
	{
		_VertexBuffer^ vb = VertexBuffer;

		ResetOffsets();

		int bufferSize = 0;
		int offset = 0;

		int semanticSize = 0;

		if (position != nullptr)
		{
			semanticSize = 3 * 4;		// X,Y,Z * sizeof(float);

			bufferSize += semanticSize;

			OffsetForPosition = offset;

			offset += semanticSize;

			vb->BindPosition(position, numVertices, OffsetForPosition, semanticSize);
		}

		if (color != nullptr)
		{
			semanticSize = 4 * 4;		// RGBA * sizeof(float);

			bufferSize += semanticSize;

			OffsetForColor = offset;

			offset += semanticSize;

			vb->BindColor(color, OffsetForColor, semanticSize);
		}

		if (normal != nullptr)
		{
			semanticSize = 3 * 4;		// X,Y,Z * sizeof(float);

			bufferSize += semanticSize;

			OffsetForNormal = offset;

			offset += semanticSize;

			vb->BindNormal(normal, OffsetForNormal, semanticSize);
		}

		if (tangent != nullptr)
		{
			semanticSize = 3 * 4;		// X,Y,Z * sizeof(float);

			bufferSize += semanticSize;

			OffsetForTangent = offset;

			offset += semanticSize;

			vb->BindTangent(tangent, OffsetForTangent, semanticSize);
		}

		if (texCoord != nullptr)
		{
			int numChannels = numChannelsTexCoord;

			semanticSize = 2 * numChannels * 4;		// X,Y * sizeof(float);

			bufferSize += semanticSize;

			OffsetForTexCoord = offset;

			offset += semanticSize;

			vb->BindTexCoord(texCoord, 1, OffsetForTexCoord, semanticSize);
		}

		vb->Update();

		inputLayout = gcnew _InputLayout(m_Device);

		if (position != nullptr)
		{
			InputLayoutElement^ element = gcnew InputLayoutElement();

			element->SemanticName = InputLayoutSemantic::Position;
			element->Format = (int)DXGIFORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
			element->AlignedByteOffset = OffsetForPosition;

			inputLayout->Items.Add(element);

		}

		if (color != nullptr)
		{
			InputLayoutElement^ element = gcnew InputLayoutElement();

			element->SemanticName =  InputLayoutSemantic::Color;
			element->Format = (int)DXGIFORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
			element->AlignedByteOffset = OffsetForColor;

			inputLayout->Items.Add(element);

		}

		if (normal != nullptr)
		{
			InputLayoutElement^ element = gcnew InputLayoutElement();

			element->SemanticName = InputLayoutSemantic::Normal;
			element->Format = (int)DXGIFORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
			element->AlignedByteOffset = OffsetForNormal;

			inputLayout->Items.Add(element);

		}

		if (tangent != nullptr)
		{
			InputLayoutElement^ element = gcnew InputLayoutElement();

			element->SemanticName = InputLayoutSemantic::Tangent;
			element->Format = (int)DXGIFORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
			element->AlignedByteOffset = OffsetForTangent;

			inputLayout->Items.Add(element);

		}

		if (texCoord != nullptr)
		{
			InputLayoutElement^ element = gcnew InputLayoutElement();

			element->SemanticName = InputLayoutSemantic::TexCoord;
			element->Format = (int)DXGIFORMAT::DXGI_FORMAT_R32G32_FLOAT;
			element->AlignedByteOffset = OffsetForTexCoord;

			inputLayout->Items.Add(element);

		}
		

		inputLayout = m_Device->AddInputLayoutIfNeeded(inputLayout);

		// NOTE: We do not need to remove the elements not used, it will be Garbage Collected
	}
	static RenderableMeshPart nullRenderableMeshPart;

	//--------------------------------------------------------------------------------------
	// Draws the mesh
	//--------------------------------------------------------------------------------------
	void _MeshBasic::Draw(Matrix% world, Matrix% view, Matrix% projection, Material^ material, Camera^ camera, System::Collections::Generic::List<Light^>^ lights)
	{
		Draw(world, view, projection, material,camera, nullRenderableMeshPart,lights);
	}

	_Texture^ _MeshBasic::FindTextureFromID(int textureID)
	{
		for each(_Texture^ textureTmp in m_Device->m_Textures)
		{
			if (textureTmp->GetID() == textureID)
			{
				return textureTmp;

			}
		}

		return nullptr;

	}

	struct _shaderLight
	{
		Vector4 a;
		Vector4 b;
		Vector4 c;
		Vector4 d;
	};

	struct _shaderMaterial
	{
		Vector4 a;
		Vector4 b;
		Vector4 c;
		Vector4 d;
	};
	struct _CameraShader
	{
		Vector4 a;
		Vector4 b;
		Vector4 c;
		Vector4 d;
	};
	//--------------------------------------------------------------------------------------
	// Draws the mesh
	//--------------------------------------------------------------------------------------
	void _MeshBasic::Draw(Matrix% world, Matrix% view, Matrix% projection, Material^ material,Camera^ camera, RenderableMeshPart% meshPart, System::Collections::Generic::List<Light^>^ lights)
	{
		VertexBuffer->SetToDevice(0);
		IndexBuffer->SetToDevice(0);
		inputLayout->SetToDevice();

		// We need to find the texture based on a ID

		int textureID = material->Layers[0]->DiffuseMap->GetID();

		_Texture^ texture = FindTextureFromID(textureID);

		if (texture == nullptr)
		{
			char tmp[120];
			sprintf(tmp, "_MeshBasic::Draw:. ERROR. I CAN NOT FOUND THE DIFFUSEMAP TEXTURE ID:%D", textureID);
			InternalLog.WriteLine(tmp);
		}

		texture->SetToDevice(0);

		m_Device->m_DefaultStates.Linear->SetToDevice(0);

		if (material->Layers[0]->NormalMap != nullptr)
		{
			textureID = material->Layers[0]->NormalMap->GetID();

			texture = FindTextureFromID(textureID);

			if (texture == nullptr)
			{
				char tmp[120];
				
				sprintf(tmp, "_MeshBasic::Draw:. ERROR. I CAN NOT FOUND THE NORMALMAP TEXTURE ID:%D", textureID);

				InternalLog.WriteLine(tmp);
			}

			texture->SetToDevice(1);

			m_Device->m_DefaultStates.Linear->SetToDevice(1);
		}

		if (lights == nullptr || lights->Count==0)
		{
			m_Device->RectangleShader->SetToDevice();


			Vector4 v(1, 1, 1, 1);

			buffer->BeginAddData();
			buffer->AddMatrix(world);
			buffer->AddMatrix(view);
			buffer->AddMatrix(projection);
			buffer->Add(v);
			buffer->EndAddData();
		}
		else
		{
			_Shader^ shader = m_Device->GetShaderByID(material->Layers[0]->ShaderID);

			shader->SetToDevice();


			Vector4 v(1, 1, 1, 1);

			buffer->BeginAddData();
			buffer->AddMatrix(world);
			buffer->AddMatrix(view);
			buffer->AddMatrix(projection);
			
			MaterialLayer^ layer = material->Layers[0];

			_shaderMaterial shaderMaterial;
			shaderMaterial.a = v;
			shaderMaterial.b.X = layer->Specular.Power;
			shaderMaterial.b.Y = layer->Specular.Color.X;
			shaderMaterial.b.Z = layer->Specular.Color.Y;
			shaderMaterial.b.W = layer->Specular.Color.Z;

			shaderMaterial.c.X = (layer->Specular.Enabled ? 1 : 0 );

			buffer->Add(shaderMaterial.a);
			buffer->Add(shaderMaterial.b);
			buffer->Add(shaderMaterial.c);
			buffer->Add(shaderMaterial.d);

			_shaderLight shaderLight;

			Light^ light = lights[0];

			shaderLight.a.X = light->Position.X;
			shaderLight.a.Y = light->Position.Y;
			shaderLight.a.Z = light->Position.Z;

			shaderLight.a.W = light->Range;

			shaderLight.b = light->Color*light->Intensity;

			shaderLight.c.X = 1.0f / light->Range;
			shaderLight.c.Y = (float)light->Attenuation;
			shaderLight.c.Z = (float)light->FarAttenStart;

			if (light->GetType() == OmniLight::typeid)
			{
				// Use this trick to no consum another Vector4
				shaderLight.c.W = -99;
			}
				
			if (light->GetType() == SpotLight::typeid)
			{
				SpotLight^ spot = (SpotLight^)light;
				
				shaderLight.c.W = cos(ToRadians(spot->FallOff));
				shaderLight.d.X = spot->Direction.X;
				shaderLight.d.Y = spot->Direction.Y;
				shaderLight.d.Z = spot->Direction.Z;
				shaderLight.d.W = cos(ToRadians(spot->HotSpot));
			}

			buffer->Add(shaderLight.a);
			buffer->Add(shaderLight.b);
			buffer->Add(shaderLight.c);
			buffer->Add(shaderLight.d);

			_CameraShader cameraMaterial;

			cameraMaterial.a.X = camera->Position.X;
			cameraMaterial.a.Y = camera->Position.Y;
			cameraMaterial.a.Z = camera->Position.Z;

			buffer->Add(cameraMaterial.a);
			buffer->Add(cameraMaterial.b);
			buffer->Add(cameraMaterial.c);
			buffer->Add(cameraMaterial.d);


			/*
			buffer->Add(light->Position);
			buffer->Add(light->Color);
			v.X = light->Range;
			buffer->Add(v);
			v.X = 1 / light->Range;
			buffer->Add(v);
			*/
			buffer->EndAddData();
		}

		buffer->SetToDevice(0);
		
		if (meshPart.IndexCount>0)
			m_Device->DrawIndexed(meshPart.IndexCount, meshPart.StartIndexLocation, meshPart.BaseVertexLocation);
		else
			m_Device->DrawIndexed(IndexBuffer->Count);
	}

}
