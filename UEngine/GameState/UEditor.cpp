#include "UEditor.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    UEngine::WinMemoryLeak::Detect();

    UEngine::SingletonManager::Init();

    UEngine::UEditor::EditorState editorState(hInstance);
    editorState.Load();
    int result = editorState.Run(200);

    UEngine::SingletonManager::Release();
    return result;
}