#include "../includes/fractol.h"


t_complex		init_complex(float re, float im)
{
	t_complex	c;

	c.im = im;
	c.re = re;
	return (c);
}

void			alert_message(char *alert)
{
	ft_putstr(alert);
}

void			apply_zoom(t_fcl *f, t_complex mouse, float z_fact)
{
	float 		interpolation;

	interpolation = 1.0f / z_fact;
	f->min.re = mouse.re + (f->min.re - mouse.re) * interpolation;
	f->min.im = mouse.im + (f->min.im - mouse.im) * interpolation;
	f->max.re = mouse.re + (f->min.re - mouse.re) * interpolation;
	f->max.im = mouse.im + (f->min.im - mouse.im) * interpolation;
	f->factor = init_complex((f->max.re - f->min.re) / (WTH - 1),
							 (f->max.im - f->min.im) / (HGT - 1));
	f->tmp1 = (f->max.re - f->min.re) * 0.025f;
	printf("%f \n", f->scale);
}

void			init_fcl(t_fcl *f)
{
	f->mlx.mlx = mlx_init();
	f->mlx.win = mlx_new_window(f->mlx.mlx, WTH, HGT, "Fractal");
	f->mlx.img = mlx_new_image(f->mlx.mlx, WTH, HGT);
	f->mlx.data = mlx_get_data_addr(f->mlx.img, &(f->mlx.bpp), &(f->mlx.sl), &(f->mlx.endian));
	f->max_i = 100;
	f->zoom_factor = 1.0f;
	f->stop_move = 1;
	f->tmp1 = 0.5f;
	f->offset1 = 0.0f;
	f->offset2 = 0.0f;
	f->scale = 0.0f;
	f->min = init_complex(-2.0f * HGT / WTH, -2.0f);
	f->max.re = 2.0f * HGT / WTH;
	f->max.im = f->min.im + (f->max.re - f->min.re) * HGT / WTH;
	f->factor = init_complex((f->max.re - f->min.re) / (WTH - 1),
							   (f->max.im - f->min.im) / (HGT - 1));

}

void			pixel_to_img(t_fcl *f, int x, int y, t_color c)
{
	int 		index;

	index = (int)(x * 4 + f->mlx.sl * y);
	f->mlx.data[index] = (char)c.b;
	f->mlx.data[index + 1] = (char)c.g;
	f->mlx.data[index + 2] = (char)c.r;
}

void			set_color(int iter, int x, int y, t_fcl *f)
{
	t_color		color;
	float 		t;

	t = (float)iter / f->max_i;
	color.r = 0;
	color.g = 0;
	color.b = 0;
	if (iter < f->max_i)
	{
		color.r = (char)(9 * (1 - t) * t * t * t * 255);
		color.g = (char)(15 * (1 - t) * (1 - t) * t * t * 255);
		color.b = (char)(8.5f * (1 - t) * (1 - t) * (1 - t) * t * 255);
	}
	pixel_to_img(f, x, y, color);
}

//float			julia(float x, float y, void *fr)
//{
//	float 		iter;
//	float 		x_temp;
//	float 		y_temp;
//
//	t_fcl *f = (t_fcl *)fr;
//	iter = 0;
////	f->pt.x = WTH / HGT * (x - WTH * 0.5f) / (f->zoom_factor * WTH * 0.5f);
////	f->pt.y = (y - HGT * 0.5f) / (f->zoom_factor * HGT * 0.5f);
//	f->pt.x = (WTH / HGT * (x - WTH * 0.5f) / (WTH * 0.5f));
//	f->pt.y = ((y - HGT * 0.5f) / (HGT * 0.5f));
////	f->c.im = (f->max.im - x * f->factor.im);
////	f->c.re = (f->min.re - y * f->factor.re);
//	while (f->pt.x * f->pt.x + f->pt.y * f->pt.y <= 4.0f && iter < f->max_i)
//	{
//		x_temp = f->pt.x * f->pt.x - f->pt.y * f->pt.y + f->c.re;
//		y_temp = 2.0f * f->pt.x * f->pt.y + f->c.im;
//		f->pt.x = x_temp;
//		f->pt.y = y_temp;
//		iter++;
//	}
//	return (iter);
//}

int 			julia(t_complex c, void *f_)
{
	int 		i;
	t_complex	z;
	t_fcl		*f;
	float 		z_sum;

	z = c;
	f = (t_fcl *)f_;
	i = 0;
	z_sum = z.re * z.re - z.im * z.im;
	while (z_sum <= 4 && i < f->max_i)
	{
		z.im = 2 * z.re * z.im + f->c.im;
		z.re = z_sum + f->c.re;
		z_sum = z.re * z.re - z.im * z.im;
		i++;
	}
	return (i);
}

void			*fractals(void *thread)
{
	t_thr		*t;
	int 		x;
	t_color		black;
	int			iter;
	t_complex	c;

	t = (t_thr *)thread;
	while (++(t->in) < t->out)
	{
		c.im = (t->fcl.max.im + t->fcl.offset2 - (float)t->in * t->fcl.factor.im);
		x = -1;
		while (++x < (int)WTH)
		{
			c.re = (t->fcl.min.re + t->fcl.offset1 + (float)x * t->fcl.factor.re);
			if ((iter = t->fcl.f(c, &(t->fcl))) < t->fcl.max_i)
				set_color(iter, x, t->in, &(t->fcl));
			else
			{
				black.r = 0;
				black.g = 0;
				black.b = 0;
				pixel_to_img(&t->fcl, x, t->in, black);
			}
		}
	}
	return (NULL);
}

void			pthread_calc(t_fcl *f)
{
	t_thr		*t;
	int 		i;

	i = 0;
	if ((t = (t_thr *)malloc(sizeof(t_thr) * THR)) == NULL)
		alert_message("malloc drop programme!\n");
	while (i < THR)
	{
		t[i].in = i * (int)PART - 1;
		t[i].out = (i + 1) * (int)PART;
		t[i].fcl = *f;
		pthread_create(&(t[i].p), NULL, fractals, (void *)&t[i]);
		i++;
	}
	while (i--)
		pthread_join(t[i].p, NULL);
	free(t);
}

void			showing(t_fcl *f)
{
	pthread_calc(f);
	mlx_put_image_to_window(f->mlx.mlx, f->mlx.win, f->mlx.img, 0, 0);
}

void			change_algorithm(t_fcl *fcl, char *algorithm)
{
	if (ft_strcmp("julia", algorithm) == 0)
		 fcl->f = julia;
}

int 			main(int argc, char *argv[])
{
	t_fcl		f;

	if (argc == 2)
	{
		change_algorithm(&f, argv[1]);
		init_fcl(&f);
		showing(&f);
		hooks(&f);
		mlx_loop(f.mlx.mlx);
	}
	return 0;
}