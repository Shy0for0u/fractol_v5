#include "../includes/fractol.h"

t_complex		init_complex(double re, double im)
{
	t_complex	c;

	c.im = im;
	c.re = re;
	return (c);
}

void			apply_zoom(t_fcl *f, t_complex mouse, double z_fact)
{
	double 		interpolation;

	interpolation = 1.0 / z_fact;
	f->min.re = mouse.re + (f->min.re - mouse.re) * interpolation;
	f->min.im = mouse.im + (f->min.im - mouse.im) * interpolation;
	f->max.re = mouse.re + (f->max.re - mouse.re) * interpolation;
	f->max.im = mouse.im + (f->max.im - mouse.im) * interpolation;
	f->factor = init_complex((f->max.re - f->min.re) / (WTH - 1),
							 (f->max.im - f->min.im) / (HGT - 1));
	f->tmp1 = (f->max.re - f->min.re) * 0.025;
}

int			change_algorithm(t_fcl *fcl, char *algorithm)
{
	if (ft_strcmp("julia", algorithm) == 0)
		fcl->f = julia;
	else if (ft_strcmp("mandelbrot", algorithm) == 0)
		fcl->f = mandelbrot;
	else if (ft_strcmp("mandelbar", algorithm) == 0)
		fcl->f = mandelbar;
	else
	{
		alert_message(0);
		return (0);
	}
	return (1);
}

int			alert_message(int key)
{
	if (key == 0)
		ft_putstr("\nUsage:\n"
				  "\t\t\t\t ./Fractal <type of fractal>\n\n"
				  "List of fractals:\n"
				  "\t\t\t\t - julia\n"
				  "\t\t\t\t - mandelbrot\n"
				  "\t\t\t\t - mandelbar\n");
	if (key == 1)
	{
		ft_putstr("malloc drop programme!\n");
		exit(0);
	}
	return (0);
}

int 			out(t_fcl *f)
{
	mlx_destroy_window(f->mlx.mlx, f->mlx.win);
	exit(0);
}

