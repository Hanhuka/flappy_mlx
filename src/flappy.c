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
	new_wall->passed = 0;
	// if (i > 0)
	// 	new_wall->gap_start = 64 * (rand() % 5 - 2) + tmp[i - 1]->gap_start;
	// else
		new_wall->gap_start = 64 * (rand() % (HEIGHT/64) - 1);
	if (new_wall->gap_start < 0)
		new_wall->gap_start = 10;
	// printf("original gap start = %d\n", new_wall->gap_start);
	// printf("height - gap.size = %d\n", HEIGHT - new_wall->gap_size);
	if (new_wall->gap_start >= HEIGHT - new_wall->gap_size)
		new_wall->gap_start = HEIGHT - new_wall->gap_size - 10;
	new_wall->width = flappy->settings[PILLAR_WIDTH];
	new_wall->x = WIDTH - new_wall->width;
	// if (i > 0)
	// new_wall->width
	flappy->wall_gap = flappy->settings[WALL_GAP];
	printf("creating upper rectangle with size w[%d]h[%d]\n", new_wall->width, new_wall->gap_start);
	create_rectangle(flappy->mlx, &new_wall->upper, new_wall->width, new_wall->gap_start, 0x00AA55AA);
	printf("creating lower rectangle with size w[%d]h[%d]\n", new_wall->width, HEIGHT - new_wall->gap_start - new_wall->gap_size);
	// printf("height = %d\n, gap start =%d\n size = %d\n", HEIGHT, new_wall->gap_start, new_wall->gap_size);
	create_rectangle(flappy->mlx, &new_wall->lower, new_wall->width, HEIGHT - new_wall->gap_start - new_wall->gap_size, 0x00AA55AA);
	for (i = 0; tmp && tmp[i]; i++)
		flappy->walls[i] = tmp[i];
	flappy->walls[i] = new_wall;
	flappy->walls[i + 1] = NULL;
}

void	reset_game(t_flappy *flappy) {
	if (flappy->walls)
	{
		while (walls_size(flappy->walls))
			delete_wall(flappy);
	}
	init_flappy(flappy);
}

void	check_collision(t_flappy *flappy) {
	int i = 0;

	if (flappy->py < 0 || flappy->py + flappy->player.height > HEIGHT) {			flappy->g_state = LOSS;
		return ;
	}
	for (i = 0; flappy->walls && flappy->walls[i]; i++)
	{
		if (!flappy->walls[i]->passed && \
			(flappy->px >= flappy->walls[i]->x && flappy->px <= flappy->walls[i]->x + flappy->walls[i]->width) || \
			(flappy->px + flappy->player.width >=  flappy->walls[i]->x && flappy->px + flappy->player.width <= flappy->walls[i]->x + flappy->walls[i]->width))
		{
			if (flappy->py <= flappy->walls[i]->gap_start || flappy->py + flappy->player.height >= flappy->walls[i]->gap_start + flappy->walls[i]->gap_size)
			{
				flappy->g_state = LOSS;
				return ;
			}
		}
		if (!flappy->walls[i]->passed && flappy->walls[i]->x < flappy->px - flappy->player.width)
		{
			flappy->walls[i]->passed = 1;
			flappy->score += 1;
		}
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
	create_image(flappy->mlx, &flappy->frame, WIDTH, HEIGHT);
	if (flappy->g_state == MENU)
	{
			char *nbr = ft_itoa(flappy->settings[PILLAR_GAP]);
			print_phrase(flappy, "pillar gap", 20, 20, 50, 40, 10, flappy->selection == PILLAR_GAP ?  &flappy->font_selected : &flappy->font);
			print_phrase(flappy, nbr, 800, 20, 50, 40, 10, flappy->selection == PILLAR_GAP ?  &flappy->font_selected : &flappy->font);
			free(nbr);
			
			nbr = ft_itoa(flappy->settings[WALL_GAP]);
			print_phrase(flappy, "wall gap", 20, 80, 50, 40, 10, flappy->selection == WALL_GAP ? &flappy->font_selected : &flappy->font);
			print_phrase(flappy, nbr, 800, 80, 50, 40, 10, flappy->selection == WALL_GAP ? &flappy->font_selected : &flappy->font);
			free(nbr);

			nbr = ft_itoa(flappy->settings[PILLAR_WIDTH]);
			print_phrase(flappy, "wall width", 20, 140, 50, 40, 10, flappy->selection == PILLAR_WIDTH ? &flappy->font_selected : &flappy->font);
			print_phrase(flappy, nbr, 800, 140, 50, 40, 10, flappy->selection == PILLAR_WIDTH ? &flappy->font_selected : &flappy->font);
			free(nbr);
	}
	//move
	//draw
	if (flappy->g_state == PLAYING)
	{
		y_movement(flappy);
		move_walls(flappy);
		check_collision(flappy);
	}
	check_for_walls(flappy);
	print_to_frame(&flappy->player, &flappy->frame, flappy->px, flappy->py);

	print_walls(flappy);
	if (flappy->g_state == PLAYING)
	{
		char *nbr = ft_itoa(flappy->score);
		print_phrase(flappy, "score", 20, 20, 20, 20, 5, &flappy->font);
		print_phrase(flappy, nbr, 200, 20, 20, 20, 5, &flappy->font);
		free(nbr);
	}
	if (flappy->g_state == LOSS)
	{
		char *nbr = ft_itoa(flappy->score);
		print_phrase(flappy, "score", WIDTH / 2 - (((6 + strlen(nbr)) * 75) / 2), HEIGHT / 2 - 30, 70, 60, 5, &flappy->font);
		print_phrase(flappy, nbr, WIDTH / 2 + ((5 * 75) / 2) - (strlen(nbr) * 75 / 2), HEIGHT / 2 - 30, 70, 60, 5, &flappy->font);
		free(nbr);
		print_phrase(flappy, "press space to retry", WIDTH / 2 - ((20 * 75) / 2), HEIGHT / 2 + 80, 70, 60, 5, &flappy->font_selected);
	}
	mlx_clear_window(flappy->mlx, flappy->win);
	mlx_put_image_to_window(flappy->mlx, flappy->win, flappy->frame.img, 0, 0);
	mlx_destroy_image(flappy->mlx, flappy->frame.img);
}

void	print_phrase(t_flappy *flappy, char *str, int x_start, int y_start, int width, int height, int spacing, t_img *font) {

	for (int i = 0; str && str[i]; i++) {
		print_character(flappy, str[i], x_start + (i * (width + spacing)), y_start, width, height, font);
	}
}

void	print_character(t_flappy *flappy, char c, int x_start, int y_start, int width, int height, t_img *font) {
	// WIDTH = 9
	// HEIGHT = 7
	int start;
	if (width <= 0 || height <= 0)
	{
		printf("width or height < 0\n");
		return ;
	}
	// printf("character = [%c]\n", c);
	if (c >= 'a' && c <= 'z')
		start = 9 * 10 + (c - 'a') * 9;
	else if (c >= '0' && c <= '9')
		start = (c - '0') * 9;
	else
		return ;
	// printf("start = %d\n", start);
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			// printf("putting pixel at x[%d]y[%d]\n", x_start + x, y_start + y);
			my_mlx_pixel_put(&flappy->frame, x_start + x, y_start + y, get_image_color(font, ((double)x / (double)width) * 9.0 + (double) start, ((double)y / (double)height) * 7.0));
		}
	}

}

void	init_flappy(t_flappy *flappy) {
	flappy->g_state = MENU;
	flappy->px = WIDTH/2;
	flappy->py = HEIGHT/2;
	flappy->score = 0;
	flappy->key_space = 0;
	flappy->gravity = 2.0;
	flappy->jumping_speed = 8.0;
	flappy->wall_gap = 20;
	flappy->walls = NULL;
	flappy->selection = 0;
	flappy->settings[PILLAR_GAP] = 300;
	flappy->settings[WALL_GAP] = 500;
	flappy->settings[PILLAR_WIDTH] = 50;
	gettimeofday(&flappy->start_time, NULL);
	flappy->state = FALLING;
}


int	main(void) {
	t_flappy flappy;


	int h;
	int w;
	flappy.mlx = mlx_init();
	flappy.win = mlx_new_window(flappy.mlx, WIDTH, HEIGHT, "YAAA");
	create_square(flappy.mlx, &flappy.player, 64, 0x00FF0000);
	create_square(flappy.mlx, &flappy.floor, 64, 0x00FF00FF);
	create_square(flappy.mlx, &flappy.floor2, 64, 0x00FF55FF);
	create_xpm_image(flappy.mlx, &flappy.font, "font.xpm");
	create_xpm_image(flappy.mlx, &flappy.font_selected, "font_selected.xpm");
	init_flappy(&flappy);
	mlx_key_hook(flappy.win, key_up, &flappy);
	// mlx_hook(flappy.win, 2, 1L << 0, key_down, &flappy);
	// mlx_hook(flappy.win, 3, 1L << 1, key_up2, &flappy);
	mlx_hook(flappy.win, 17, 0, close_game, &flappy);
	mlx_loop_hook(flappy.mlx, game_engine, &flappy);
	mlx_loop(flappy.mlx);
}