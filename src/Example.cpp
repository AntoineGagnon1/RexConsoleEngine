#include <iostream>
#include "RexConsoleEngine.h"



int main()
{
	Console c(200,100);

	while (true)
	{
		c.PollInputs();
		c.Clear(Console::Pixel(Console::Color::Black));

		for (int x = 0; x < c.GetWidth(); x++)
		{
			for (int y = 0; y < c.GetHeight(); y++)
			{
				c.Draw(x,y, Console::Pixel(Console::Color::White, Console::Color::Cyan, rand() % 26 + 'a'));
			}
		}

		c.DrawString(50, 10, Console::Color::White, Console::Color::Cyan, L"Hello world !\ntest !\n\n\nhi !");

		int centerX = c.GetWidth() / 2;
		int centerY = c.GetHeight() / 2;

		for (int a = 0; a < 360; a += 15)
		{
			c.DrawLine(centerX, centerY, centerX + cos(a * (3.14f / 180.0f)) * 25, centerY + sin(a * (3.14f / 180.0f)) * 25, Console::Color::Dark_Green);
		}

		if (c.IsPressed(Console::Key::MouseForward))
			c.Draw(5,5, Console::Pixel(Console::Color::Red));

		c.Draw(c.MouseX(), c.MouseY(), Console::Pixel(Console::Color::Yellow));

		c.DrawString(0, 0, Console::Color::White, Console::Color::Black, std::to_wstring(1.0f / c.DeltaTime()));

		c.BlipToScreen();
	}
	return 0;
}