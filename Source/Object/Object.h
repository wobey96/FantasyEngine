#pragma once 

#include "Renderer/RendererManager.h"
#include "Object/DataStructures.h"
#include <vector>

class Object
{	
public:
	Object() = default; 
	~Object() = default; 

	void SetProps();
	void SetPosition(XMFLOAT3 position) { mWorldData.Position = position; }
	void setRotation(XMFLOAT3 rotation) { mWorldData.Rotation = rotation; }
	void SetScale(XMFLOAT3 scale) { mWorldData.Scale = scale; }
	void UpdateMatrix(XMMATRIX view, XMMATRIX projection); 
	uint32_t GetIndexCount() const { return mIndices.size(); }	

	virtual void Update() = 0; 

protected: 
	void InitBuffers(); 

	std::vector<VertexData> mVertices;
	std::vector<uint32_t> mIndices;
	WorldData mWorldData;
private:
	ComPtr<ID3D11Buffer> mVertexBuffer;
	ComPtr<ID3D11Buffer> mIndexBuffer;

	ComPtr<ID3D11Buffer> mConstantBuffer; 

	VertexData mVertexData;
	TransformData mTransformData;

	uint8_t* mTextureData;
	XMINT3 mTextureSize;
	ComPtr<ID3D11Texture2D> mTexture; 
	ComPtr<ID3D11ShaderResourceView> mTextureView;
	ComPtr<ID3D11SamplerState> mSamplerState; 
};