#pragma once
#include <vector>

class Console
{
#pragma region Singleton
public:
	static Console* Get() { if (instance == nullptr) instance = new Console; return instance; }
	static void Attach(Console* console) { instance = console; }
	static void Detach() { instance = nullptr; }
	static void Release() { delete instance; Detach(); }

private:
	Console() = default;
	~Console() = default;
	static Console* instance;

#pragma endregion
private:
	std::vector<std::string> logs;

public:
	static void Write(std::string log);
	static void WriteLine(std::string log);
	static void Clear();
	static void Render();
};

