#include "Object.h"

#include <iostream>
#include <xutility>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::string GetProjectDir()
{
	std::string fullPath(__FILE__);
	size_t found = fullPath.find_last_of("/\\");
	if (found != std::string::npos)
	{
		std::string projectDir = fullPath.substr(0, found);

		found = projectDir.find_last_of("/\\");
		if (found != std::string::npos)
		{
			return projectDir.substr(0, found + 1);
		}
	}

	return "";
}


void Object::SetProps()
{

	uint32_t stride = sizeof(VertexData);
	uint32_t offset = 0;

	RendererManager::GetRenderer()->GetDeviceContext()->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
	RendererManager::GetRenderer()->GetDeviceContext()->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	RendererManager::GetRenderer()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); 
	RendererManager::GetRenderer()->GetDeviceContext()->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());
	RendererManager::GetRenderer()->GetDeviceContext()->PSSetShaderResources(0, 1, mTextureView.GetAddressOf()); 
	RendererManager::GetRenderer()->GetDeviceContext()->PSSetSamplers(0, 1, mSamplerState.GetAddressOf()); 
}

void Object::UpdateMatrix(XMMATRIX view, XMMATRIX projection)
{
	printf("Cube Position: (%.2f, %.2f, %.2f)\n", mWorldData.Position.x, mWorldData.Position.y, mWorldData.Position.z);
	//mWorldData.Rotation.z += 0.1f; 

	mTransformData.World = XMMatrixTranspose(XMMatrixScaling(mWorldData.Scale.x, mWorldData.Scale.y, mWorldData.Scale.z) *
		XMMatrixRotationRollPitchYaw(XMConvertToRadians(mWorldData.Rotation.x), XMConvertToRadians(mWorldData.Rotation.y), XMConvertToRadians(mWorldData.Rotation.z)) *
		XMMatrixTranslation(mWorldData.Position.x, mWorldData.Position.y, mWorldData.Position.z));	

	mTransformData.View = XMMatrixTranspose(view);
	mTransformData.Projection = XMMatrixTranspose(projection);

	// MAP Data 
	D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
	RendererManager::GetRenderer()->GetDeviceContext()->Map(mConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	memcpy(mappedSubresource.pData, &mTransformData, sizeof(TransformData));
	RendererManager::GetRenderer()->GetDeviceContext()->Unmap(mConstantBuffer.Get(), 0);

}

void Object::InitBuffers()
{

	mWorldData =
	{
		.Position = XMFLOAT3(0.0f, 0.0f, 0.0f),
		.Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f),
		.Scale = XMFLOAT3(1.0f, 1.0f, 1.0f)
	};

	mTransformData =
	{
		.World = XMMatrixIdentity(),
		.View = XMMatrixIdentity(),
		.Projection = XMMatrixIdentity()
	};

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexData) * mIndices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
	vertexSubresourceData.pSysMem = mVertices.data();
	vertexSubresourceData.SysMemPitch = 0;
	vertexSubresourceData.SysMemSlicePitch = 0;

	RendererManager::GetRenderer()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, mVertexBuffer.GetAddressOf());

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(uint32_t) * mIndices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexSubresourceData = {};
	indexSubresourceData.pSysMem = mIndices.data();
	indexSubresourceData.SysMemPitch = 0;
	indexSubresourceData.SysMemSlicePitch = 0;

	RendererManager::GetRenderer()->GetDevice()->CreateBuffer(&indexBufferDesc, &indexSubresourceData, mIndexBuffer.GetAddressOf());

	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.ByteWidth = sizeof(TransformData);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA constantSubresourceData = {};
	constantSubresourceData.pSysMem = &mTransformData;

	RendererManager::GetRenderer()->GetDevice()->CreateBuffer(&constantBufferDesc, &constantSubresourceData, mConstantBuffer.GetAddressOf());

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 1.0f; 
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;

	for (auto& el : samplerDesc.BorderColor)
	{
		el = 0.0f; 
	}

	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	RendererManager::GetRenderer()->GetDevice()->CreateSamplerState(&samplerDesc, mSamplerState.GetAddressOf());

	std::string path = GetProjectDir(); +"Textures\\"; 

	mTextureData = stbi_load((path + "TCom_Wall_Stone3_2x2_2K_albedo.png").c_str(), &mTextureSize.x, &mTextureSize.y, &mTextureSize.z, 4);

	D3D11_TEXTURE2D_DESC textureDesc = {}; 
	textureDesc.Width = mTextureSize.x; 
	textureDesc.Height = mTextureSize.y; 
	textureDesc.MipLevels = 1; 
	textureDesc.ArraySize = 1; 
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  
	textureDesc.SampleDesc.Count = 1; 
	textureDesc.SampleDesc.Quality = 0; 
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE; 
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE; 

	D3D11_SUBRESOURCE_DATA textureSubresourceData = {}; 
	textureSubresourceData.pSysMem = mTextureData; 
	textureSubresourceData.SysMemPitch = mTextureSize.x * 4 * sizeof(uint8_t);

	if (FAILED(RendererManager::GetRenderer()->GetDevice()->CreateTexture2D(&textureDesc, &textureSubresourceData, mTexture.GetAddressOf())))
	{
		printf("Failed to create texture\n"); 
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {}; 
	shaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; 
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D; 
	shaderResourceViewDesc.Texture2D.MipLevels = -1; 
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0; 

	if (FAILED(RendererManager::GetRenderer()->GetDevice()->CreateShaderResourceView(mTexture.Get(), &shaderResourceViewDesc, mTextureView.GetAddressOf())))
	{
		printf("Failed to create texture view\n");
	}

}
