#include "Game.h"


Game* Game::Instance = nullptr; // definiton if the static variable?

Game::Game():
	Input(this), Display(L"My3DApp", this), TotalTime(0.0f)
{
	Instance = this;
	
	Components.push_back(std::make_unique<TriangleComponent>(this, DirectX::XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)));
}

void Game::CreateBackBuffer()
{
    auto res = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);	// __uuidof(ID3D11Texture2D)
	res = Device->CreateRenderTargetView(backBuffer, nullptr, &RenderView);
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
 	std::cout<< "game init\n";
   D3D_FEATURE_LEVEL featureLevel[] = {D3D_FEATURE_LEVEL_11_1};
    
    DXGI_SWAP_CHAIN_DESC swapDesc = {};
    swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = Instance->Display.ClientWidth;
	swapDesc.BufferDesc.Height = Instance->Display.ClientHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;;
	swapDesc.OutputWindow = Instance->Display.hWnd;
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
		&Instance->SwapChain,
		&(Instance->Device),
		nullptr,
		&(Instance->Context));
		
	if(FAILED(res))
	{
		std::cerr << "Failed to create device and swap chain!" << std::endl;
		return false;
	}
	
	
	PrepareResources();
	
	Instance->StartTime = std::chrono::steady_clock::now();
	Instance->PrevTime = Instance->StartTime;
	
	return true;
}

void Game::PrepareResources()
{
	std::cout<< "game res prep\n";
	Instance->CreateBackBuffer();
	
	for(auto& component : Instance->Components)
	{
		std::cout<< "game res prep for component\n";
		component->Initialize();
	}	
}

void Game::Update()
{
	for(auto& component : Instance->Components)
	{
		component->Update();
	}
}

void Game::UpdateInput()
{
	if (Instance->Input.IsKeyDown(Keys::Escape))
	{
		PostQuitMessage(0);
	}
}

void Game::UpdateInternal()
{
	unsigned int frameCount = 0;
	
	auto currentTime = std::chrono::steady_clock::now();
	float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - Instance->PrevTime).count() / 1000000.0f;
	Instance->PrevTime = currentTime;
	
	Instance->TotalTime += deltaTime;
	frameCount++;
	
	if(Instance->TotalTime > 1.0f)
	{
		float fps = frameCount / Instance->TotalTime;
		Instance->TotalTime -= 1.0f;
		
		WCHAR text[256];
		swprintf_s(text, TEXT("FPS: %f"), fps);
		SetWindowText(Instance->Display.hWnd, text); 						

		frameCount = 0;
	}
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

void Game::PrepareFrame()
{
	Instance->Context->ClearState();

	float color[] = {0.08235f, 0.12941f, 0.16471f, 1.0f};
	Instance->Context->ClearRenderTargetView(Instance->RenderView, color);

	// 																				TODO: set viewPort only on resize
	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(Instance->Display.ClientWidth);
	viewport.Height = static_cast<float>(Instance->Display.ClientHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	Instance->Context->RSSetViewports(1, &viewport);
	
	Instance->RestoreTargets();
}

void Game::EndFrame()
{
	Instance->SwapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
	Instance->Context->OMSetRenderTargets(0, nullptr, nullptr);
}

void Game::RestoreTargets()
{
	Context->OMSetRenderTargets(1, &RenderView, nullptr);
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
		UpdateInput();
		Update();
		PrepareFrame();
		Draw();
		EndFrame();
	}
	Instance->Exit();
	return 0;
}

void Game::Exit()
{
	DestroyResources();
	PostQuitMessage(0);
}

void Game::Draw()
{
	for(auto& component : Instance->Components)
	{
		component->Draw();
	}
}

void Game::DestroyResources()
{
	// Instance->Context->ClearState();
	for(auto& component : Instance->Components)
	{
		component->DestroyResources();
	}
}