/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralves-g <ralves-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 16:39:27 by ralves-g          #+#    #+#             */
/*   Updated: 2024/03/04 22:12:36 by ralves-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FLAPPY_H
# define FLAPPY_H

# define WIDTH 2000
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

# define SPATH "sprites/"

#define ESC 65307
#define SPACE 32

#define FALLING 0
#define JUMPING 1

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
}			t_walls;

typedef struct s_flappy {
	struct timeval start_time, check_time;
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
	t_img	frame;
	t_img	player;
	t_img	pipe;
	t_img	floor;
	t_img	floor2;
	t_img	background;
	int		key_space;
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
void	check_for_walls(t_flappy *flappy);
void	move_walls(t_flappy *flappy);
void	create_wall(t_flappy *flappy);
void	delete_wall(t_flappy *flappy);