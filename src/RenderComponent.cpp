#include "RenderComponent.h"
#include "Game.h"
#include <array>

RenderComponent::RenderComponent(TransformData& ownerTransform, Vector4 Color, std::vector<Vertex> Vertices, std::vector<std::uint32_t> Indices)
	: GameComponent(ownerTransform, Color),
	vertices(Vertices), indices(Indices)
{}

void RenderComponent::LoadShadersAndLayout()
{
	ID3DBlob* errorVertexCode = nullptr;
	auto res = D3DCompileFromFile(L"./Shaders/MyVeryFirstShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexShaderByteCode,
		&errorVertexCode);
	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(Game::Instance->Display.hWnd, L"MyVeryFirstShader.hlsl", L"Missing Shader File", MB_OK);
		}

		return;
	}
	D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };

	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(L"./Shaders/MyVeryFirstShader.hlsl",
		Shader_Macros /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pixelShaderByteCode,
		&errorPixelCode);

	Game::Instance->Device->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, &vertexShader);

	Game::Instance->Device->CreatePixelShader(
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr, &pixelShader);

	// Input Layout
	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
	D3D11_INPUT_ELEMENT_DESC {
		"POSITION",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		0,
		D3D11_INPUT_PER_VERTEX_DATA,
		0},
	D3D11_INPUT_ELEMENT_DESC {
		"COLOR",
		0,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		0,
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA,
		0}
	};
	Game::Instance->Device->CreateInputLayout(
		inputElements,
		2,
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		&layout);
}

void RenderComponent::LoadGeometry()
{
	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(Vertex) * vertices.size();

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = vertices.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	Game::Instance->Device->CreateBuffer(&vertexBufDesc, &vertexData, &vertexBuffer);

	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(std::uint32_t) * indices.size();

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indices.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	Game::Instance->Device->CreateBuffer(&indexBufDesc, &indexData, &indexBuffer);

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = TRUE;

	auto res = Game::Instance->Device->CreateRasterizerState(&rastDesc, &rastState);
	if(FAILED(res))
	{
		std::cerr << "Failed to create rasterizer state!" << std::endl;
		return;
	}
	
	// constant buffer
	D3D11_BUFFER_DESC constantBuffDesc = {};
	constantBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBuffDesc.MiscFlags = 0;
	constantBuffDesc.StructureByteStride = 0;
	constantBuffDesc.ByteWidth = sizeof(ObjectConstants);
	Game::Instance->Device->CreateBuffer(&constantBuffDesc, nullptr, constantBuffer.GetAddressOf());
}

void RenderComponent::Initialize()
{
	LoadShadersAndLayout();

	LoadGeometry();
}

void RenderComponent::Draw()
{
	Game::Instance->Context->RSSetState(rastState.Get());
	Game::Instance->Context->IASetInputLayout(layout.Get());
	Game::Instance->Context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	UINT strides[] = { sizeof(Vertex) };
	UINT offsets[] = { 0 };
	Game::Instance->Context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), strides, offsets);
	Game::Instance->Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3D11Buffer* buffers[2] = {
	constantBuffer.Get(),
	Game::Instance->ProjectionBuffer.Get()
	};
	Game::Instance->Context->VSSetConstantBuffers(0, 2, buffers);

	Game::Instance->Context->VSSetShader(vertexShader.Get(), nullptr, 0);
	Game::Instance->Context->PSSetShader(pixelShader.Get(), nullptr, 0);
	Game::Instance->Context->DrawIndexed(indices.size(), 0, 0);
}

void RenderComponent::Update(float dt)
{
	ApplyTransform();

	// update constant buffer
	D3D11_MAPPED_SUBRESOURCE res = {};
	Game::Instance->Context->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
	auto dataPtr = reinterpret_cast<float*>(res.pData);
	memcpy(dataPtr, &constantData, sizeof(ObjectConstants));
	Game::Instance->Context->Unmap(constantBuffer.Get(), 0);
}

void RenderComponent::DestroyResources()
{
	vertexBuffer->Release();
	indexBuffer->Release();
	layout->Release();
	pixelShader->Release();
	pixelShaderByteCode->Release();
	rastState->Release();
	vertexShader->Release();
	vertexShaderByteCode->Release();
	constantBuffer->Release();
}