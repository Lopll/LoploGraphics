#include "Game.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

float FOCUS_COLOR[4] = {0.08235f, 0.12941f, 0.16471f, 1.0f};

Game* Game::Instance = nullptr; // definiton if the static variable?

float cameraMovementSpeed = 5000.f;
float actCameraMovementSpeed = cameraMovementSpeed;
float cameraRotationSpeed = 100.f;
float actCameraRotationSpeed = cameraRotationSpeed;

Game::Game(LPCWSTR Name, int width, int height):
	Input(this),
	Display(Name, this, width, height),
	TotalTime(0.0f),
	frameCount(0)
{
	Instance = this;
	
	aspectRatio = (float)width/(float)height;
	// Cameras setup
    Entities["FPS_Camera"].transform.Scale = Vector3(1.f, 1.f, 1.f);
	Entities["FPS_Camera"].transform.Translation = Vector3(0.f, 0.f, 30.f);
	Camera = Entities["FPS_Camera"].AddComponent<CameraComponent>("FPS_Camera");
	Camera->SetProjectionValues(fov, aspectRatio, nearZ, farZ);
    Camera->SetLookAt(Vector3());
	
	Entities["Orbital_Camera"].transform.Scale = Vector3(1.f, 1.f, 1.f);
	Entities["Orbital_Camera"].transform.Translation = Vector3(0.f, 0.f, 0.f);
    Camera = Entities["Orbital_Camera"].AddComponent<OrbitalCameraComponent>("Orbital_Camera");
    Camera->SetProjectionValues(fov, aspectRatio, nearZ, farZ);
    Camera->SetLookAt(Vector3());
    
    directionalLight = LightSource(Vector3(0.5f, -0.5f, 0.2f), 1.f);
}

void Game::CreateBackBuffer()
{
    auto res = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());	// __uuidof(ID3D11Texture2D)
	res = Device->CreateRenderTargetView(backBuffer.Get(), nullptr, RenderView.GetAddressOf());
	if(FAILED(res))
	{
		std::cerr << "Failed to create render target view!" << std::endl;
		return;
	}
}

bool Game::Initialize()
{
	D3D_FEATURE_LEVEL featureLevel[] = {D3D_FEATURE_LEVEL_11_1};
    
    DXGI_SWAP_CHAIN_DESC swapDesc = {};
    swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = Display.ClientWidth;
	swapDesc.BufferDesc.Height = Display.ClientHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;;
	swapDesc.OutputWindow = Display.hWnd;
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	
	auto res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapDesc,
		SwapChain.GetAddressOf(),
		Device.GetAddressOf(),
		nullptr,
		Context.GetAddressOf());
		
	if(FAILED(res))
	{
		std::cerr << "Failed to create device and swap chain!" << std::endl;
		return false;
	}
	
	
	PrepareResources();
	
	InitImGui();
	
	StartTime = std::chrono::steady_clock::now();
	PrevTime = StartTime;
	
	
	return true;
}

Matrix Game::CalcProjectionMatrix()
{
	Camera->SetProjectionValues(fov, aspectRatio, nearZ, farZ);
	return Camera->projection;
}

void Game::PrepareResources()
{
	CreateBackBuffer();
	
	if(!DepthBuffer)
	{
		D3D11_TEXTURE2D_DESC depthDesc = {};
		depthDesc.ArraySize = 1;
		depthDesc.MipLevels = 1;
		depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthDesc.CPUAccessFlags = 0;
		depthDesc.MiscFlags = 0;
		depthDesc.Usage = D3D11_USAGE_DEFAULT;
		depthDesc.Width = Display.ClientWidth;
		depthDesc.Height = Display.ClientHeight;
		depthDesc.SampleDesc = {1, 0};
		
		Device->CreateTexture2D(&depthDesc, nullptr, &DepthBuffer);
		Device->CreateDepthStencilView(DepthBuffer.Get(), nullptr, &DepthView);
	}
	
	if(!ProjectionBuffer)
	{
		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.ByteWidth = sizeof(PassConstants);

		Device->CreateBuffer(&desc, nullptr, ProjectionBuffer.GetAddressOf());
	}
	
	if(!LightBuffer)
	{
		D3D11_BUFFER_DESC lightBuffDesc = {};
		lightBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
		lightBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		lightBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		lightBuffDesc.ByteWidth = sizeof(LightPass);
		Device->CreateBuffer(&lightBuffDesc, nullptr, LightBuffer.GetAddressOf());
	}
	
	if(!ShadowMap)
	{
		D3D11_TEXTURE2D_DESC shadowMapDesc = {};
		shadowMapDesc.Width = 8192;
		shadowMapDesc.Height = 8192;
		shadowMapDesc.MipLevels = 1;
		shadowMapDesc.ArraySize = 1;
		shadowMapDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		shadowMapDesc.SampleDesc.Count = 1;
		shadowMapDesc.SampleDesc.Quality = 0;
		shadowMapDesc.Usage = D3D11_USAGE_DEFAULT;
		shadowMapDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		shadowMapDesc.CPUAccessFlags = 0;
		shadowMapDesc.MiscFlags = 0;
		
		Device->CreateTexture2D(&shadowMapDesc, nullptr, &ShadowMap);

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;
		Device->CreateDepthStencilView(ShadowMap.Get(), &dsvDesc, &ShadowMapDepthView);
		
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;
		Device->CreateShaderResourceView(ShadowMap.Get(), &srvDesc, &ShadowMapResourceView);
		
		D3D11_SAMPLER_DESC shadowSamplerDesc = {};
		shadowSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		shadowSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		shadowSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		shadowSamplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
		shadowSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		shadowSamplerDesc.BorderColor[0] = 1.0f;
		shadowSamplerDesc.BorderColor[1] = 1.0f;
		shadowSamplerDesc.BorderColor[2] = 1.0f;
		shadowSamplerDesc.BorderColor[3] = 1.0f;
		// shadowSamplerDesc.MaxLOD = INT_MAX;
		Device->CreateSamplerState(&shadowSamplerDesc, &ShadowSampler);
		
		CD3D11_RASTERIZER_DESC shadowRastDesc = {};
		shadowRastDesc.CullMode = D3D11_CULL_FRONT;
		shadowRastDesc.FillMode = D3D11_FILL_SOLID;
		shadowRastDesc.FrontCounterClockwise = TRUE;
		shadowRastDesc.DepthBias = 100;
		shadowRastDesc.DepthBiasClamp = 0.0f;
		shadowRastDesc.SlopeScaledDepthBias = 0.5f;
		Device->CreateRasterizerState(&shadowRastDesc, &ShadowRS);
	}

	UpdateProjectionBuffer(Camera->projection, Camera->view);
	
	for(auto& component : Components)
	{
		component->Initialize();
	}
	
	
}

void Game::Update(float dt)
{
	actCameraMovementSpeed = cameraMovementSpeed * dt;
	actCameraRotationSpeed = cameraRotationSpeed * dt;
	for(auto& component : Components)
	{
		component->Update(dt);
	}
	
	// Light update
	// D3D11_MAPPED_SUBRESOURCE mapped = {};
	// Context->Map(LightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	// LightPass lightData;
	// lightData.LightIntencity = directionalLight.Intencity;
	// lightData.LightDirection = directionalLight.Direction;
	// lightData.LightDirection.Normalize();
	// memcpy(mapped.pData, &lightData, sizeof(LightPass));
	// Context->Unmap(LightBuffer.Get(), 0);
}

void Game::UpdateInput()
{
	if (Input.IsKeyDown(Keys::Escape))
	{
		PostQuitMessage(0);
	}
	
	// toggle wireframe
	if (Input.IsKeyDown(Keys::F1))
	{
    	wireframe = !wireframe;
		for (auto& component : Components)
	    {
	        auto* renderComp = dynamic_cast<RenderComponent*>(component.get());
	        if (renderComp)
	        {
	            renderComp->wireframe = wireframe;
	        }
	    }
	    Sleep(100);
	}
	
	// Change Camera
	if(Input.IsKeyDown(Keys::F5))
	{
		if(Camera == static_cast<CameraComponent*>(Entities["Orbital_Camera"].GetComponent<OrbitalCameraComponent>("Orbital_Camera")))
		{
			Camera = Entities["FPS_Camera"].GetComponent<CameraComponent>("FPS_Camera");
		}
		else
		{
			Camera = Entities["Orbital_Camera"].GetComponent<OrbitalCameraComponent>("Orbital_Camera");
		}
	    Sleep(100);
	}
	
	// Move Camera
	Vector3 adj;
	if(Input.IsKeyDown(Keys::W))
	{
		adj.z -= 0.01f * actCameraMovementSpeed;
	}
	if(Input.IsKeyDown(Keys::S))
	{
		adj.z += 0.01f * actCameraMovementSpeed;
	}
	if(Input.IsKeyDown(Keys::D))
	{
		adj.x += 0.01f * actCameraMovementSpeed;
	}
	if(Input.IsKeyDown(Keys::A))
	{
		adj.x -= 0.01f * actCameraMovementSpeed;
	}
	if(Input.IsKeyDown(Keys::E))
	{
		adj.y += 0.01f * actCameraMovementSpeed;
	}
	if(Input.IsKeyDown(Keys::Q))
	{
		adj.y -= 0.01f * actCameraMovementSpeed;
	}
	Camera->AdjustPosition(adj);
	
	// Rotate Camera
	adj = Vector3();
	if(Input.IsKeyDown(Keys::NumPad6))
	{
		if(Camera == static_cast<CameraComponent*>(Entities["Orbital_Camera"].GetComponent<OrbitalCameraComponent>("Orbital_Camera")))
		{
			adj.x +=0.01f * actCameraRotationSpeed;
		}
		else
		{
			adj.x -=0.01f * actCameraRotationSpeed;
		}
		
	}
	if(Input.IsKeyDown(Keys::NumPad4))
	{
		if(Camera == static_cast<CameraComponent*>(Entities["Orbital_Camera"].GetComponent<OrbitalCameraComponent>("Orbital_Camera")))
		{
			adj.x -=0.01f * actCameraRotationSpeed;
		}
		else
		{
			adj.x +=0.01f * actCameraRotationSpeed;
		}
	}
	if(Input.IsKeyDown(Keys::NumPad8))
	{
		if(Camera == static_cast<CameraComponent*>(Entities["Orbital_Camera"].GetComponent<OrbitalCameraComponent>("Orbital_Camera")))
		{
			adj.z +=0.01f * actCameraRotationSpeed;
		}
		else
		{
			adj.y +=0.01f * actCameraRotationSpeed;
		}
		
	}
	if(Input.IsKeyDown(Keys::NumPad5))
	{
		if(Camera == static_cast<CameraComponent*>(Entities["Orbital_Camera"].GetComponent<OrbitalCameraComponent>("Orbital_Camera")))
		{
			adj.z -=0.01f * actCameraRotationSpeed;
		}
		else
		{
			adj.y -=0.01f * actCameraRotationSpeed;
		}
		
	}
	Camera->AdjustRotation(adj);
	
	
}

float Game::UpdateInternal()
{
	if(ScreenResized)
	{
		Resize();
	}
	auto currentTime = std::chrono::steady_clock::now();
	float deltaTime = std::chrono::duration<float>(
		currentTime - PrevTime
	).count();

	PrevTime = currentTime;
	TotalTime += deltaTime;
	frameCount++;

	if (TotalTime >= 1.0f)
	{
		float fps = static_cast<float>(frameCount) / TotalTime;
		TotalTime -= 1.0f;

		WCHAR text[256];
		swprintf_s(text, L"FPS: %.1f", fps);
		SetWindowText(Display.hWnd, text);

		frameCount = 0;
	}
	return deltaTime;
}

void Game::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // если хочешь

	ImGui::StyleColorsDark();  // или Light, Classic...

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(Display.hWnd);
	ImGui_ImplDX11_Init(Device.Get(), Context.Get());
}

void Game::ShutdownImGui()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Game::NewImGuiFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Game::RenderImGui()
{
	ImGui::Render();
	//Context->OMSetRenderTargets(1, &RenderView, nullptr);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

bool Game::MessageHandler()
{
	MSG msg = {};
	// Handle the windows messages.
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	// If windows signals to end the application then exit out.
	if (msg.message == WM_QUIT) {
		return true;
	}
	return false;
}

void Game::SetViewport(float w, float h)
{
	D3D11_VIEWPORT viewport = {};
	viewport.Width = w;
	viewport.Height = h;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	Context->RSSetViewports(1, &viewport);
}

void Game::PrepareFrame()
{
	Context->ClearState();

	float color[] = {0.0f, 0.0f, 0.0f, 1.0f};
	Context->ClearRenderTargetView(RenderView.Get(), FOCUS_COLOR);
	Context->ClearDepthStencilView(DepthView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	SetViewport(static_cast<float>(Display.ClientWidth), static_cast<float>(Display.ClientHeight));
	
	Context->OMSetRenderTargets(1, RenderView.GetAddressOf(), DepthView.Get());
}

void Game::PrepareShadowFrame()
{
	Context->ClearDepthStencilView(ShadowMapDepthView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
	SetViewport(8192.f, 8192.f);
	
	Context->RSSetState(ShadowRS.Get());
	
	Context->PSSetShader(nullptr, nullptr, 0);
	Context->OMSetRenderTargets(0, nullptr, ShadowMapDepthView.Get());
}

void Game::EndFrame()
{
	SwapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
	Context->OMSetRenderTargets(0, nullptr, nullptr);
}

void Game::RestoreTargets()
{
	Context->OMSetRenderTargets(1, RenderView.GetAddressOf(), DepthView.Get());
}

int Game::Run()
{
	if (!Initialize()) 
	{
		return 1;
	}
	
	while(!MessageHandler())
	{
		float dt = UpdateInternal();
		// Draw Shadow Map
		PrepareShadowFrame();
		
		// ligth
		D3D11_MAPPED_SUBRESOURCE mapped = {};
		Context->Map(LightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
		LightPass lightData;
		lightData.LightIntencity = directionalLight.Intencity;
		lightData.LightDirection = directionalLight.Direction;
		lightData.LightDirection.Normalize();
		
		Vector3 lightPos = directionalLight.Direction * 2000.f;
		lightPos.y *= -1;
		Matrix lightView = Matrix::CreateLookAt(lightPos, Vector3(), Vector3::Up);
		Matrix lightProjection = Matrix::CreateOrthographic(2000.f, 2000.f, 0.1f, 2000.f);
		lightData.LightView = (lightView * lightProjection).Transpose();
		
		memcpy(mapped.pData, &lightData, sizeof(LightPass));
		Context->Unmap(LightBuffer.Get(), 0);
		
		UpdateProjectionBuffer(lightProjection, lightView);
		DrawShadow();
		
		PrepareFrame();
		NewImGuiFrame();
		UpdateInput();
		Update(dt);
		UpdateProjectionBuffer(Camera->projection, Camera->view);

		Draw();
		RenderImGui();
		EndFrame();
	}
	Exit();
	return 0;
}

void Game::Exit()
{
	ShutdownImGui();
	DestroyResources();
	PostQuitMessage(0);
}

void Game::Draw()
{
	for(auto& component : Components)
	{
		component->Draw();
	}
}

void Game::DrawShadow()
{
	for(auto& component : Components)
	{
		component->DrawShadow();
	}
}

void Game::DestroyResources()
{
	// Context->ClearState();
	for(auto& component : Components)
	{
		component->DestroyResources();
	}
}

void Game::UpdateProjectionBuffer(Matrix proj, Matrix view)
{
	Matrix tProj = proj.Transpose();
	Matrix tView = view.Transpose();
	
	PassConstants temp = { tProj, tProj.Invert(), tView, tView.Invert() };

	D3D11_MAPPED_SUBRESOURCE mapped = {};
	Context->Map(ProjectionBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	memcpy(mapped.pData, &temp, sizeof(PassConstants));
	Context->Unmap(ProjectionBuffer.Get(), 0);
}

// LEGACY
void Game::Resize()
{
	// RenderView.Reset();
	// backBuffer.Reset();
	
	// Context->OMSetRenderTargets(0, nullptr, nullptr);
	
	// auto res = SwapChain->ResizeBuffers(0, Display.ClientWidth, Display.ClientHeight, DXGI_FORMAT_UNKNOWN, 0);
	// if(FAILED(res))
	// {
	// 	std::cerr << "Failed to resize buffers!" << std::endl;
	// 	return;
	// }
	
	// CreateBackBuffer();
	// RestoreTargets();
	
	// if (!ProjectionBuffer)
	// {
	// 	D3D11_BUFFER_DESC desc = {};
	// 	desc.Usage = D3D11_USAGE_DYNAMIC;
	// 	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	// 	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// 	desc.ByteWidth = sizeof(PassConstants);

	// 	Device->CreateBuffer(&desc, nullptr, ProjectionBuffer.GetAddressOf());
	// }

	// Matrix proj = CalcProjectionMatrix();
	// // UpdateProjectionBuffer(proj);
	
	// ScreenResized = false;
}

bool Game::LoadOBJModel(const std::string filepath, std::vector<Vertex>& outVertices, std::vector<std::uint32_t>& outIndices, Vector4 defaultColor)
{
	tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    
    std::string objDirectory = filepath.substr(0, filepath.find_last_of("/\\") + 1);
    
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filepath.c_str(), objDirectory.c_str(), true);          
    
	if (!err.empty()) {
	    std::cerr << "OBJ Error: " << err << std::endl;
	    return false;
	}
    
	if (!ret) {
	    std::cerr << "Failed to load OBJ file: " << filepath << std::endl;
	    return false;
	}
    
	for (const auto& shape : shapes) {
	    for (const auto& index : shape.mesh.indices) {
	        Vertex vertex;
            
	        vertex.Pos.x = attrib.vertices[3 * index.vertex_index + 0];
	        vertex.Pos.y = attrib.vertices[3 * index.vertex_index + 1];
	        vertex.Pos.z = attrib.vertices[3 * index.vertex_index + 2];
            
	        vertex.Color = defaultColor;
	        
	        if(index.texcoord_index >= 0 && attrib.texcoords.size() > 0)
	        {
	        	vertex.TexCoord.x = attrib.texcoords[2*index.texcoord_index + 0];
	        	vertex.TexCoord.y = 1.0f - attrib.texcoords[2*index.texcoord_index + 1];
	        }
	        else
	        {
	        	vertex.TexCoord = Vector2(0,0);
	        }
	        
	        if (index.normal_index >= 0 && attrib.normals.size() > 0)
			{
			    vertex.Normal.x = attrib.normals[3 * index.normal_index + 0];
			    vertex.Normal.y = attrib.normals[3 * index.normal_index + 1];
			    vertex.Normal.z = attrib.normals[3 * index.normal_index + 2];
			}
			else
			{
			    vertex.Normal = Vector3(0, 1, 0); 
			}
            
	        outVertices.push_back(vertex);
	        outIndices.push_back(static_cast<std::uint32_t>(outIndices.size()));
	    }
	}
	
	// center model
	Vector3 minV(FLT_MAX, FLT_MAX, FLT_MAX);
    Vector3 maxV(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    for (const auto& v : outVertices) {
        minV.x = min(minV.x, v.Pos.x);
        minV.y = min(minV.y, v.Pos.y);
        minV.z = min(minV.z, v.Pos.z);
        
        maxV.x = max(maxV.x, v.Pos.x);
        maxV.y = max(maxV.y, v.Pos.y);
        maxV.z = max(maxV.z, v.Pos.z);
    }
    Vector3 center = (minV + maxV) * 0.5f;
    for (auto& v : outVertices) {
        v.Pos -= center;
    }
    
	return true;
}

void Game::RemoveComponentFromEntity(const std::string& entityName, const std::string& componentName)
{
    auto entityIt = Entities.find(entityName);
    if (entityIt == Entities.end()) return;
    
    Entity& entity = entityIt->second;
    auto compIt = entity.components.find(componentName);
    if (compIt == entity.components.end()) return;
    
    GameComponent* component = compIt->second;
    
    auto globalIt = std::find_if(Components.begin(), Components.end(),
        [component](const std::unique_ptr<GameComponent>& comp) {
            return comp.get() == component;
        });
    
    if (globalIt != Components.end())
    {
        component->DestroyResources();
        Components.erase(globalIt);
    }
    
    entity.components.erase(compIt);
}