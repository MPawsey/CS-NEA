#pragma once



namespace Menu
{
	enum class MenuState
	{
		MainMenu,
		StartConfig,
		StartCar,
		StartMap
	};

	void Init();
	void Update();

	void GoToState(MenuState newState);

}