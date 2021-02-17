#pragma once
#include "dxrframework.h"

namespace UEngine
{
	struct MultiSampleDesc {
		bool enable;
		DXGI_SAMPLE_DESC sample_description;
	};

	struct DXRenderingDesc
	{
		bool IsDebuggable = false;
		bool IsFullScreen = false;
		bool ScissorEnable = false;
		bool AntialiasedLineEnable = false;
		D3D11_FILL_MODE FillMode = D3D11_FILL_SOLID;
		D3D11_CULL_MODE CullMode = D3D11_CULL_BACK;
		DXGI_RATIONAL RefreshRate{ 60, 1 };
		MultiSampleDesc MultiSampleDesc{ false, {1, 0} };
		D3D_FEATURE_LEVEL* FeatureLevels = nullptr;
		UINT FeatureLevelsCount = 0;
	};

	struct DXRenderViewResource
	{
		D3D11_VIEWPORT viewport{};
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> render_target_texture_map;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view_map;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> output_texture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> output_shader_resource_view;
		UINT width{};
		UINT height{};

		DXRenderViewResource() {}
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