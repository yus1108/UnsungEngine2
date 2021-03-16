#include "UEngine.h"
#include "Material.h"

void UEngine::Material::Awake()
{
	uv = UV{ 0, 0, 1, 1 };
	colorBuffer->AttachData(&color.value, sizeof(Color));
	spriteBuffer->AttachData(&uv.value, sizeof(UV));
	GetGameObject()->GetScene()->ResourceManager.AddResource<DXRenderer::DXConstantBuffer>(colorBuffer->UID, colorBuffer);
	GetGameObject()->GetScene()->ResourceManager.AddResource<DXRenderer::DXConstantBuffer>(spriteBuffer->UID, spriteBuffer);
}

void UEngine::Material::Update()
{
	auto renderComponent = GetComponent<RenderComponent>();
	if (renderComponent == nullptr) return;
	if (renderObject != renderComponent->GetRenderObject())
	{
		renderObject = renderComponent->GetRenderObject();
		if (renderObject == nullptr) return;
		renderComponent->AddConstantBuffer(typeid(Color).raw_name(), colorBuffer);
		renderComponent->AddConstantBuffer(typeid(UV).raw_name(), spriteBuffer);
		if (imageTexture) renderComponent->AddImageTexture(imageTexture);
	}
}

void UEngine::Material::OnPreRender()
{
	colorBuffer->Update(DXRenderer::Get()->GetImmediateDeviceContext());
	spriteBuffer->Update(DXRenderer::Get()->GetImmediateDeviceContext());
}

void UEngine::Material::OnDestroy()
{
	GetGameObject()->GetScene()->ResourceManager.RemoveResource<DXRenderer::DXConstantBuffer>(colorBuffer->UID);
	GetGameObject()->GetScene()->ResourceManager.RemoveResource<DXRenderer::DXConstantBuffer>(spriteBuffer->UID);
}

void UEngine::Material::DeSerialize(TiXmlNode* node)
{
	Serializer::DeSerialize(node);
	std::wstring fileName;
	convert_utf8_to_unicode_string(fileName, this->fileName.value.c_str(), this->fileName.value.size());
	using namespace std;

	std::string delimiter = "\n";
	std::list<std::string> tokens;
	size_t pos = 0;

	while ((pos = samplerName.value.find(delimiter)) != std::string::npos) {
		tokens.emplace_back(samplerName.value.substr(0, pos));
		samplerName.value.erase(0, pos + delimiter.length());
	}

	D3D11_SAMPLER_DESC desc;
	desc.AddressU = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(stoll(tokens.front())); tokens.pop_front();
	desc.AddressV = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(stoll(tokens.front())); tokens.pop_front();
	desc.AddressW = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(stoll(tokens.front())); tokens.pop_front();
	desc.BorderColor[0] = stof(tokens.front()); tokens.pop_front();
	desc.BorderColor[1] = stof(tokens.front()); tokens.pop_front();
	desc.BorderColor[2] = stof(tokens.front()); tokens.pop_front();
	desc.BorderColor[3] = stof(tokens.front()); tokens.pop_front();
	desc.ComparisonFunc = static_cast<D3D11_COMPARISON_FUNC>(stoll(tokens.front())); tokens.pop_front();
	desc.Filter = static_cast<D3D11_FILTER>(stoll(tokens.front())); tokens.pop_front();
	desc.MaxAnisotropy = stoll(tokens.front()); tokens.pop_front();
	desc.MaxLOD = stof(tokens.front()); tokens.pop_front();
	desc.MinLOD = stof(tokens.front()); tokens.pop_front();
	desc.MipLODBias = stof(tokens.front()); tokens.pop_front();
	LoadImageMaterial(fileName, desc);
}

void UEngine::Material::LoadImageMaterial(std::wstring fileName)
{
	// Sampler State
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.MaxLOD = FLT_MAX;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MipLODBias = 0;

	this->fileName = fileName;
	this->samplerName = DXRenderer::DXSamplerState::MakeName(samplerDesc);

	auto scene = GetGameObject()->GetScene();
	imageTexture = scene->ResourceManager.GetResource<DXRenderer::DXTexture>(this->fileName.value);
	if (imageTexture == nullptr)
	{
		imageTexture = DXRenderer::DXTexture::Load(fileName);
		GetGameObject()->GetScene()->ResourceManager.AddResource<DXRenderer::DXTexture>(this->fileName.value, imageTexture);
	}
	imageSampler = scene->ResourceManager.GetResource<DXRenderer::DXSamplerState>(this->fileName.value);
	if (imageTexture == nullptr)
	{
		imageSampler = DXRenderer::DXSamplerState::Load(samplerDesc);
		GetGameObject()->GetScene()->ResourceManager.AddResource<DXRenderer::DXSamplerState>(samplerName.value, imageSampler);
	}
	color = Color{ 1, 1, 1, 1 };
	if (renderObject != nullptr) GetComponent<RenderComponent>()->AddImageTexture(imageTexture);
	if (renderObject != nullptr) GetComponent<RenderComponent>()->AddImageSampler(imageSampler);
}

void UEngine::Material::LoadImageMaterial(std::wstring fileName, D3D11_TEXTURE_ADDRESS_MODE addressMode, D3D11_FILTER filter)
{
	// Sampler State
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.AddressU = addressMode;
	samplerDesc.AddressV = addressMode;
	samplerDesc.AddressW = addressMode;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.Filter = filter;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.MaxLOD = FLT_MAX;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MipLODBias = 0;

	this->fileName = fileName;
	this->samplerName = DXRenderer::DXSamplerState::MakeName(samplerDesc);

	auto scene = GetGameObject()->GetScene();
	if (imageTexture == nullptr)
	{
		imageTexture = DXRenderer::DXTexture::Load(fileName);
		GetGameObject()->GetScene()->ResourceManager.AddResource<DXRenderer::DXTexture>(this->fileName.value, imageTexture);
	}
	imageSampler = scene->ResourceManager.GetResource<DXRenderer::DXSamplerState>(this->fileName.value);
	if (imageTexture == nullptr)
	{
		imageSampler = DXRenderer::DXSamplerState::Load(samplerDesc);
		GetGameObject()->GetScene()->ResourceManager.AddResource<DXRenderer::DXSamplerState>(samplerName.value, imageSampler);
	}
	color = Color{ 1, 1, 1, 1 };
	if (renderObject != nullptr) GetComponent<RenderComponent>()->AddImageTexture(imageTexture);
	if (renderObject != nullptr) GetComponent<RenderComponent>()->AddImageSampler(imageSampler);
}

void UEngine::Material::LoadImageMaterial(std::wstring fileName, D3D11_SAMPLER_DESC desc)
{
	this->fileName = fileName;
	this->samplerName = DXRenderer::DXSamplerState::MakeName(desc);

	auto scene = GetGameObject()->GetScene();
	imageTexture = scene->ResourceManager.GetResource<DXRenderer::DXTexture>(this->fileName.value);
	if (imageTexture == nullptr)
	{
		imageTexture = DXRenderer::DXTexture::Load(fileName);
		GetGameObject()->GetScene()->ResourceManager.AddResource<DXRenderer::DXTexture>(this->fileName.value, imageTexture);
	}
	imageSampler = scene->ResourceManager.GetResource<DXRenderer::DXSamplerState>(this->fileName.value);
	if (imageTexture == nullptr)
	{
		imageSampler = DXRenderer::DXSamplerState::Load(desc);
		GetGameObject()->GetScene()->ResourceManager.AddResource<DXRenderer::DXSamplerState>(samplerName.value, imageSampler);
	}
	color = Color{ 1, 1, 1, 1 };
	if (renderObject != nullptr) GetComponent<RenderComponent>()->AddImageTexture(imageTexture);
	if (renderObject != nullptr) GetComponent<RenderComponent>()->AddImageSampler(imageSampler);
}
