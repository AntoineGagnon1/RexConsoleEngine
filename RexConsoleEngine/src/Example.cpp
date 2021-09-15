#include "RexConsoleEngine.h"

#include <fstream>
#include <iostream>

#define PI 3.14159265358979323846
#pragma warning(disable:4996)

using namespace RexConsoleEngine;

int main()
{
	Sprite sprite;
	sprite.LoadBMP("Test.bmp");
	// Create a 200x100 console titled Example
	Console* c = new Console(200,100, "Example");

	int scrollPos = 0; // Position changed by the mouse scroll wheel

	while (true)
	{
		c->PollInputs(); // Get the new inputs

		if (c->IsPressed(Console::Key::Escape) || c->ShouldClose()) // Should the window close ? break the loop if true
			break;
		
		c->Draw(10, 10, Console::Color::Dark_Cyan);
		c->Draw(11,11, sprite);

		c->Draw(0,0, DrawString("Test\nString", Console::Color::White, Console::Color::Black));

		// Apply the screen buffer to the console
		c->BlipToScreen();
	}

	delete c;
	return 0;
}