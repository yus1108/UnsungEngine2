#pragma once
#include "dxrframework.h"

namespace UEngine
{
	struct CPU_WORLD
	{
		DirectX::XMMATRIX matrix;
	};

	struct CPU_CAMERA
	{
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	namespace DXRenderer
	{
		struct RASTERIZER_DESC
		{
			bool EnableDepthStencil = false;
			bool EnableMultisampling = false;
			bool EnableAntialisedLine = false;
			D3D11_FILL_MODE FillMode = D3D11_FILL_SOLID;
			D3D11_CULL_MODE CullMode = D3D11_CULL_BACK;
		};

		struct RENDERER_DESC
		{
			bool IsDebuggable = false;
			bool IsFullScreen = false;
			bool EnableDepthStencil = false;
			bool EnableBlendState = false;
			bool EnableAntialisedLine = false;
			bool EnableMultisampling = false;
			UINT FeatureLevelsCount = 0;
			DXGI_SAMPLE_DESC MultisampleDesc = { 1, 0 };
			DXGI_RATIONAL RefreshRate{ 60, 1 };
			D3D_FEATURE_LEVEL* FeatureLevels = nullptr;
			D3D11_FILL_MODE FillMode = D3D11_FILL_SOLID;
			D3D11_CULL_MODE CullMode = D3D11_CULL_BACK;
		};

		struct ViewContext
		{
			D3D11_VIEWPORT Viewport{};
			Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthStencilTexture2D;
			Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilState;
			Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView;

			Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTargetView;
			Microsoft::WRL::ComPtr<ID3D11Texture2D> RenderTargetViewTexture2D;

			Microsoft::WRL::ComPtr<ID3D11Texture2D> OutputTexture2D;
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> OutputShaderResourceView;

			Microsoft::WRL::ComPtr<ID3D11DeviceContext> DeviceContext;
			Microsoft::WRL::ComPtr<ID3D11CommandList> CommandList;
			UINT width{};
			UINT height{};
		};

		struct Pipeline
		{
			Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
			Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
			Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
			Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometryShader;
			Microsoft::WRL::ComPtr<ID3D11HullShader> hullShader;
			Microsoft::WRL::ComPtr<ID3D11DomainShader> domainShader;
			Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
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

		struct CONSTANT_BUFFER_DESC
		{
			std::string TypeName; // Name of the type of the data
			std::size_t Size{ 0 }; // sizeof(data)
			UINT Flag{ 0x00 }; // UENGINE_DXRENDERER_SHADERTYPE
			/*
				Start slot of each pipeline in order of UENGINE_DXRENDERER_SHADERTYPE
				If null, start slots will be set to default which is 0 for all pipeline
				i.e) if Flag = UENGINE_DXRENDERER_SHADERTYPE_VERTEX_SHADER | UENGINE_DXRENDERER_SHADERTYPE_GEOMETRY_SHADER,
				StartSlots = ARRAY{ slot_of_vertex_shader, slot_of_geometry_shader }
			*/
			UINT* StartSlots{ nullptr };
		};

		struct SHARED_RENDERMESH
		{
			class DXRenderMesh* renderMesh;
			std::vector<SIMPLE_VERTEX> vertices;
			std::vector<class DXScene*> scenes;
		};
	}
}

#pragma warning(disable:26812)
enum UENGINE_DXRENDERER_SHADERTYPE : UINT
{
	UENGINE_DXRENDERER_SHADERTYPE_UNKNOWN = 0x00,
	UENGINE_DXRENDERER_SHADERTYPE_VERTEX_SHADER = 0X01,
	UENGINE_DXRENDERER_SHADERTYPE_PIXEL_SHADER = 0X02,
	UENGINE_DXRENDERER_SHADERTYPE_GEOMETRY_SHADER = 0X04,
	UENGINE_DXRENDERER_SHADERTYPE_HULL_SHADER = 0X08,
	UENGINE_DXRENDERER_SHADERTYPE_DOMAIN_SHADER = 0X10,
};