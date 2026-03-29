#include "Game.h"

#include "RectangleComponent.h"
#include "RenderComponent.h"
#include "GeometryGenerator.h"

float FOCUS_COLOR[4] = {0.08235f, 0.12941f, 0.16471f, 1.0f};

Game* Game::Instance = nullptr; // definiton if the static variable?

float cameraMovementSpeed = 1000.f;
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
    
    Entities["WorldBounds"].transform.Scale = Vector3(10.f, 10.f, 100.f);
    Entities["WorldBounds"].transform.Translation = Vector3(0.0f, 0.0f, 0.0f);
    Entities["WorldBounds"].transform.Rotation = Vector3(0.f, 0.0f, 0.0f);
    //Entities["WorldBounds"].AddComponent<RectangleComponent>("Bounds");

	// Variant with Geometry Generator
	GeometryGenerator geoGen;
	GeometryGenerator::MeshData mesh = geoGen.CreateBox(1, 1, 1, 2);
	std::vector<Vertex> vert;
	vert.resize(mesh.Vertices.size());
	for (int i = 0; i < mesh.Vertices.size(); i++)
	{
		vert[i].Pos = mesh.Vertices[i].Position;
		float randCol = ((i % (vert.size() / 2)) + 255) / 255;
		vert[i].Color = { mesh.Vertices[i].TexC.x, mesh.Vertices[i].TexC.y, 0, 1 };
	}
	Entities["TestRenderVariantA"].AddComponent<RenderComponent>("Cube", Color(1,1,1,1), vert, mesh.Indices32);
	Entities["TestRenderVariantA"].transform.Translation = Vector3(-5,0,0);
	Entities["TestRenderVariantA"].transform.Scale = Vector3(20,20,20);
	// End of first variant

	// Variant with no Geometry Generator
	std::vector<Vertex> vertices =
	{
		Vertex({ Vector3(-1.0f, -1.0f, -1.0f), Color(1.f, 1.f, 1.f) }), // White
		Vertex({ Vector3(-1.0f, +1.0f, -1.0f), Color(0.f, 0.f, 0.f) }), // Black
		Vertex({ Vector3(+1.0f, +1.0f, -1.0f), Color(1.f, 0.f, 0.f) }), // Red
		Vertex({ Vector3(+1.0f, -1.0f, -1.0f), Color(0.f, 1.f, 0.f) }), // Green
		Vertex({ Vector3(-1.0f, -1.0f, +1.0f), Color(0.f, 0.f, 1.f) }), // Blue
		Vertex({ Vector3(-1.0f, +1.0f, +1.0f), Color(1.f, 1.f, 0.f) }), // Yellow
		Vertex({ Vector3(+1.0f, +1.0f, +1.0f), Color(0.f, 1.f, 1.f) }), // Cyan
		Vertex({ Vector3(+1.0f, -1.0f, +1.0f), Color(1.f, 0.f, 1.f) })  // Magenta
	};
	std::vector<std::uint32_t> indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};
	Entities["TestRenderVariantB"].AddComponent<RenderComponent>("Cube", Color(1, 1, 1, 1), vertices, indices);
	Entities["TestRenderVariantB"].transform.Scale = Vector3(10, 10, 10);
	Entities["TestRenderVariantB"].transform.Translation = Vector3(25, 0, 0);
	// end of second varitant


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
	
	if (!ProjectionBuffer)
	{
		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.ByteWidth = sizeof(PassConstants);

		Device->CreateBuffer(&desc, nullptr, ProjectionBuffer.GetAddressOf());
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
	// Camera->SetLookAt(Vector3());
	for(auto& component : Components)
	{
		component->Update(dt);
	}
	
}

bool cameraChanged = false;
void Game::UpdateInput()
{
	if (Input.IsKeyDown(Keys::Escape))
	{
		PostQuitMessage(0);
	}
	
	// Change Camera
	if(!cameraChanged && Input.IsKeyDown(Keys::F5))
	{
		cameraChanged = true;
		if(Camera == static_cast<CameraComponent*>(Entities["Orbital_Camera"].GetComponent<OrbitalCameraComponent>("Orbital_Camera")))
		{
			Camera = Entities["FPS_Camera"].GetComponent<CameraComponent>("FPS_Camera");
		}
		else
		{
			Camera = Entities["Orbital_Camera"].GetComponent<OrbitalCameraComponent>("Orbital_Camera");
		}
	}
	else if (cameraChanged && !Input.IsKeyDown(Keys::F5))
	{
		cameraChanged = false;
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
		adj.x +=0.01f * actCameraRotationSpeed;
	}
	if(Input.IsKeyDown(Keys::NumPad4))
	{
		adj.x -=0.01f * actCameraRotationSpeed;
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

void Game::SetViewport()
{
	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(Display.ClientWidth);
	viewport.Height = static_cast<float>(Display.ClientHeight);
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

	SetViewport();
	
	RestoreTargets();
}

void Game::EndFrame()
{
	SwapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
	Context->OMSetRenderTargets(0, nullptr, nullptr);
}

void Game::RestoreTargets()
{
	Context->OMSetRenderTargets(1, RenderView.GetAddressOf(), nullptr);
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
	
	PassConstants temp = { tProj, tProj.Invert(), tView };

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

