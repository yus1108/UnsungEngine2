#include "dxrframework.h"
#include "DXView.h"

UEngine::DXView* const UEngine::DXView::Instantiate
(
    DXRenderer* renderer,
    UINT width, 
    UINT height,
    bool enableDepthStencil,
    DXGI_SAMPLE_DESC sampleDesc
)
{
    DXView* instance = new DXView;
    DXRenderViewContext* contextAddressOf[] = { &instance->context };
    ZeroMemory(&instance->context, sizeof(UEngine::DXRenderViewContext));
    renderer->InitRenderViewContext(contextAddressOf, 400, 400, enableDepthStencil, sampleDesc);

    return instance;
}

void UEngine::DXView::Release(DXView** const view)
{
    delete* view;
    *view = nullptr;
}

void UEngine::DXView::AddRenderObject(DXRenderObject* const renderObject)
{
    renderObjectList.push_back(renderObject);
}

void UEngine::DXView::Begin()
{
    // clearing depth buffer and render target
    context.DeviceContext->ClearRenderTargetView(context.RenderTargetView.Get(), DirectX::Colors::Transparent);
    if (context.DepthStencilView) context.DeviceContext->ClearDepthStencilView(context.DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

    context.DeviceContext->RSSetViewports(1, &context.Viewport);

    context.DeviceContext->OMSetRenderTargets(1, context.RenderTargetView.GetAddressOf(), context.DepthStencilView.Get());
    context.DeviceContext->OMSetDepthStencilState(context.DepthStencilState.Get(), 1);

    for (size_t i = 0; i < renderObjectList.size(); i++)
    {
        renderObjectList[i]->Set(context.DeviceContext.Get());
        renderObjectList[i]->Draw(context.DeviceContext.Get());
    }

    context.DeviceContext->FinishCommandList(true, context.CommandList.GetAddressOf());
}

void UEngine::DXView::End(ID3D11DeviceContext* deviceContext)
{
    deviceContext->ExecuteCommandList(context.CommandList.Get(), true);
    context.CommandList.ReleaseAndGetAddressOf();

    deviceContext->ResolveSubresource
    (
        (ID3D11Resource*)context.OutputTexture2D.Get(),
        D3D11CalcSubresource(0, 0, 1),
        (ID3D11Resource*)context.RenderTargetViewTexture2D.Get(), 
        D3D11CalcSubresource(0, 0, 1), 
        DXGI_FORMAT_R32G32B32A32_FLOAT
    );
}

