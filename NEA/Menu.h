#pragma once



namespace Menu
{
	enum class MenuState
	{
		MainMenu,
		StartConfig,
		StartCar,
		StartMap,
		None
	};

	void Init();
	void Update();

	void GoToState(MenuState newState);

}