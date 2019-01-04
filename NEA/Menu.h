#pragma once



namespace Menu
{
	enum class MenuState;

	void Init();
	void Update();

	void GoToState(MenuState newState);

}