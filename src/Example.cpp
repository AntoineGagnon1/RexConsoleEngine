#include <iostream>
#include "RexConsoleEngine.h"



int main()
{
	Console* c = new Console(200,100, L"Example");

	int scrollPos = 50;

	while (true)
	{
		c->PollInputs();

		if (c->IsPressed(Console::Key::Escape))
			break;

		if (c->ScrollDelta() > 0)
			scrollPos--;
		else if (c->ScrollDelta() < 0)
			scrollPos++;


		c->Clear(Console::Pixel(Console::Color::Black));

		for (int x = 0; x < c->Width(); x++)
		{
			for (int y = 0; y < c->Height(); y++)
			{
				c->Draw(x,y, Console::Pixel(Console::Color::Dark_Grey, Console::Color::Black, rand() % 26 + 'a'));
			}
		}

		c->DrawString(50, 10, Console::Color::Green, Console::Color::Black, L"Hello world !\ntest !\n\n\nhi !");

		int centerX = c->Width() / 2;
		int centerY = c->Height() / 2;

		for (int a = 0; a < 360; a += 15)
		{
			c->DrawLine(centerX, centerY, centerX + cos(a * (3.14f / 180.0f)) * 25, centerY + sin(a * (3.14f / 180.0f)) * 25, Console::Color::Dark_Green);
		}

		c->Draw(c->MouseX(), c->MouseY(), Console::Pixel(c->IsPressed(Console::Key::MouseLeft) ? Console::Color::Red : Console::Color::Yellow));

		c->Draw(20, scrollPos, Console::Pixel(Console::Color::Blue));

		c->DrawString(0, 0, Console::Color::White, Console::Color::Black, std::to_wstring(1.0f / c->DeltaTime()));
		
		c->BlipToScreen();
	}

	delete c;
	return 0;
}