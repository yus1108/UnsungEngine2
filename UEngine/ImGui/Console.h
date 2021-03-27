#pragma once
#include <vector>

namespace UEngine
{
	enum class LOG_TYPE
	{
		LOG,
		WARNING,
		ERROR_LOG
	};

	struct Message
	{
		LOG_TYPE logType;
		std::string message;
	};

	class Console
	{
	public:
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
		std::vector<Message> logs;
		std::vector<Message> gpu_logs;
		unsigned warningCounter = 0;
		unsigned errorCounter = 0;
		bool gotoEnd = false;

	public:
		static void Write(Message log);
		static void Write(std::string log);
		static void WriteLine(std::string log);
		static void WriteLine(float log);
		static void WriteWarning(std::string log);
		static void WriteWarningLine(std::string log);
		static void WriteError(std::string log);
		static void WriteErrorLine(std::string log);
		static void Clear();
		static void Sync();
		static void Render(bool* isFocused = nullptr);
	};
}

