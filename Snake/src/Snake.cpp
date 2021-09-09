// A simple snake program using the RexConsoleEngine
#include "RexConsoleEngine.h"

#include <deque>
#include <time.h>
#include <algorithm>

constexpr int MapSize = 50; // The size of the play area
constexpr int UIWidth = 28; // The width of the ui

class Snake
{
public:
	enum class Direction { Up, Right, Down, Left};

private:
	std::deque<std::pair<int, int>> m_body;

	Direction m_dir;

public:
	// Start position
	Snake(int x, int y, Direction dir)
	{
		m_body.push_back({x,y});
		m_dir = dir;
	}
	
	// Set the direction
	void SetDir(Direction dir) { m_dir = dir; }

	// Update (move) the snake, return true if the snake is still alive, false otherwise
	bool Update(bool grow)
	{
		int dx = 0, dy = 0;

		switch (m_dir)
		{
		case Snake::Direction::Up:
			dy -= 1;
			break;
		case Snake::Direction::Right:
			dx += 1;
			break;
		case Snake::Direction::Down:
			dy += 1;
			break;
		case Snake::Direction::Left:
			dx -= 1;
			break;
		default:
			break;
		}

		std::pair<int, int> newHead = { (m_body.back().first + dx + MapSize) % MapSize, (m_body.back().second + dy + MapSize) % MapSize };
		// Check if the snake ate itself
		if (Contains(newHead.first, newHead.second))
			return false; // Dead

		m_body.push_back(newHead);
		if (!grow)
			m_body.pop_front();

		return true; // Alive
	}

	// Draw the snake
	void Draw(Console& c) const
	{
		for (auto p : m_body)
		{
			c.Draw(p.first, p.second, Console::Color::Dark_Green);
		}

		c.Draw(m_body.back().first, m_body.back().second, Console::Color::Green); // Draw the head in a different green
	}

	// Is this coord inside the snake
	bool Contains(int x, int y)
	{
		return (std::find(m_body.begin(), m_body.end(), std::pair<int,int>(x,y)) != m_body.end());
	}
};

void RandomApplePos(int& x, int& y)
{
	x = rand() % MapSize;
	y = rand() % MapSize;
}

int main()
{
	srand(time(NULL));

	Console* c = new Console(MapSize + UIWidth, MapSize, L"Snake");
	Snake s(MapSize/2, MapSize/2, Snake::Direction::Up);

	float time = 0;
	int appleX, appleY;
	int score = 0;
	RandomApplePos(appleX, appleY);

	while (!c->ShouldClose())
	{
		c->PollInputs();

		if (c->IsPressed(Console::Key::Up))
			s.SetDir(Snake::Direction::Up);
		else if (c->IsPressed(Console::Key::Right))
			s.SetDir(Snake::Direction::Right);
		else if (c->IsPressed(Console::Key::Down))
			s.SetDir(Snake::Direction::Down);
		else if (c->IsPressed(Console::Key::Left))
			s.SetDir(Snake::Direction::Left);

		time += c->DeltaTime();
		if (time >= 0.075f)
		{
			time = 0;
			bool ateApple = s.Contains(appleX, appleY);
			
			if (!s.Update(ateApple))
				break;
			
			if (ateApple)
			{
				score++;
				RandomApplePos(appleX, appleY);
			}
		}

		c->Clear(Console::Color::Dark_Grey);



		s.Draw(*c); // Drawn the snake
		c->Draw(appleX, appleY, Console::Color::Red); // Draw the apple

		// UI
		c->Fill(MapSize, 0, UIWidth, MapSize, Console::Color::Black);

		// Snake logo
		c->DrawString(MapSize, 1, Console::Color::Green, Console::Color::Black, L" _____             _\n/  ___|           | |\n\\ `--. _ __   __ _| | _____\n `--. \\ '_ \\ / _` | |/ / _ \\\n/\\__/ / | | | (_| |   <  __/\n\\____/|_| |_|\\__,_|_|\\_\\___|");

		c->DrawString(MapSize + 11, 10, Console::Color::White, Console::Color::Black, L"Score:");
		c->DrawString(MapSize + (UIWidth / 2) - std::to_wstring(score).length(), 11, Console::Color::White, Console::Color::Black, std::to_wstring(score));

		c->BlipToScreen();
	}


	delete c;
	return 0;
}