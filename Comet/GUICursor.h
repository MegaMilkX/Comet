#pragma once

namespace Comet
{
	class GUI;
	class GUIElement;

	class GUICursor
	{
	public:
		GUICursor();
		~GUICursor();

		void Reset();
		void Update();

		double X(){ return x; }
		double Y(){ return y; }
		GUIElement* GetFocusElement(){ return focusElement; }

		friend GUI;
		friend GUIElement;
	private:
		double x, y;
		GUIElement* focusElement;
	};

}