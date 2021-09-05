#include <iostream>
#include "RexConsoleEngine.h"


int main()
{
	Console c(200,100);

	while (true)
	{
		c.Clear(Console::Pixel(Console::Color::Black));

		for (int x = 0; x < c.GetWidth(); x++)
		{
			for (int y = 0; y < c.GetHeight(); y++)
			{
				c.Draw(x,y, Console::Pixel(Console::Color::White, Console::Color::Cyan, rand() % 26 + 'a'));
			}
		}

		c.DrawString(50, 10, Console::Color::White, Console::Color::Cyan, "Hello world !\ntest !\n\n\nhi !");

		c.BlipToScreen();
	}
	return 0;
}