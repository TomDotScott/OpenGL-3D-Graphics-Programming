#include "Constants.h"
#include "Game.h"

int main()
{
	Game game("3D Graphics Programming ICA SCOTT Thomas W9036922",
		constants::k_screenWidth, constants::k_screenHeight,
		4, 5,
		true);

	while (!game.GetWindowShouldClose())
	{
		game.Update();
		game.Render();
	}

	return 0;
}
