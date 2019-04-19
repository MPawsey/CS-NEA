#pragma once
#include "Clickable.h"
#include "Tooltip.h"

namespace UI
{


	class CheckBox : public Clickable
	{
	private:

		bool m_isChecked = false;
		sf::RectangleShape m_background;
		sf::Text m_checkedText;
		Event<bool> m_checkBoxUpdateEvent;
		Tooltip m_tooltip;

		virtual void OnMouseHover();
		virtual void OnMouseUnhover();
		virtual void OnMouseClick();

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	public:

		CheckBox();
		CheckBox(const CheckBox& checkBox);
		CheckBox(sf::View& view);

		CheckBox& operator=(const CheckBox& checkBox);

		void SetTooltipText(sf::Text content);
		void SetTooltipText(std::string content);

		void SetChecked(bool isChecked);

		bool IsChecked() const;

		Event<bool>& GetCheckBoxUpdateEvent();
	};




}