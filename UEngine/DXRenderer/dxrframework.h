#pragma once

#include <Windows.h>
#include <tchar.h>
#include <wrl.h>
#include <string>
#include <unordered_map>
#include <list>
#include <queue>
#include <map>
#include <vector>
#include <stdexcept>
#include <typeinfo>

// DX111 ���̺귯�� �߰�
// D2D1
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")


#include "Texture Loader\DDSTextureLoader.h"
#include "Texture Loader\WICTextureLoader.h"

#include "DXRDefinitions.h"
#include "DXRenderer.h"

#include "RenderComponents\DXShader.h"
#include "RenderComponents\DXRenderMesh.h"
#include "RenderComponents\DXConstantBuffer.h"
#include "RenderComponents\DXRenderObject.h"
#include "RenderComponents\DXView.h"
#include "RenderComponents\DXTexture.h"

#include "ResourceManager\DXScene.h"
#include "ResourceManager\DXResourceManager.h"