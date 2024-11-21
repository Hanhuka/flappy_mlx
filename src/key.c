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
		printf("space!\n");
		flappy->key_space = 1;
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
			flappy->selection += 1;
			flappy->selection = flappy->selection % SETTINGS_SIZE;

		}
		else if (code == XK_Left)
		{
			flappy->selection -= 1;
			printf("flappy selection = %d\n", flappy->selection);
			flappy->selection = flappy->selection % SETTINGS_SIZE;
		}
		else if (code == XK_Up)
		{
			int upper_limit = 400;
			if ((flappy->settings[flappy->selection] + 10) >= upper_limit)
				return 0;
			flappy->settings[flappy->selection] += 10;
			delete_wall(flappy);
			create_wall(flappy);
		}
		else if (code == XK_Down)
		{
			int	lower_limit = 0;

			if ((flappy->settings[flappy->selection] - 10) <= lower_limit)
				return 0;
			flappy->settings[flappy->selection] -= 10;
			delete_wall(flappy);
			create_wall(flappy);
		}
	}
	return 0;
}

int key_hook(int code, t_flappy *flappy) {

	if (code == 'a')
		printf("up!\n");
	return (0);
}