#include "RexConsoleEngine.h"

#define PI 3.14159265358979323846

int main()
{
	// Create a 200x100 console titled Example
	Console* c = new Console(200,100, L"Example");

	int scrollPos = 0; // Position changed by the mouse scroll wheel

	while (true)
	{
		c->PollInputs(); // Get the new inputs

		if (c->IsPressed(Console::Key::Escape) || c->ShouldClose()) // Should the window close ? break the loop if true
			break;

		// Update the scroll position using the new scrollDelta of this frame
		if (c->ScrollDelta() > 0)
			scrollPos--;
		else if (c->ScrollDelta() < 0)
			scrollPos++;

		// Clear the screen buffer with black pixels
		c->Clear(Console::Pixel(Console::Color::Black));

		// Showcase the DrawString() method
		c->DrawString(50, 10, Console::Color::White, Console::Color::Black, L"Hello world !\nAnother line !");

		// Center of the screen
		int centerX = c->Width() / 2;
		int centerY = c->Height() / 2;

		// Draw a line every 36 degrees in a circle at the center of the screen and offset it on the y axis by the scroll position
		for (int a = 0; a < 360; a += 36)
			c->DrawLine(centerX, centerY + scrollPos, centerX + cos(a * (PI / 180.0f)) * 25, centerY + scrollPos + sin(a * (PI / 180.0f)) * 25, (Console::Color)((a / 15) + 1));

		// Draw a yellow pixel under the mouse, the pixel will turn red when mouse1 is clicked
		c->Draw(c->MouseX(), c->MouseY(), Console::Pixel(c->IsPressed(Console::Key::MouseLeft) ? Console::Color::Red : Console::Color::Yellow));

		// Apply the screen buffer to the console
		c->BlipToScreen();
	}

	delete c;
	return 0;
}