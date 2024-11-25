/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flappy.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralves-g <ralves-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 16:39:27 by ralves-g          #+#    #+#             */
/*   Updated: 2024/11/25 16:03:33 by ralves-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FLAPPY_H
# define FLAPPY_H

# define WIDTH 1920
# define HEIGHT 1000

// # include <mlx.h>
# include "mlx_linux/mlx.h"
# include <stdio.h>
# include <sys/time.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/errno.h>
# include <sys/time.h>
# include <math.h>
# include "X11/keysym.h"
# include <string.h>
# include <stdlib.h>

# define SPATH "sprites/"

#define ESC 65307
#define SPACE 32

#define FALLING 0
#define JUMPING 1

#define MENU 0
#define PLAYING 1
#define LOSS 2

#define PILLAR_GAP 0
#define WALL_GAP 1
#define PILLAR_WIDTH 2

#define SETTINGS_SIZE 3

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		height;
	int		width;
}			t_img;

typedef struct s_walls
{
	t_img	upper;
	t_img	lower;
	int		gap_size;
	int		gap_start;
	int		x;
	int		width;
	int		passed;
}			t_walls;

typedef struct s_flappy {
	int		g_state;
	int		selection;
	struct	timeval start_time, check_time;
	t_walls	**walls;
	double	time_diff;
	double	gravity;
	double	jumping_speed;
	int		state;
	void	*mlx;
	void	*win;
	double	px;
	double	py;
	double	y_speed;
	int		wall_gap;
	int		score;
	t_img	frame;
	t_img	player;
	t_img	pipe;
	t_img	floor;
	t_img	floor2;
	t_img	background;
	t_img	font;
	t_img	font_selected;
	int		key_space;
	int		settings[3];
}			t_flappy;
#endif

unsigned int	get_image_color(t_img *data, int x, int y);
void	my_mlx_pixel_put(t_img *data, int x, int y, int color);
void	print_to_frame(t_img *img, t_img *frame, int x, int y);
void	create_square(void *mlx, t_img *img, int size, int color);
int		key_down(int code, t_flappy *flappy);
int		key_up(int code, t_flappy *flappy);
int		key_hook(int code, t_flappy *flappy);
int		close_game(t_flappy *flappy);
void	create_image(void *mlx, t_img *data, int width, int height);
void	create_xpm_image(void *mlx, t_img *data, char *name);
void	check_for_walls(t_flappy *flappy);
void	move_walls(t_flappy *flappy);
void	create_wall(t_flappy *flappy);
void	delete_wall(t_flappy *flappy);
void	print_character(t_flappy *flappy, char c, int x_start, int y_start, int width, int height, t_img *font);
void	print_phrase(t_flappy *flappy, char *str, int x_start, int y_start, int width, int height, int spacing, t_img *font);
char	*ft_itoa(int n);
void	init_flappy(t_flappy *flappy);
void	reset_game(t_flappy *flappy);
int key_up2(int code, t_flappy *flappy);
