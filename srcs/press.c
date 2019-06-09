/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   press.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gedemais <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 18:50:26 by gedemais          #+#    #+#             */
/*   Updated: 2019/06/08 15:52:01 by gedemais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

void	ft_zoom(void *param, double ratio_x, double ratio_y)
{
	t_mlx	*s;

	s = ((t_mlx*)param);
	s->draw.minre += (ratio_x / 10) * s->draw.scale;
	s->draw.maxre -= ((1 - ratio_x) / 10) * s->draw.scale;
	s->draw.minim += ((1 - ratio_y) / 10) * s->draw.scale;
	s->draw.maxim -= (ratio_y / 10) * s->draw.scale;
	if (s->automatic)
		s->draw.maxiterations++;
}

void	ft_dezoom(void *param, double ratio_x, double ratio_y)
{
	t_mlx	*s;

	s = ((t_mlx*)param);
	s->draw.minre -= ratio_x / 10 * s->draw.scale;
	s->draw.maxre += (1 - ratio_x) / 10 * s->draw.scale;
	s->draw.minim -= (1 - ratio_y) / 10 * s->draw.scale;
	s->draw.maxim += ratio_y / 10 * s->draw.scale;
	if (s->automatic)
		s->draw.maxiterations--;
}

int		ft_pos(int x, int y, void *param)
{
	t_mlx	*s;
	clock_t	t;
	double	time;

	s = ((t_mlx*)param);
	if (s->julia_m == false)
		return (1);
	t = clock();
	*julia_x() = (x - WDT) / (double)WDT;
	*julia_y() = (y - HGT) / (double)HGT;
	ft_memset(((t_mlx*)param)->img_data, 0, HGT * WDT * 4);
	s->img_data = ft_run_kernel(s, &s->s, s->img_data);
	mlx_put_image_to_window((t_mlx*)param, ((t_mlx*)param)->mlx_win,
		((t_mlx*)param)->img_ptr, 0, 0);
	t = clock() - t;
	time = ((double)t) / CLOCKS_PER_SEC;
	if (s->hud)
		ft_hud(param, time, s->draw.maxiterations);
	return (1);
}

int		ft_press(int button, int x, int y, void *param)
{
	t_mlx			*s;
	clock_t			t;
	double			time;
	double			ratio_x;
	double			ratio_y;

	s = ((t_mlx*)param);
	ratio_x = (double)((double)x / WDT);
	ratio_y = (double)((double)y / HGT);
	if ((button == 1 || button == 4))
		ft_zoom(param, ratio_x, ratio_y);
	else if (button == 2 && s->draw.mask == JULIA)
		s->julia_m = (s->julia_m) ? false : true;
	else if ((button == 5) && s->draw.maxiterations > 30)
		ft_dezoom(param, ratio_x, ratio_y);
	t = clock();
	s->img_data = *gpu() ? ft_run_kernel(s, &s->s, ft_memset(s->img_data, 0, sizeof(char) * HGT * WDT * 4)) : ft_mandelbrot(ft_memset(s->img_data, 0, sizeof(char) * HGT * WDT * 4), s->draw.palette, &s->draw);
	mlx_put_image_to_window((t_mlx*)param, s->mlx_win, s->img_ptr, 0, 0);
	t = clock() - t;
	time = ((double)t) / CLOCKS_PER_SEC;
	ft_hud(s->hud ? param : NULL, time, s->draw.maxiterations);
	s->draw.scale = (s->draw.maxre - s->draw.minre) *
	(double)((double)s->draw.maxiterations / 120);
	return (1);
}
