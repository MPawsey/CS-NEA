#pragma once



namespace Menu
{
	enum class MenuState
	{
		MainMenu,
		StartConfig,
		AdvancedConfig,
		StartCar,
		StartMap,
		None
	};

	class Menu
	{
	public:
		virtual void Init() = 0;
		virtual void Update() = 0;
		virtual void Load() = 0;
		virtual void Unload() = 0;
	};

	void Init();
	void Update();

	void GoToState(MenuState newState);

}