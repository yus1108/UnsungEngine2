#include "dxrframework.h"
#include "DXView.h"

namespace UEngine
{
    namespace DXRenderer
    {
        long long DXView::nextId = 0;

        DXView* const DXView::Instantiate
        (
            DXRenderer* renderer,
            UINT width,
            UINT height,
            bool EnableDepthStencil,
            DXGI_SAMPLE_DESC sampleDesc
        )
        {
            DXView* instance = new DXView;
            ViewContext* contextAddressOf[] = { &instance->context };
            ZeroMemory(&instance->context, sizeof(ViewContext));
            renderer->InitRenderViewContext(contextAddressOf, width, height, EnableDepthStencil, sampleDesc);

            return instance;
        }

        void DXView::Release(DXView* const view)
        {
            view->context.CommandList.ReleaseAndGetAddressOf();
            delete view;
        }

        void DXView::Release(DXView** const view)
        {
            (*view)->context.CommandList.ReleaseAndGetAddressOf();
            delete* view;
            *view = nullptr;
        }

        void DXView::Begin()
        {
            // clearing depth buffer and render target
            context.DeviceContext->ClearRenderTargetView(context.RenderTargetView.Get(), DirectX::Colors::Transparent);
            if (context.DepthStencilView) context.DeviceContext->ClearDepthStencilView(context.DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

            context.DeviceContext->RSSetViewports(1, &context.Viewport);

            context.DeviceContext->OMSetRenderTargets(1, context.RenderTargetView.GetAddressOf(), context.DepthStencilView.Get());
            context.DeviceContext->OMSetDepthStencilState(context.DepthStencilState.Get(), 1);
        }

        void DXView::End()
        {
            context.DeviceContext->FinishCommandList(true, context.CommandList.GetAddressOf());
        }

        void DXView::Execute(ID3D11DeviceContext* deviceContext)
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


    }
}