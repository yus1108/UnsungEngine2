#pragma once
#include "dxrframework.h"

namespace UEngine
{
	struct DXRasterDesc
	{
		bool ScissorEnable = false;
		bool DepthClipEnable = false;
		bool MultiSampleEnable = false;
		bool AntialiasedLineEnable = false;
		D3D11_FILL_MODE FillMode = D3D11_FILL_SOLID;
		D3D11_CULL_MODE CullMode = D3D11_CULL_BACK;
	};

	struct DXRenderingDesc
	{
		bool IsDebuggable = false;
		bool IsFullScreen = false;
		UINT FeatureLevelsCount = 0;
		DXGI_RATIONAL RefreshRate{ 60, 1 };
		D3D_FEATURE_LEVEL* FeatureLevels = nullptr;
		DXGI_SAMPLE_DESC MultisampleDesc = { 1, 0 };
		DXRasterDesc RasterizerStateDesc;
	};

	struct DXRenderViewContext
	{
		D3D11_VIEWPORT Viewport{};
		Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthStencilTexture2D;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilState;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> RenderTargetViewTexture2D;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTargetViewMap;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShaderResourceView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> OutputTexture2D;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> OutputShaderResourceView;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> DeviceContext;
		Microsoft::WRL::ComPtr<ID3D11CommandList> CommandList;
		UINT width{};
		UINT height{};
	};

	struct DXRPipeline
	{
		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometryShader;
		Microsoft::WRL::ComPtr<ID3D11HullShader> hullShader;
		Microsoft::WRL::ComPtr<ID3D11DomainShader> domainShader;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
		Microsoft::WRL::ComPtr<ID3D11BlendState> blendingState;
	};

	struct SIMPLE_VERTEX {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 tex;
		DirectX::XMFLOAT3 normals;
		DirectX::XMFLOAT3 tangents;
		DirectX::XMFLOAT3 binomals;
		DirectX::XMFLOAT4 weights;
		DirectX::XMUINT4 joints;
	};

	enum class ShaderType : UINT
	{
		VERTEX_SHADER,
		PIXEL_SHADER,
		GEOMETRY_SHADER,
		HULL_SHADER,
		DOMAIN_SHADER,
		COUNT
	};
}