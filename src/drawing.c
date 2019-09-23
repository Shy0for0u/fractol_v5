/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgorold- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 14:24:08 by dgorold-          #+#    #+#             */
/*   Updated: 2019/09/23 23:20:41 by dgorold-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

void			pixel_to_img(t_fcl *f, int x, int y, t_color c)
{
	int			index;

	index = (int)(x * 4 + f->mlx.sl * y);
	f->mlx.data[index] = (char)c.b;
	f->mlx.data[index + 1] = (char)c.g;
	f->mlx.data[index + 2] = (char)c.r;
}

void			set_color(int iter, int x, int y, t_fcl *f)
{
	t_color		color;
	double		t;

	t = (double)iter / f->max_i;
	color.r = 0;
	color.g = 0;
	color.b = 0;
	if (iter < f->max_i)
	{
		color.r = (char)(9 * (1 - t) * t * t * t * 255);
		color.g = (char)(15 * (1 - t) * (1 - t) * t * t * 255);
		color.b = (char)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
	}
	pixel_to_img(f, x, y, color);
}
