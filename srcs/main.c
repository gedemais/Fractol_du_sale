/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gedemais <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/20 18:43:03 by gedemais          #+#    #+#             */
/*   Updated: 2019/05/25 15:08:32 by gedemais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"
#include "stdio.h"

int	ft_exit(void)
{
	exit(EXIT_SUCCESS);
}

int		*ft_palette(void)
{
	static int	val = 0;

	return (&val);
}

char	*ft_clear_image(void *param)
{
	ft_memset(((t_mlx*)param)->img_data, 0, HGT * WDT * 4);
	mlx_put_image_to_window((t_mlx*)param, ((t_mlx*)param)->mlx_win, ((t_mlx*)param)->img_ptr, 0, 0);
	return (((t_mlx*)param)->img_data);
}

int 	ft_press(int button, int x, int y, void *param)
{
	clock_t 		t;
	double 			time;
	double	ratio_x;
	double	ratio_y;

	ratio_x = (double)((double)x / WDT);
	ratio_y = (double)((double)y / HGT);
	if ((button == 1 || button == 4))
	{
		((t_mlx*)param)->draw.MinRe += (ratio_x / 10) * ((t_mlx*)param)->draw.scale;
		((t_mlx*)param)->draw.MaxRe -= ((1 - ratio_x) / 10) * ((t_mlx*)param)->draw.scale;
		((t_mlx*)param)->draw.MinIm += ((1 - ratio_y) / 10) * ((t_mlx*)param)->draw.scale;
		((t_mlx*)param)->draw.MaxIm -= (ratio_y / 10) * ((t_mlx*)param)->draw.scale;
		if (((t_mlx*)param)->automatic)
			((t_mlx*)param)->draw.MaxIterations += 2;
	}
	else if (button == 5)
	{
		((t_mlx*)param)->draw.MinRe -= ratio_x / 10 * ((t_mlx*)param)->draw.scale;
		((t_mlx*)param)->draw.MaxRe += (1 - ratio_x) / 10 * ((t_mlx*)param)->draw.scale;
		((t_mlx*)param)->draw.MinIm -= (1 - ratio_y) / 10 * ((t_mlx*)param)->draw.scale;
		((t_mlx*)param)->draw.MaxIm += ratio_y / 10 * ((t_mlx*)param)->draw.scale;
		if (((t_mlx*)param)->automatic)
			((t_mlx*)param)->draw.MaxIterations--;
	}
	t = clock(); 
	((t_mlx*)param)->img_data = ft_clear_image(param);
	((t_mlx*)param)->img_data = ft_mandelbrot(((t_mlx*)param)->img_data, *ft_palette(), &((t_mlx*)param)->draw);
	mlx_put_image_to_window((t_mlx*)param, ((t_mlx*)param)->mlx_win, ((t_mlx*)param)->img_ptr, 0, 0);
	t = clock() - t;
	time = ((double)t) / CLOCKS_PER_SEC;
	if (((t_mlx*)param)->hud == true)
		ft_hud(param, time, ((t_mlx*)param)->draw.MaxIterations);
	((t_mlx*)param)->draw.scale = (((t_mlx*)param)->draw.MaxRe - ((t_mlx*)param)->draw.MinRe) * (double)((double)((t_mlx*)param)->draw.MaxIterations / 100);
	return (1);
}

void	ft_arrows(void *param, int key, double scale)
{
	if (key == 123)
	{
		((t_mlx*)param)->draw.MinRe += 0.01 * scale;
		((t_mlx*)param)->draw.MaxRe += 0.01 * scale;
	}
	else if (key == 124)
	{
		((t_mlx*)param)->draw.MinRe -= 0.01 * scale;
		((t_mlx*)param)->draw.MaxRe -= 0.01 * scale;
	}
	if (key == 125)
		((t_mlx*)param)->draw.MinIm += 0.01 * scale;
	else if (key == 126)
		((t_mlx*)param)->draw.MinIm -= 0.01 * scale;
}

int		ft_deal_key(int key, void *param)
{
	clock_t 		t;
	double 			time;
	static int		palette = 0;

	if (key == 123 || key == 124 || key == 125 || key == 126)
		ft_arrows(param, key, ((t_mlx*)param)->draw.scale);
	else if (key == 53)
		ft_exit();
	else if (key == 34)
		((t_mlx*)param)->draw.MaxIterations++;
	else if (key == 8)
	{
		if (*ft_palette() < NB_PALETTES)
			*ft_palette() = *ft_palette() + 1;
		else
			*ft_palette() = 0;
	}
	else if (key == 4)
		((t_mlx*)param)->hud = (((t_mlx*)param)->hud == false) ? true : false;
	else if (key == 0)
		((t_mlx*)param)->automatic = (((t_mlx*)param)->automatic == false) ? true : false;
	else if (key == 0)
	t = clock(); 
	((t_mlx*)param)->img_data = ft_clear_image(param);
	((t_mlx*)param)->img_data = ft_mandelbrot(((t_mlx*)param)->img_data, *ft_palette(), &((t_mlx*)param)->draw);
	mlx_put_image_to_window((t_mlx*)param, ((t_mlx*)param)->mlx_win, ((t_mlx*)param)->img_ptr, 0, 0);
	t = clock() - t; 
	time = ((double)t) / CLOCKS_PER_SEC;
	if (((t_mlx*)param)->hud == true)
		ft_hud(param, time, ((t_mlx*)param)->draw.MaxIterations);
	((t_mlx*)param)->draw.scale = (((t_mlx*)param)->draw.MaxRe - ((t_mlx*)param)->draw.MinRe) * (double)((double)((t_mlx*)param)->draw.MaxIterations / 100);
	return (1);
}

int		ft_fractol(char *name)
{
	t_mlx	env;
	int		mask;

	if (!(env.mlx_ptr = mlx_init()))
		return (-1);
	if (!(env.mlx_win = mlx_new_window(env.mlx_ptr, WDT, HGT, "Fractol du sale")))
		return (-1);
	if ((mask = ft_name_tree(name)) == -1)
		return (-1);
	if (!(env.img_ptr = mlx_new_image(env.mlx_ptr, WDT, HGT)))
		return (-1);
	if (!(env.img_data = mlx_get_data_addr(env.img_ptr, &env.bpp,
					&env.s_l, &env.endian)))
		return (-1);
	env.draw.scale = 1;
	env.hud = true;
	env.automatic = false;
	env.draw.MaxIterations = ITER_BASE;
	env.draw.MaxRe = 1.910770;
	env.draw.MaxIm = 1.146398;
	env.draw.MinRe = -2.152622;
	env.draw.MinIm = -1.139259;
	env.img_data = ft_mandelbrot(env.img_data, *ft_palette(), &(env.draw));
	mlx_put_image_to_window(&env, env.mlx_win, env.img_ptr, 0, 0);
	mlx_hook(env.mlx_win, KEY_PRESS, KEY_PRESS_MASK, ft_deal_key, &env);
	mlx_hook(env.mlx_win, 4, (1L<<2), ft_press, &env);
	mlx_hook(env.mlx_win, 17, 1L << 17, ft_exit, &env);
	mlx_loop(env.mlx_ptr);
	return (0);
}

int		main(int argc, char **argv)
{
	if (argc != 2)
	{
		ft_putstr_fd("Usage : ./fractol [fractal name]\n", 2);
		return (1);
	}
	else if (ft_fractol(argv[1]) == -1)
	{
		ft_putstr_fd("Wrong fracatal name.\n", 2);
		return (1);
	}
	return (0);
}
