#include <string>
#include "ImGui.h"
#include "Console.h"

Console* Console::instance = nullptr;

void Console::Write(std::string log)
{
    instance->logs.push_back(log);
}

void Console::WriteLine(std::string log)
{
    instance->logs.push_back(log + "\n");
}

void Console::Clear()
{
    instance->logs.clear();
}

void Console::Render()
{
    ImGui::Begin("Console");
    for (size_t i = 0; i < instance->logs.size(); i++)
        ImGui::Text(instance->logs[i].c_str());
    ImGui::End();
}
