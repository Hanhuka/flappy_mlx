#include "../flappy.h"


void create_square(void *mlx, t_img *img, int size, int color) {
	img->img = mlx_new_image(mlx, size, size);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel, &img->line_length, &img->endian);
	img->width = size;
	img->height = size;
	if (!img->img)
		return ;
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			my_mlx_pixel_put(img, x, y, color);
		}
	}
}

void create_rectangle(void *mlx, t_img *img, int width, int height, int color) {
	// printf("new img w[%d] h[%d]\n", width, height);
	img->img = mlx_new_image(mlx, width, height);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel, &img->line_length, &img->endian);
	img->width = width;
	img->height = height;
	if (!img->img)
		return ;
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			my_mlx_pixel_put(img, x, y, color);
		}
	}
}

void	print_game(t_flappy *flappy) {

}

int	close_game(t_flappy *flappy) {
	if (flappy->player.img)
		mlx_destroy_image(flappy->mlx, flappy->player.img);
	if (flappy->floor.img)
		mlx_destroy_image(flappy->mlx, flappy->floor.img);
	if (flappy->floor2.img)
		mlx_destroy_image(flappy->mlx, flappy->floor2.img);
	// mlx_destroy_window(flappy->mlx, flappy->win);
	free(flappy->mlx);
	exit(0);
}

void	y_movement(t_flappy *flappy) {
	gettimeofday(&flappy->check_time, NULL);
	long seconds = flappy->check_time.tv_sec - flappy->start_time.tv_sec;
	long microseconds = flappy->check_time.tv_usec - flappy->start_time.tv_usec;
	flappy->time_diff = (double) seconds + ((double) microseconds / 1000000.0);
	// printf("state = %d\n", flappy->state);
	if (flappy->state == FALLING)
	{
		flappy->y_speed = 0.5 + (flappy->gravity * flappy->time_diff) * (flappy->gravity * flappy->time_diff);
		if (flappy->y_speed >= 3.0)
			flappy->y_speed = 3.0;
		flappy->py += flappy->y_speed;
		// printf("diference = %f\n", flappy->time_diff);
	}
	else
	{
		// printf(":( ----> %f\n", ((flappy->gravity  * flappy->time_diff) *  (flappy->gravity  * flappy->time_diff)));
		flappy->y_speed = -flappy->jumping_speed / (1.0 + 99.0 * (((flappy->gravity  * (flappy->time_diff)) * (flappy->gravity  * flappy->time_diff) *  (flappy->gravity  * flappy->time_diff)) * (flappy->gravity  * flappy->time_diff)));
		if (flappy->y_speed > -0.5)
			flappy->state = FALLING;
	}
	// printf("[[[[[[[[y_speed = %f]]]]]]]]]]]]\n", flappy->y_speed);
	// printf("player at %f\n", flappy->py);
	flappy->py += flappy->y_speed;
}

int	walls_size(t_walls **walls)
{
	int i;
	if (!walls)
		return (0);
	for (i = 0; walls[i]; i++)
		;
	return (i);
}

void	check_for_walls(t_flappy *flappy) {

	int	i = 0;

	if (!flappy->walls)
	{
		create_wall(flappy);
		return ;
	}
	for (int i = 0; flappy->walls && flappy->walls[i]; i++)
	{
		if (flappy->walls[i]->x < -flappy->walls[i]->width)
		{
			delete_wall(flappy);
		}
	}
	i = walls_size(flappy->walls) - 1;
	if (i >= 0 && flappy->walls[i] && flappy->walls[i]->x + flappy->walls[i]->width + flappy->wall_gap < WIDTH)
		create_wall(flappy);
}

void	move_walls(t_flappy *flappy) {
	if (!flappy->walls)
		return ;
	for (int i = 0; flappy->walls[i]; i++)
		flappy->walls[i]->x -= 2;
}

void	delete_wall(t_flappy *flappy) {
	t_walls	**tmp;
	int		i;

	// printf("delete wall!\n");
	if (walls_size(flappy->walls) == 1)
	{
		free(flappy->walls[0]);
		free(flappy->walls);
		flappy->walls = NULL;
		return ;
	}
	tmp = malloc(sizeof(t_flappy *) * walls_size(flappy->walls));
	for (i = 1; flappy->walls && flappy->walls[i]; i++)
	{
		tmp[i - 1] = flappy->walls[i];
	}
	tmp[i - 1] = NULL;
	free(flappy->walls[0]);
	free(flappy->walls);
	flappy->walls = tmp;
	// printf("walls_size AFTER = %d\n", walls_size(flappy->walls));
}

void	create_wall(t_flappy *flappy) {
	t_walls	**tmp;
	t_walls	*new_wall;
	int		i;

	// printf("create wall!!\n");
	tmp = flappy->walls;
	flappy->walls = malloc(sizeof(t_walls *) * (walls_size(tmp) + 2));
	new_wall = malloc(sizeof(t_walls));
	// printf("new_wall = %p\n", new_wall);
	new_wall->gap_size = flappy->settings[PILLAR_GAP];
	i = walls_size(tmp);
	if (i > 0)
		new_wall->gap_start = 64 * (rand() % 5 - 2) + tmp[i - 1]->gap_start;
	else
		new_wall->gap_start = 64 * (rand() % (HEIGHT/64) - 1);
	if (new_wall->gap_start < 0)
		new_wall->gap_start = 0;
	// printf("original gap start = %d\n", new_wall->gap_start);
	// printf("height - gap.size = %d\n", HEIGHT - new_wall->gap_size);
	if (new_wall->gap_start >= HEIGHT - new_wall->gap_size)
		new_wall->gap_start = HEIGHT - new_wall->gap_size + 10;
	new_wall->width = flappy->settings[PILLAR_WIDTH];
	new_wall->x = WIDTH - 20;
	flappy->wall_gap = flappy->settings[WALL_GAP];
	// printf("creating upper rectangle with size w[%d]h[%d]\n", new_wall->width, new_wall->gap_start);
	create_rectangle(flappy->mlx, &new_wall->upper, new_wall->width, new_wall->gap_start, 0x00AA55AA);
	// printf("creating lower rectangle with size w[%d]h[%d]\n", new_wall->width, HEIGHT - new_wall->gap_start - new_wall->gap_size);
	// printf("height = %d\n, gap start =%d\n size = %d\n", HEIGHT, new_wall->gap_start, new_wall->gap_size);
	create_rectangle(flappy->mlx, &new_wall->lower, new_wall->width, HEIGHT - new_wall->gap_start - new_wall->gap_size, 0x00AA55AA);
	for (i = 0; tmp && tmp[i]; i++)
		flappy->walls[i] = tmp[i];
	flappy->walls[i] = new_wall;
	flappy->walls[i + 1] = NULL;
}

void	check_collision(t_flappy *flappy) {
	int i = 0;

	for (i = 0; flappy->walls && flappy->walls[i]; i++)
	{
		// if (flappy->walls[i]->x + flappy->walls[i]->width  >= flappy->px)
	}
}

void	print_walls(t_flappy *flappy) {
	for (int i = 0; flappy->walls && flappy->walls[i]; i++)
	{
		print_to_frame(&(flappy->walls[i]->upper), &flappy->frame, flappy->walls[i]->x, 0);
		print_to_frame(&(flappy->walls[i]->lower), &flappy->frame, flappy->walls[i]->x, flappy->walls[i]->gap_start + flappy->walls[i]->gap_size);
	}
}

int game_engine(t_flappy *flappy) {
	if (flappy->g_state == MENU)
	{

		return 0;
	}
	//move
	//draw
	if (flappy->g_state == PLAYING)
	{
		y_movement(flappy);
		check_for_walls(flappy);
		move_walls(flappy);
	}
	create_image(flappy->mlx, &flappy->frame, WIDTH, HEIGHT);
	print_to_frame(&flappy->player, &flappy->frame, flappy->px, flappy->py);
	print_walls(flappy);
	mlx_clear_window(flappy->mlx, flappy->win);
	mlx_put_image_to_window(flappy->mlx, flappy->win, flappy->frame.img, 0, 0);
	mlx_destroy_image(flappy->mlx, flappy->frame.img);
}


void	init_flappy(t_flappy *flappy) {
	flappy->mlx = mlx_init();
	flappy->win = mlx_new_window(flappy->mlx, WIDTH, HEIGHT, "YAAA");
	create_square(flappy->mlx, &flappy->player, 64, 0x00FF0000);
	create_square(flappy->mlx, &flappy->floor, 64, 0x00FF00FF);
	create_square(flappy->mlx, &flappy->floor2, 64, 0x00FF55FF);
	flappy->state = MENU;
	flappy->px = WIDTH/2;
	flappy->py = HEIGHT/2;
	flappy->key_space = 0;
	flappy->gravity = 2.0;
	flappy->jumping_speed = 8.0;
	flappy->wall_gap = 20;
	flappy->walls = NULL;
	flappy->settings[PILLAR_GAP] = 200;
	flappy->settings[WALL_GAP] = 500;
	flappy->settings[PILLAR_WIDTH] = 50;
	gettimeofday(&flappy->start_time, NULL);
	flappy->state = FALLING;
}


int	main(void) {
	t_flappy flappy;

	init_flappy(&flappy);
	mlx_key_hook(flappy.win, key_up, &flappy);
	// mlx_hook(flappy.win, 2, 1L << 0, key_down, &flappy);
	// mlx_hook(flappy.win, 3, 1L << 1, key_up, &flappy);
	mlx_hook(flappy.win, 17, 0, close_game, &flappy);
	mlx_loop_hook(flappy.mlx, game_engine, &flappy);
	mlx_loop(flappy.mlx);
}