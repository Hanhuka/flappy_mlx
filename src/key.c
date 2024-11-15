#include "../flappy.h"

int key_down(int code, t_flappy *flappy) {
	printf("keydown space = %d\n", flappy->key_space);
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
	if (code == ESC)
		close_game(flappy);
	if (code == SPACE)
	{
		flappy->state = JUMPING;
		flappy->y_speed = -flappy->jumping_speed;
		gettimeofday(&flappy->start_time, NULL);
	}
	return 0;
}

int key_hook(int code, t_flappy *flappy) {

	if (code == 'a')
		printf("up!\n");
	return (0);
}