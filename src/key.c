#include "../flappy.h"

int key_down(int code, t_flappy *flappy) {
	printf("keydown space = %d\n", code);
	printf("%d\n", code);
	if (code == ESC)
		close_game(flappy);
	if (code == 'a')
		printf("down!\n");
	if (code == SPACE && flappy->key_space == 0)
	{
		flappy->key_space = 1;
		printf("space! = %d\n", flappy->key_space);
	}
	return 0;
}

int key_up2(int code, t_flappy *flappy) {
	printf("keydown space = %d\n", code);
	printf("%d\n", code);
	if (code == ESC)
		close_game(flappy);
	if (code == 'a')
		printf("down!\n");
	if (code == SPACE && flappy->key_space == 1)
	{
		flappy->key_space = 0;
		printf("space! = %d\n", flappy->key_space);
	}
	return 0;
}

int key_up(int code, t_flappy *flappy) {
	printf("keydown space = %d\n", code);
	printf("g_state = %d\n", flappy->g_state);
	if (code == ESC)
		close_game(flappy);
	if (flappy->g_state == PLAYING)
	{
		if (code == SPACE)
		{
			flappy->state = JUMPING;
			flappy->y_speed = -flappy->jumping_speed;
			gettimeofday(&flappy->start_time, NULL);
		}
	}
	else if (flappy->g_state == MENU)
	{
		if (code == SPACE)
		{
			flappy->g_state = PLAYING;
			flappy->state = JUMPING;
			flappy->y_speed = -flappy->jumping_speed;
			gettimeofday(&flappy->start_time, NULL);
		}
		if (code == XK_Right)
		{
			int upper_limit = 500;
			if (flappy->selection == WALL_GAP)
				return 0;
			if ((flappy->settings[flappy->selection] + 10) > upper_limit)
				return 0;
			flappy->settings[flappy->selection] += 10;
			delete_wall(flappy);
			create_wall(flappy);

		}
		else if (code == XK_Left)
		{
			int	lower_limit = 0;

			if ((flappy->settings[flappy->selection] - 10) <= lower_limit)
				return 0;
			flappy->settings[flappy->selection] -= 10;
			delete_wall(flappy);
			create_wall(flappy);
		}
		else if (code == XK_Up)
		{
			flappy->selection -= 1;
						// printf("flappy selection = %d\n", flappy->selection);
			if (flappy->selection < 0 )
				flappy->selection = SETTINGS_SIZE;

			// flappy->selection = flappy->selection % SETTINGS_SIZE;
						// printf("flappy selection after = %d\n", flappy->selection);


		}
		else if (code == XK_Down)
		{
			flappy->selection += 1;
			flappy->selection = flappy->selection % SETTINGS_SIZE;
		}
	}
	else if (flappy->g_state == LOSS) {
		if (code == SPACE)
		{
			flappy->g_state = MENU;
			reset_game(flappy);
		}	
	}
	return 0;
}

int key_hook(int code, t_flappy *flappy) {

	if (code == 'a')
		printf("up!\n");
	return (0);
}