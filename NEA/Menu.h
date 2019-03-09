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

	void Init();
	void Update();

	void GoToState(MenuState newState);

}