#include <string>
#include "ImGui.h"
#include "Console.h"

namespace UEngine
{
    Console* Console::instance = nullptr;

    void Console::Write(std::string log)
    {
        Write(std::pair<LOG_TYPE, std::string>(LOG_TYPE::LOG, log));
    }

    void Console::Write(std::pair<LOG_TYPE, std::string> log)
    {
        instance->logs.push_back(log);
        instance->gotoEnd = true;
    }

    void Console::WriteLine(std::string log)
    {
        Write(std::pair<LOG_TYPE, std::string>(LOG_TYPE::LOG, log + "\n"));
    }

    void Console::WriteLine(float log)
    {
        WriteLine(std::to_string(log));
    }

    void Console::WriteWarning(std::string log)
    {
        Write(std::pair<LOG_TYPE, std::string>(LOG_TYPE::WARNING, log));
        instance->warningCounter++;
    }

    void Console::WriteWarningLine(std::string log)
    {
        Write(std::pair<LOG_TYPE, std::string>(LOG_TYPE::WARNING, log + "\n"));
        instance->warningCounter++;
    }

    void Console::WriteError(std::string log)
    {
        Write(std::pair<LOG_TYPE, std::string>(LOG_TYPE::ERROR_LOG, log));
        instance->errorCounter++;
    }

    void Console::WriteErrorLine(std::string log)
    {
        Write(std::pair<LOG_TYPE, std::string>(LOG_TYPE::ERROR_LOG, log + "\n"));
        instance->errorCounter++;
    }

    void Console::Clear()
    {
        instance->warningCounter = 0;
        instance->errorCounter = 0;
        instance->gotoEnd = false;
        instance->logs.clear();
    }

    void Console::Render(bool* isFocused)
    {
        ImGui::Begin("Console");
        if (isFocused) *isFocused = ImGui::IsWindowFocused();

        float fps = ImGui::GetIO().Framerate;
        ImGui::Text((std::string("FPS : ") + std::to_string(fps)).c_str());
        ImGui::Separator();
        for (size_t i = 0; i < instance->logs.size(); i++)
        {
            switch (instance->logs[i].first)
            {
            case UEngine::LOG_TYPE::LOG:
                ImGui::Text(instance->logs[i].second.c_str());
                break;
            case UEngine::LOG_TYPE::WARNING:
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), (std::string("[WARNING] ") + instance->logs[i].second).c_str());
                break;
            case UEngine::LOG_TYPE::ERROR_LOG:
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), (std::string("[ERROR] ") + instance->logs[i].second).c_str());
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

