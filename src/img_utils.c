# include "../flappy.h"

unsigned int	get_image_color(t_img *data, int x, int y)
{
	// printf("getting color on x[%d]y[%d] from image with size x[%d]y[%d]\n", x, y, data->width, data->height);
	if (x > data->width)
		x = data->width;
	if (y > data->height)
		y = data->height;
	return (*(unsigned int *)(data->addr + (y * data->line_length
			+ x * (data->bits_per_pixel / 8))));
}

void	my_mlx_pixel_put(t_img *data, int x, int y, int color)
{
	char	*dst;

	// printf("frame width[%d] height[%d] x[%d] y[%d]\n", data->width, data->height, x, y);
	if (x < 0 || x >= data->width || y < 0 || y >= data->height)
		return ;
	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	if (color != 0xFF000000)
	{
		// printf("putting color %d\n", color);
		*(unsigned int *)dst = color;
	}
}

void	print_to_frame(t_img *img, t_img *frame, int x, int y)
{
	int i;
	int j;

	i = 0;
	// printf("frame width[%d] height[%d] x[%d] y[%d]\n", img->width, img->height, x, y);
	while (i < img->height)
	{
		j = 0;
		while (j < img->width)
		{
			if ((j + x) > 0 && (j + x) < WIDTH && (i + y) > 0 &&  (i + y) < HEIGHT)
			{
				// printf("putting pixel at x[%d]y[%d]\n", x, y);
				my_mlx_pixel_put(frame, j + x, i + y, get_image_color(img, j, i));
			}
			j++;
		}
		i++;
	}
}

void	create_image(void *mlx, t_img *data, int width, int height) {
	data->img = mlx_new_image(mlx, width, height);
	if (!data->img)
		return;
	data->width = width;
	data->height = height;
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel,
			&data->line_length, &data->endian);
}

void	create_xpm_image(void *mlx, t_img *data, char *name) {
	int	width;
	int	height;

	data->img = mlx_xpm_file_to_image(mlx, name, &width, &height);
	if (!data->img)
		return;
	data->width = width;
	data->height = height;
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel,
			&data->line_length, &data->endian);
}