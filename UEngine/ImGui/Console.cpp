#include <string>
#include "ImGui.h"
#include "Console.h"

namespace UEngine
{
    Console* Console::instance = nullptr;

    void Console::Write(std::string log)
    {
		Write({ LOG_TYPE::LOG, log });
    }

    void Console::Write(Message log)
    {
        instance->logs.push_back(log);
        instance->gotoEnd = true;
    }

    void Console::WriteLine(std::string log)
    {
        Write({ LOG_TYPE::LOG, log + "\n" });
    }

    void Console::WriteLine(float log)
    {
        WriteLine(std::to_string(log));
    }

    void Console::WriteWarning(std::string log)
    {
        Write({ LOG_TYPE::WARNING, log });
        instance->warningCounter++;
    }

    void Console::WriteWarningLine(std::string log)
    {
        Write({ LOG_TYPE::WARNING, log + "\n" });
        instance->warningCounter++;
    }

    void Console::WriteError(std::string log)
    {
        Write({ LOG_TYPE::ERROR_LOG, log });
        instance->errorCounter++;
    }

    void Console::WriteErrorLine(std::string log)
    {
        Write({ LOG_TYPE::ERROR_LOG, log + "\n" });
        instance->errorCounter++;
    }

    void Console::Clear()
    {
        instance->warningCounter = 0;
        instance->errorCounter = 0;
        instance->gotoEnd = false;
        instance->logs.clear();
    }

    void Console::Sync()
    {
        instance->gpu_logs = instance->logs;
    }

    void Console::Render(bool* isFocused)
    {
        ImGui::Begin("Console");
        if (isFocused) *isFocused = ImGui::IsWindowFocused();

        float fps = ImGui::GetIO().Framerate;
        ImGui::Text((std::string("FPS : ") + std::to_string(fps)).c_str());
        ImGui::Separator();
        for (size_t i = 0; i < instance->gpu_logs.size(); i++)
        {
            switch (instance->gpu_logs[i].logType)
            {
            case UEngine::LOG_TYPE::LOG:
                ImGui::Text(instance->gpu_logs[i].message.c_str());
                break;
            case UEngine::LOG_TYPE::WARNING:
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), (std::string("[WARNING] ") + instance->gpu_logs[i].message).c_str());
                break;
            case UEngine::LOG_TYPE::ERROR_LOG:
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), (std::string("[ERROR] ") + instance->gpu_logs[i].message).c_str());
                break;
            }
        }
        if (instance->gotoEnd)
        {
            float maxScrollY = ImGui::GetScrollMaxY();
            if (ImGui::GetScrollY() != maxScrollY)
            {
                ImGui::SetScrollY(maxScrollY);
                instance->gotoEnd = false;
            }
        }
        ImGui::End();

        
    }
}

