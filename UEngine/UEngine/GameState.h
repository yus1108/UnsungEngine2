#pragma once

namespace UEngine
{
	class GameState
	{
#pragma region Singleton
	public:
		static GameState* Get() { return &instance; }

	private:
		GameState() = default;
		~GameState() { Release(); }
		static GameState instance;
#pragma endregion
	public:
		void Init();
		void Release();

		template <typename Lambda>
		void LoadScene(Lambda loadingScene);
		void LoadScene(std::string file_name);

		void Update();
	};

	template<typename Lambda>
	inline void GameState::LoadScene(Lambda loadingScene)
	{

	}

}