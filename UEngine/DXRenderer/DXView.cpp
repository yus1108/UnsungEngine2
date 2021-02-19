#include "dxrframework.h"
#include "DXView.h"

UEngine::DXView* const UEngine::DXView::Instantiate
(
    DXRenderer* renderer,
    UINT width, 
    UINT height
)
{
    DXView* instance = new DXView;
    DXRenderViewContext* contextAddressOf[] = { &instance->context };
    ZeroMemory(&instance->context, sizeof(UEngine::DXRenderViewContext));
    renderer->InitRenderViewContext(contextAddressOf, 400, 400);

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

void UEngine::DXView::Set()
{
    // clearing depth buffer and render target
    context.DeviceContext->ClearRenderTargetView(context.RenderTargetView.Get(), DirectX::Colors::Transparent);
    context.DeviceContext->RSSetViewports(1, &context.Viewport);

    context.DeviceContext->OMSetRenderTargets(1, context.RenderTargetView.GetAddressOf(), context.DepthStencilView.Get());
    context.DeviceContext->OMSetDepthStencilState(context.DepthStencilState.Get(), 1);

    for (size_t i = 0; i < renderObjectList.size(); i++)
    {
        renderObjectList[i]->Set(context.DeviceContext.Get());
        renderObjectList[i]->Draw(context.DeviceContext.Get());
    }
}

void UEngine::DXView::Begin()
{
    context.DeviceContext->FinishCommandList(true, context.CommandList.GetAddressOf());
}

void UEngine::DXView::End(ID3D11DeviceContext* deviceContext)
{
    deviceContext->ExecuteCommandList(context.CommandList.Get(), true);
    context.CommandList.ReleaseAndGetAddressOf();
}

