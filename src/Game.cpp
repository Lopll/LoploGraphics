#include "Game.h"

float FOCUS_COLOR[4] = {0.08235f, 0.12941f, 0.16471f, 1.0f};

Game* Game::Instance = nullptr; // definiton if the static variable?

Game::Game(LPCWSTR Name, int width, int height):
	Input(this),
	Display(Name, this, width, height),
	TotalTime(0.0f),
	frameCount(0)
{
	Instance = this;
	
	// aspectRatio = (float)width/(float)height;
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
	// res = Device->CreateShaderResourceView(backBuffer, nullptr, &RenderSRV);
	// if(FAILED(res))
	// {
	// 	std::cerr << "Failed to create shader resource view!" << std::endl;
	// 	return;
	// }
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
	return Matrix::CreateOrthographic((float)Display.ClientWidth, (float)Display.ClientHeight, -100.f, 100.f);
	// return Matrix::CreateOrthographic(2.f, 2.f, -100.f, 100.f);
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

	Matrix proj = CalcProjectionMatrix();
	UpdateProjectionBuffer(proj);
	
	for(auto& component : Components)
	{
		component->Initialize();
	}	
}

void Game::Update()
{
	for(auto& component : Components)
	{
		component->Update();
	}
}

void Game::UpdateInput()
{
	if (Input.IsKeyDown(Keys::Escape))
	{
		PostQuitMessage(0);
	}
}

void Game::UpdateInternal()
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
	Context->ClearRenderTargetView(RenderView.Get(), color);

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
		UpdateInternal();
		PrepareFrame();
		NewImGuiFrame();
		UpdateInput();
		Update();

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

void Game::Resize()
{
	RenderView.Reset();
	backBuffer.Reset();
	
	Context->OMSetRenderTargets(0, nullptr, nullptr);
	
	auto res = SwapChain->ResizeBuffers(0, Display.ClientWidth, Display.ClientHeight, DXGI_FORMAT_UNKNOWN, 0);
	if(FAILED(res))
	{
		std::cerr << "Failed to resize buffers!" << std::endl;
		return;
	}
	
	CreateBackBuffer();
	RestoreTargets();
	
	if (!ProjectionBuffer)
	{
		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.ByteWidth = sizeof(PassConstants);

		Device->CreateBuffer(&desc, nullptr, ProjectionBuffer.GetAddressOf());
	}

	Matrix proj = CalcProjectionMatrix();
	UpdateProjectionBuffer(proj);
	
	ScreenResized = false;
}

void Game::UpdateProjectionBuffer(Matrix proj)
{
	Matrix tProj = proj.Transpose();

	PassConstants temp = { tProj, tProj.Invert() };

	D3D11_MAPPED_SUBRESOURCE mapped = {};
	Context->Map(ProjectionBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	memcpy(mapped.pData, &temp, sizeof(PassConstants));
	Context->Unmap(ProjectionBuffer.Get(), 0);
}