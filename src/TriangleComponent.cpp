#include "TriangleComponent.h"

#include "Game.h" 

TriangleComponent::TriangleComponent(Game* gamePtr, std::array<int,3> idx, Vector3 Scale, float Rotation, Vector3 Translation, Vector4 Color)
	: GameComponent(gamePtr, Scale, Rotation, Translation, Color), indices(idx)
{

}

void TriangleComponent::Initialize()
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
			MessageBox(game->Display.hWnd, L"MyVeryFirstShader.hlsl", L"Missing Shader File", MB_OK);
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
	
	game->Device->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, &vertexShader);

	game->Device->CreatePixelShader(
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr, &pixelShader);
	
	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
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
	game->Device->CreateInputLayout(
		inputElements,
		2,
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		&layout);

    D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(Vector4) * std::size(points);

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	game->Device->CreateBuffer(&vertexBufDesc, &vertexData, &vertexBuffer);
	
    D3D11_BUFFER_DESC indexBufDesc = {};
    indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufDesc.CPUAccessFlags = 0;
    indexBufDesc.MiscFlags = 0;
    indexBufDesc.StructureByteStride = 0;
    indexBufDesc.ByteWidth = sizeof(int) * std::size(indices);

    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = indices.data();
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    game->Device->CreateBuffer(&indexBufDesc, &indexData, &indexBuffer);
    
    CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	res = game->Device->CreateRasterizerState(&rastDesc, &rastState);
	
	// constant buffer
	D3D11_BUFFER_DESC constantBuffDesc = {};
	constantBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBuffDesc.MiscFlags = 0;
	constantBuffDesc.StructureByteStride = 0;
	constantBuffDesc.ByteWidth = sizeof(ConstantData);
	game->Device->CreateBuffer(&constantBuffDesc, nullptr, constantBuffer.GetAddressOf());
}

void TriangleComponent::Draw()
{
	game->Context->RSSetState(rastState);
	
	game->Context->IASetInputLayout(layout);
	
	game->Context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	
	UINT strides[] = { 32 };
	UINT offsets[] = { 0 };
	game->Context->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);
	
	game->Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	game->Context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	
	game->Context->VSSetShader(vertexShader, nullptr, 0);
	game->Context->PSSetShader(pixelShader, nullptr, 0);
	
	
	game->Context->DrawIndexed(3, 0, 0);
}

void TriangleComponent::Update()
{
	ApplyTransform(transform);
	
	// update constant buffer
	D3D11_MAPPED_SUBRESOURCE res = {};
	game->Context->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
	auto dataPtr = reinterpret_cast<float*>(res.pData);
	memcpy(dataPtr, &constantData, sizeof(ConstantData));
	game->Context->Unmap(constantBuffer.Get(), 0);
}

void TriangleComponent::DestroyResources()
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