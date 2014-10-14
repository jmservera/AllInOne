#include "stdafx.h"

#include "AllHeaders.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#ifdef _DEBUG
#pragma comment(lib, "d3dx11d.lib")
#pragma comment(lib, "d3dx9d.lib")
#else
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx9.lib")
#endif
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "DxErr.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "dxgi.lib")

namespace GraphicsEngineSpace {

	D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;

	Log InternalLog;
	Log FatalLog;

	// BD counter for the GraphicsDevice
	static int GraphicsDevice_BDID = 0;
	
	// Used to know when the last Graphics Device is Destroyed and then Release the SQL static class
	static int GraphicsDeviceCounter = 0;

	GraphicsDevice::GraphicsDevice()
	{
		m_ImmediateContext = nullptr;
		m_Device = nullptr;
		m_Factory = nullptr;

		m_DefaultStates.m_Device = this;

		m_QuadMesh = nullptr;

		m_TemporalBuffer = nullptr;

		GraphicsDeviceCounter ++;

	}

	GraphicsDevice::~GraphicsDevice()
	{
		Destroy();
	}
	
//--------------------------------------------------------------------------------------
// Create Direct3D device
//--------------------------------------------------------------------------------------
	void GraphicsDevice::Initialize(String^ deviceName,String^ pathParam)
	{
#ifdef _DEBUG
		// Set output debug info
		_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

		CString wPath(pathParam);

		InternalLog.SetFileName(wPath+L"Log\\infoLog.txt");

		FatalLog.SetFileName(wPath+L"Log\\FatalLog.txt");

		InternalLog.Delete();

		FatalLog.Delete();

		m_BDID = ++GraphicsDevice_BDID;

		if (GraphicsDeviceCounter==1)
			SQL::Open(pathParam);

		SQL::Insert("INSERT INTO DEVICE(IDDevice,NDevice) VALUES(" + m_BDID + "," + SQL::ToString(deviceName)+ ")");
			
		HRESULT hr = Init();

		m_DefaultStates.Create();

		// Now we create the shaders

		Shader = gcnew _Shader(this,0);

		Shader->CompileFromFile(pathParam + "genericShader.txt");

		m_Shaders.Add(Shader);

		PositionColorShader = gcnew _Shader(this, 2);

		PositionColorShader->CompileFromFile(pathParam + "PositionColor.txt");

		m_Shaders.Add(PositionColorShader);

		RectangleShader = gcnew _Shader(this, 1);

		RectangleShader->CompileFromFile(pathParam + "RectangleTextured.txt");

		m_Shaders.Add(RectangleShader);

		// Create the temporal buffer
		m_TemporalBuffer = gcnew _Buffer(this);
		m_TemporalVB = gcnew _VertexBuffer(this, true);
		m_TemporalIB = gcnew _IndexBuffer(this);
		m_TemporalIB->Use32BitIndices = true;
	}

	_Shader^ GraphicsDevice::GetShaderByID(UInt32 id)
	{
		for each (_Shader^ shader in m_Shaders)
		{
			if (shader->GetID() == id)
				return shader;
		}

		return nullptr;
	}

	HRESULT GraphicsDevice::Init()
	{
		HRESULT hr = S_OK;



		UINT createDeviceFlags = 0;
	#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};

		const char* driverTypesString[]=
		{
			"D3D_DRIVER_TYPE_HARDWARE",
			"D3D_DRIVER_TYPE_WARP",
			"D3D_DRIVER_TYPE_REFERENCE",
		};


		UINT numDriverTypes = ARRAYSIZE( driverTypes );

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		UINT numFeatureLevels = ARRAYSIZE( featureLevels );

		const char*  featureLevelsString[] =
		{
			"D3D_FEATURE_LEVEL_11_0",
			"D3D_FEATURE_LEVEL_10_1",
			"D3D_FEATURE_LEVEL_10_0",
		};
		IDXGIFactory1* lfactory;

		pin_ptr<IDXGIFactory1*> factory = &m_Factory;
		
		InternalLog.WriteLine("Creating Factory1");

		hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(factory));
		if (FAILED(hr))
		{
			FatalLog.WriteDXError("GraphicsDevice::Initialize(CreateDXGIFactory1)", hr);
			return hr;
		}
		/**/
		if (factory == nullptr)
		{
			FatalLog.WriteLine("GraphicsDevice::Initialize. Factory is null");
			return 0;
		}
		
		if (m_Factory == nullptr)
		{
			FatalLog.WriteLine("GraphicsDevice::Initialize. Factory is null");
			return 0;
		}
		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			g_driverType = driverTypes[driverTypeIndex];
			pin_ptr<ID3D11Device*> device = &m_Device;
			pin_ptr<ID3D11DeviceContext*> context = &m_ImmediateContext;
			char tmpString[128];

			sprintf(tmpString, "Trying driver %s for D3D11_SDK_VERSION using D3D11CreateDevice", driverTypesString[driverTypeIndex]);

			InternalLog.WriteLine(tmpString);

			hr = D3D11CreateDevice(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
				D3D11_SDK_VERSION, device, &g_featureLevel, context);
			if (SUCCEEDED(hr))
			{
				sprintf(tmpString, "Driver %s created.", driverTypesString[driverTypeIndex]);
				InternalLog.WriteLine(tmpString);
				break;
			}
		}

		if (FAILED(hr))
		{
			FatalLog.WriteDXError("GraphicsDevice::Initialize(D3D11CreateDevice)", hr);
			return hr;
		}

		/*
		// This code is an example using  D3D11CreateDeviceAndSwapChain

			m_SwapChainDesc = new DXGI_SWAP_CHAIN_DESC;

			ZeroMemory(m_SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
			m_SwapChainDesc->BufferCount = 1;
			m_SwapChainDesc->BufferDesc.Width = m_Width;
			m_SwapChainDesc->BufferDesc.Height = m_Height;
			m_SwapChainDesc->BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			m_SwapChainDesc->BufferDesc.RefreshRate.Numerator = 0;	// vsync off
			m_SwapChainDesc->BufferDesc.RefreshRate.Denominator = 1;
			m_SwapChainDesc->BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			m_SwapChainDesc->OutputWindow = m_hWnd;
			m_SwapChainDesc->SampleDesc.Count = 1;
			m_SwapChainDesc->SampleDesc.Quality = 0;
			m_SwapChainDesc->Windowed = TRUE;

			for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
			{
				g_driverType = driverTypes[driverTypeIndex];
				pin_ptr<ID3D11Device*> device = &m_Device;
				pin_ptr<ID3D11DeviceContext*> context = &m_ImmediateContext;
				pin_ptr<IDXGISwapChain*> swapChain = &m_SwapChain;
				char tmpString[128];

				sprintf(tmpString, "Trying driver %s for D3D11_SDK_VERSION using D3D11CreateDeviceAndSwapChain", driverTypesString[driverTypeIndex]);

				InternalLog.WriteLine(tmpString);

				hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
					D3D11_SDK_VERSION, m_SwapChainDesc, swapChain, device, &g_featureLevel, context);

				if (SUCCEEDED(hr))
				{
					sprintf(tmpString, "Driver %s created.", driverTypesString[driverTypeIndex]);

					InternalLog.WriteLine(tmpString);

					break;
				}
			}

			if (FAILED(hr))
			{
				FatalLog.WriteDXError("GraphicsDevice::Initialize(D3D11CreateDeviceAndSwapChain)", hr);
				return hr;
			}

			// Get the backbuffer
			pin_ptr<ID3D11Texture2D*> backBuffer = &m_Texture2D11;
			hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
			if (FAILED(hr))
			{
				FatalLog.WriteDXError("GraphicsDevice::Initialize(GetBuffer)", hr);
				return hr;
			}

		}

		m_backBufferTexture = gcnew _Texture2D(this, m_Texture2D11);
		m_renderTarget = gcnew _RenderTarget(this, m_backBufferTexture);

		SetViewPort(0, 0, m_Width, m_Height, 0, 1);
		*/
		return S_OK;
	}
	
	///----------------------------------------------------------
	/// Creates a RenderTarget used for the WPF D3DImage control
	///----------------------------------------------------------
	_RenderTarget^ GraphicsDevice::CreateDX9SharedRenderTarget(String^ name,int width, int height)
	{
		ID3D11Texture2D* pTexture = NULL;

		D3D11_TEXTURE2D_DESC Desc;
		Desc.Width = width;
		Desc.Height = height;
		Desc.MipLevels = 1;
		Desc.ArraySize = 1;
		Desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		Desc.SampleDesc.Count = 1;
		Desc.SampleDesc.Quality = 0;
		Desc.Usage = D3D11_USAGE_DEFAULT;
		Desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		Desc.CPUAccessFlags = 0;
		Desc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;

		InternalLog.WriteLine("GraphicsDevice::CreateDX9SharedRenderTarget");

		HRESULT hr = m_Device->CreateTexture2D(&Desc, NULL, &pTexture);

		if (FAILED(hr))
		{
			FatalLog.WriteDXError("GraphicsDevice::CreateSharedTextureForDX9", hr);
			return nullptr;
		}

		_Texture2D^ backBufferTexture = gcnew _Texture2D(this, name,pTexture);
		_RenderTarget^ renderTarget = gcnew _RenderTarget(this, backBufferTexture);
		
		renderTarget->m_IsDX9BackBuffer = true;

		return renderTarget;
	}

	///----------------------------------------------------------
	/// Creates a RenderTarget from the BackBuffer
	///----------------------------------------------------------
	_RenderTarget^ GraphicsDevice::CreateSwapChainRenderTarget(String^ name,int hwnd)
	{
		RECT rc;
		GetClientRect((HWND)hwnd, &rc);
		UINT width = rc.right - rc.left;
		UINT height = rc.bottom - rc.top;

		DXGI_SWAP_CHAIN_DESC*		m_SwapChainDesc;
		

		m_SwapChainDesc = new DXGI_SWAP_CHAIN_DESC;

		ZeroMemory(m_SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		m_SwapChainDesc->BufferCount = 1;
		m_SwapChainDesc->BufferDesc.Width = width;
		m_SwapChainDesc->BufferDesc.Height = height;
		m_SwapChainDesc->BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_SwapChainDesc->BufferDesc.RefreshRate.Numerator = 0;	// vsync off
		m_SwapChainDesc->BufferDesc.RefreshRate.Denominator = 1;
		m_SwapChainDesc->BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		m_SwapChainDesc->OutputWindow = (HWND)hwnd;
		m_SwapChainDesc->SampleDesc.Count = 1;
		m_SwapChainDesc->SampleDesc.Quality = 0;
		m_SwapChainDesc->Windowed = TRUE;
		
		IDXGISwapChain* m_SwapChain;

		pin_ptr<IDXGISwapChain*> swapChain = &m_SwapChain;

		if (m_Factory==nullptr)
		{
			FatalLog.WriteLine("GraphicsDevice::CreateSwapChainRenderTarget. Factory is null");
			return nullptr;
		}

		/*Starting with Direct3D 11.1, we recommend not to use CreateSwapChain anymore to create a swap chain. Instead, use CreateSwapChainForHwnd, CreateSwapChainForCoreWindow, or CreateSwapChainForComposition depending on how you want to create the swap chain.*/
		HRESULT hr = m_Factory->CreateSwapChain(m_Device, m_SwapChainDesc, (IDXGISwapChain**)swapChain);

		if (FAILED(hr))
		{
			FatalLog.WriteDXError("GraphicsDevice::CreateSwapChainRenderTarget", hr);
			return nullptr;
		}

		// Delete the memory from the description
		//delete m_SwapChainDesc;

		ID3D11Texture2D* m_Texture2D11;

		// Get the backbuffer
		pin_ptr<ID3D11Texture2D*> backBuffer = &m_Texture2D11;
		hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)backBuffer);
		if (FAILED(hr))
		{
			FatalLog.WriteDXError("GraphicsDevice::CreateSwapChainRenderTarget", hr);
			return nullptr;
		}

		_Texture2D^ backBufferTexture = gcnew _Texture2D(this, name, m_Texture2D11);

		_RenderTarget^ renderTarget = gcnew _RenderTarget(this, backBufferTexture, m_SwapChain, m_SwapChainDesc);

		return renderTarget;
	}

	void GraphicsDevice::SetRenderTargetAndStencil(_RenderTarget^ renderTarget,_DepthStencil^ depthStencil)
	{
		if (renderTarget == nullptr)
		{
			m_ImmediateContext->OMSetRenderTargets(0, NULL, NULL);
		}
		else
		{
			pin_ptr<ID3D11RenderTargetView*> renderTargetView = &renderTarget->m_RenderTargetView;

			if (depthStencil == nullptr)
				m_ImmediateContext->OMSetRenderTargets(1, renderTargetView, NULL);
			else
				m_ImmediateContext->OMSetRenderTargets(1, renderTargetView, depthStencil->GetInternal());
		}
	}

	void GraphicsDevice::SetViewPort(int x,int y,int width,int height)
	{
		SetViewPort(x,y,width,height,0,1);
	}
		
	void GraphicsDevice::SetViewPort(int x,int y,int width,int height,float zmin,float zmax)
	{
		// Setup the viewport
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)width;
		vp.Height = (FLOAT)height;
		vp.MinDepth = zmin;
		vp.MaxDepth = zmax;
		vp.TopLeftX = (FLOAT)x;
		vp.TopLeftY = (FLOAT)y;
		m_ImmediateContext->RSSetViewports( 1, &vp );
	}

	void GraphicsDevice::SetScissorRect(int left, int top, int right, int bottom)
	{
		D3D11_RECT rect;
		rect.left=left;
		rect.top=top;
		rect.right=right;
		rect.bottom=bottom;

		m_ImmediateContext->RSSetScissorRects(1,&rect);
	}
	// Draw lines using the color. Note, this method is not Thread Safe
	void GraphicsDevice::DrawLines(array<Vector3>^ arrayofLines,Vector4 color,Matrix% world, Matrix% view, Matrix% projection)
	{
		pin_ptr<float> position_pointer = nullptr;
		position_pointer = &arrayofLines[0].X;	

		_VertexBuffer^ vb = m_TemporalVB;
		vb->BindPosition(position_pointer, arrayofLines->Length, 0, 3 * sizeof(float));
		vb->Update();
		vb->SetToDevice(0);

		PositionColorShader->SetToDevice();
		PositionColorShader->SetDefaultInputLayoutToDevice();

		_Buffer^ buffer = m_TemporalBuffer;

		buffer->BeginAddData();
		buffer->AddMatrix(world);
		buffer->AddMatrix(view);
		buffer->AddMatrix(projection);
		buffer->Add(color);
		buffer->EndAddData();

		buffer->SetToDevice(0);

		m_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		m_ImmediateContext->Draw(arrayofLines->Length , 0);

		vb->Destroy();

	}

	// Draw lines using the color. Note, this method is not Thread Safe
	void GraphicsDevice::DrawLines(array<Vector3>^ arrayofLines, array<UInt32>^ arrayOfIndices, Vector4 color, Matrix% world, Matrix% view, Matrix% projection)
	{
		pin_ptr<float> position_pointer = nullptr;
		position_pointer = &arrayofLines[0].X;

		_VertexBuffer^ vb = gcnew _VertexBuffer(this, true);
		vb->BindPosition(position_pointer, arrayofLines->Length, 0, 3 * sizeof(float));
		vb->Update();
		vb->SetToDevice(0);

		_IndexBuffer^ ib = m_TemporalIB;
		
		pin_ptr<UInt32> indices_pointer = &arrayOfIndices[0];

		ib->BindIndices(indices_pointer, arrayOfIndices->Length);

		ib->Update();

		ib->SetToDevice(0);

		PositionColorShader->SetToDevice();
		PositionColorShader->SetDefaultInputLayoutToDevice();

		_Buffer^ buffer = m_TemporalBuffer;

		buffer->BeginAddData();
		buffer->AddMatrix(world);
		buffer->AddMatrix(view);
		buffer->AddMatrix(projection);
		buffer->Add(color);
		buffer->EndAddData();

		buffer->SetToDevice(0);

		m_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		m_ImmediateContext->DrawIndexed(arrayOfIndices->Length,0, 0);

		vb->Destroy();

		ib->Destroy();

	}

	void GraphicsDevice::DrawIndexed(UInt32 indexBufferCount, UInt32 StartIndexLocation, int BaseVertexLocation)
	{
		// Set primitive topology
		m_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_ImmediateContext->DrawIndexed(indexBufferCount, StartIndexLocation, BaseVertexLocation);
	}

	void GraphicsDevice::DrawIndexed(int indexBufferCount)
    {
		// Set primitive topology
		m_ImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		m_ImmediateContext->DrawIndexed(indexBufferCount, 0,0 );
    }
	
	//--------------------------------------------------------------------------------------
	// This must be called when a resize ocurrs
	//--------------------------------------------------------------------------------------
	void GraphicsDevice::Resize(int width,int height)
	{
		InternalLog.StartMethod("GraphicsDevice::Resize()");

		// We only need to Release the backbuffer
		/*
		InternalLog.WriteLine("Releasing backBuffer");

		m_backBufferTexture->Destroy();

		delete m_backBufferTexture;

		InternalLog.WriteLine("Releasing renderTarget");

		m_renderTarget->Destroy();

		delete m_renderTarget;

		m_Width = width;
		m_Height = height;

		if (m_SwapChain != nullptr)
		{
			InternalLog.WriteLine("Resizing buffers");

			HRESULT hr = m_SwapChain->ResizeBuffers(m_SwapChainDesc->BufferCount, width, height, m_SwapChainDesc->BufferDesc.Format, m_SwapChainDesc->Flags);

			if (FAILED(hr))
			{
				FatalLog.WriteDXError("GraphicsDevice::Resize()", hr);
			}

			InternalLog.WriteLine("Getting BackBuffer");

			ID3D11Texture2D* pBackBuffer = NULL;
			hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			if (FAILED(hr))
			{
				FatalLog.WriteDXError("GraphicsDevice::Initialize(GetBuffer)", hr);
			}
			m_backBufferTexture->SetTexture((ID3D11ShaderResourceView*)pBackBuffer);
			m_backBufferTexture->GetDescription();

			InternalLog.WriteLine("Recreating RenderTarget");
			m_renderTarget->OnCreate();
		}
		else
		{
			CreateSharedTextureForDX9();

			m_backBufferTexture = gcnew _Texture2D(this, m_Texture2D11);
			m_renderTarget = gcnew _RenderTarget(this, m_backBufferTexture);
		}


		InternalLog.WriteLine("Setting ViewPort");

		SetViewPort(0, 0, m_Width, m_Height, 0, 1);
		*/
		InternalLog.EndMethod();
	}
	
	void GraphicsDevice::SetBlendingState(BlendDescription^ item,float* blendFactor,UINT32 sampleMask)
	{
		// If this description has an index already, we do not need to compare
		if(item->InternalIndex!=-1)
		{
			m_BlendingStates[item->InternalIndex]->SetToDevice(blendFactor,sampleMask);

			return;
		}

		// Let's see if we need to create a new one
		D3D11_BLEND_DESC desc;
		
		// Transform the Managed class to the DX struct

		_BlendingState::Convert(item,&desc);

		for each(_BlendingState^ blenderState in m_BlendingStates)
		{
			// Compare the DX struct to see if it's the same
			if(blenderState->CompareIftheSame(&desc)) 
			{
				// Is the same, so set it , and return, because we do not need a new one

				blenderState->SetToDevice(blendFactor,sampleMask);

				return;

			}
		}
		
		// A new blendState!!! We need to add it to the list
		// So, we have to create the DX object

		_BlendingState^ newState=gcnew _BlendingState(this);
		
		newState->Create(&desc);

		newState->SetToDevice(blendFactor,sampleMask);

		// Stores the index for fast access
		item->InternalIndex=m_BlendingStates.Count;

		m_BlendingStates.Add(newState);
	}
	
	// Draws a Full Screen Quad using the texture
	void GraphicsDevice::DrawQuad(ITexture^ texture)
	{
		float zvalue = 0;

		// Create the Mesh if not defined
		if (m_QuadMesh == nullptr)
		{
			m_QuadMesh = gcnew _MeshBasic(this);
			int numVertices = 4;
			int numIndices = 6;

			ib = gcnew array<UInt32>(numIndices);

			vb = gcnew array<Vector3>(numVertices);

			normal = gcnew array<Vector3>(numVertices);

			texCoord = gcnew array<Vector2, 2>(1, numVertices);

			// OGL COUNTER-ClockWISE order

			ib[0] = 2;
			ib[1] = 1;
			ib[2] = 0;
			ib[3] = 3;
			ib[4] = 2;
			ib[5] = 0;

			m_QuadMesh->Update(ib);
			
			int i = 0;

			normal[i] = Vector3(0, 0, -1); texCoord[0, i] = Vector2(0, 0); i++;
			normal[i] = Vector3(0, 0, -1); texCoord[0, i] = Vector2(1, 0); i++;
			normal[i] = Vector3(0, 0, -1); texCoord[0, i] = Vector2(1, 1); i++;
			normal[i] = Vector3(0, 0, -1); texCoord[0, i] = Vector2(0, 1); i++;

			i = 0;
			vb[i] = Vector3(-1, 1, zvalue); i++;
			vb[i] = Vector3(1, 1, zvalue); i++;
			vb[i] = Vector3(1, -1, zvalue); i++;
			vb[i] = Vector3(-1, -1, zvalue); i++;

			m_QuadMesh->Update(vb, nullptr, normal, nullptr, texCoord);

			m_QuadMeshMaterial = gcnew Material("kk");

			m_QuadMeshMaterial->CreateLayer();

		}

		Matrix identity = Matrix::Identity;
		
		m_QuadMeshMaterial->Layers[0]->DiffuseMap = texture;

		m_QuadMesh->Draw(identity, identity, identity, m_QuadMeshMaterial, nullptr, nullptr);

		// I must remove the texutre from the slot 0 because it can be a RenderTarget and when I set This RenderTarget again, DX11 warns me that a RenderTarget is bound to a texture slot
		ID3D11ShaderResourceView* nullSrv[1] = { nullptr };
		GetInmediateContext()->PSSetShaderResources(0, 1, nullSrv);

	}

	void GraphicsDevice::SetDeviceState(DeviceState^ state)
	{

		{
			D3D11_RASTERIZER_DESC m_Rasterizer;

			ZeroMemory(&m_Rasterizer, sizeof(D3D11_RASTERIZER_DESC));

			m_Rasterizer.FillMode = (state->FillModeEnabled ? D3D11_FILL_SOLID : D3D11_FILL_WIREFRAME);
			m_Rasterizer.CullMode = (D3D11_CULL_MODE)state->CullMode;
			/*
			Determines if a triangle is front- or back-facing. 
			If this parameter is TRUE, a triangle will be considered front-facing if its vertices are counter-clockwise on the render target and considered back-facing if they are clockwise.
			If this parameter is FALSE, a triangle will be considered front-facing if its vertices are clockwise on the render target and considered back-facing if they are counter-clockwise.
			*/
			// WE SET THIS VALUE TO TURE BECAUSE THE ENGINE IS USING THE OGL CONVENTIONS. IN D3D, THIS VALUE IS FALSE(Default)
			m_Rasterizer.FrontCounterClockwise = TRUE;
			m_Rasterizer.DepthBias = 0;
			m_Rasterizer.DepthBiasClamp = 0;
			m_Rasterizer.SlopeScaledDepthBias = 0;
			m_Rasterizer.DepthClipEnable = TRUE;
			m_Rasterizer.ScissorEnable = FALSE;
			m_Rasterizer.MultisampleEnable = FALSE;
			m_Rasterizer.AntialiasedLineEnable = FALSE;

			// Check is already exist this state
			for each (_RasterizerState^ stateCreated in m_RasterizerStates)
			{
				if (stateCreated->CompareIftheSame(&m_Rasterizer))
				{
					stateCreated->SetToDevice();

					goto next_1;
				}
			}

			// Does not exist, so, add it

			_RasterizerState^ newState = gcnew _RasterizerState(this);

			newState->Create(&m_Rasterizer);

			newState->SetToDevice();

			m_RasterizerStates.Add(newState);

			InternalLog.WriteLine("GraphicsDevice::SetDeviceState. Created a new RasterizerState");
		}
	next_1:
		{
			D3D11_DEPTH_STENCIL_DESC m_DepthStencil;

			ZeroMemory(&m_DepthStencil, sizeof(D3D11_DEPTH_STENCIL_DESC));

			m_DepthStencil.DepthEnable = (state->DepthEnabled ? TRUE : FALSE);
			m_DepthStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			m_DepthStencil.DepthFunc = D3D11_COMPARISON_LESS;
			m_DepthStencil.StencilEnable = FALSE;
			m_DepthStencil.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
			m_DepthStencil.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
			m_DepthStencil.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			m_DepthStencil.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			m_DepthStencil.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			m_DepthStencil.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			m_DepthStencil.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			m_DepthStencil.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			m_DepthStencil.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			m_DepthStencil.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

			// Check is already exist this state
			for each (_DepthStencilState^ stateCreated in m_DepthStencilStates)
			{
				if (stateCreated->CompareIftheSame(&m_DepthStencil))
				{
					stateCreated->SetToDevice(0);

					goto next_2;
				}
			}

			// Does not exist, so, add it

			_DepthStencilState^ newState = gcnew _DepthStencilState(this);

			newState->Create(&m_DepthStencil);

			newState->SetToDevice(0);

			m_DepthStencilStates.Add(newState);

			InternalLog.WriteLine("GraphicsDevice::SetDeviceState. Created a new DepthStencilState");
		}
	next_2:
		;
	}

	//--------------------------------------------------------------------------------------
	// Clean up the objects we've created
	//--------------------------------------------------------------------------------------
	void GraphicsDevice::Destroy()
	{
		char tmp[200];

		InternalLog.WriteLine("GraphicsDevice::Destroy");

		sprintf(tmp, "GraphicsDevice::Shader::Destroy(%d items)", m_Shaders.Count);
		InternalLog.WriteLine(tmp);

		for each(_Shader^ shader in m_Shaders)
		{
			shader->Destroy();
		}

		SAFE_DESTROY(m_TemporalBuffer);
		SAFE_DESTROY(m_TemporalVB);
		SAFE_DESTROY(m_TemporalIB);
		SAFE_DESTROY(m_QuadMesh);

		sprintf(tmp, "GraphicsDevice::RasterizerStates::Destroy(%d items)", m_RasterizerStates.Count);
		InternalLog.WriteLine(tmp);

		for each(_RasterizerState^ rasterizerState in m_RasterizerStates)
		{
			delete rasterizerState;
		}

		m_RasterizerStates.Clear();

		sprintf(tmp, "GraphicsDevice::DepthStencilStates::Destroy(%d items)", m_DepthStencilStates.Count);
		InternalLog.WriteLine(tmp);

		for each(_DepthStencilState^ depthStencilState in m_DepthStencilStates)
		{
			delete depthStencilState;
		}

		m_DepthStencilStates.Clear();

		sprintf(tmp, "GraphicsDevice::BlenderStates::Destroy(%d items)", m_BlendingStates.Count);
		InternalLog.WriteLine(tmp);

		for each(_BlendingState^ blenderState in m_BlendingStates)
		{
			delete blenderState;
		}

		m_BlendingStates.Clear();

		sprintf(tmp,"GraphicsDevice::SamplerStates::Destroy(%d items)",m_SamplerStates.Count);
		InternalLog.WriteLine(tmp);

		for each(_SamplerState^ samplerState in m_SamplerStates)
		{
			delete samplerState;
		}

		sprintf(tmp, "GraphicsDevice::InputLayout::Destroy(%d items)", m_InputLayouts.Count);
		InternalLog.WriteLine(tmp);

		for each (_InputLayout^ inputLayout in m_InputLayouts)
		{
			inputLayout->Destroy();
		}

		m_InputLayouts.Clear();

		sprintf(tmp, "GraphicsDevice::Textures::Destroy(%d items)", m_Textures.Count);
		InternalLog.WriteLine(tmp);
		
		m_DefaultStates.Destroy();

		// The Texture destructor REMOVES the texture from m_Textures, so we must use this implementation instead the for each
		while (m_Textures.Count>0)
		{
			delete m_Textures[0];
		}
		
		/*
		if (m_ImmediateContext != nullptr)
		{
			InternalLog.WriteLine("GraphicsDevice::ImmediateContext.ClearState");

			m_ImmediateContext->ClearState();
		}
		*/

		if (m_ImmediateContext != nullptr)
		{
			InternalLog.WriteLine("GraphicsDevice::ImmediateContext.Release");

			m_ImmediateContext->Release();
			m_ImmediateContext = nullptr;
		}

		if (m_Device != nullptr)
		{
			InternalLog.WriteLine("GraphicsDevice::Device");

			m_Device->Release();
			m_Device = nullptr;
		}

		if (m_Factory != nullptr)
		{

			InternalLog.WriteLine("GraphicsDevice::Factory");

			m_Factory->Release();
			m_Factory = nullptr;
		}

		SQL::Insert("UPDATE DEVICE SET Destroyed=now() WHERE IDDevice=" + m_BDID);

		GraphicsDeviceCounter--;

		if (GraphicsDeviceCounter == 0)
		{
			InternalLog.WriteLine("GraphicsDevice::LastDeviceCreated. Removing all elements");

			SQL::Destroy();
		}
	}

	void _DefaultStates::Create()
	{
		DepthEnabled = gcnew _DepthStencilState(m_Device);
		DepthEnabled->BuildDefault_Depth(true);

		DepthDisabled = gcnew _DepthStencilState(m_Device);
		DepthDisabled->BuildDefault_Depth(false);

		CullingNone = gcnew _RasterizerState(m_Device);

		CullingNone->CreateCullNone();

		Linear = gcnew _SamplerState(m_Device);
		Linear->CreateDefault();

	}
	
	void _DefaultStates::Destroy()
	{
		DepthEnabled->Destroy();
		DepthDisabled->Destroy();
		Linear->Destroy();
		CullingNone->Destroy();
	}

	//--------------------------------------------------------------------------------------
	// Checks if exists an inputLayout like the one you want. If exists, returns it
	//--------------------------------------------------------------------------------------
	_InputLayout^ GraphicsDevice::AddInputLayoutIfNeeded(_InputLayout^ layout)
	{
		for each (_InputLayout^ inputLayout in m_InputLayouts)
		{
			if (inputLayout->CompareWith(layout))
			{
				// If this layout is repeated, we do not need this layout, so me must to Destroy it!!

				layout->Destroy();

				return inputLayout;
			}
		}

		// Add it to the list

		m_InputLayouts.Add(layout);

		// This layout has not been used yet, so we must update it

		layout->Update();

		return layout;
	}
}
/*
System::String * ToNetString(const std::string & ss)
{
if (ss.empty())
{
return(new System::String(S""));
}
System::IntPtr ptr(static_cast<System::IntPtr>(static_cast<void
*>(const_cast<char *>(ss.c_str()))));
System::String *
ret(System::Runtime::InteropServices::Marshal::Ptr ToStringAnsi(ptr));
return(ret);
}

std::string ToCppString(System::String * str)
{
if (str == 0)
{
return(std::string());
}
System::IntPtr
ptr(System::Runtime::InteropServices::Marshal::Str ingToHGlobalAnsi(str));
std::string ret(static_cast<const char *>(static_cast<void *>(ptr)));
System::Runtime::InteropServices::Marshal::FreeCoT askMem(ptr);
return(ret);
}
*/
