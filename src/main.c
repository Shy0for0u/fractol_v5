#include "../includes/fractol.h"

void			init_fcl(t_fcl *f)
{
	f->mlx.mlx = mlx_init();
	f->mlx.win = mlx_new_window(f->mlx.mlx, WTH, HGT, "Fractal");
	f->mlx.img = mlx_new_image(f->mlx.mlx, WTH, HGT);
	f->mlx.data = mlx_get_data_addr(f->mlx.img, &(f->mlx.bpp), &(f->mlx.sl), &(f->mlx.endian));
	f->max_i = 100;
	f->stop_move = 1;
	f->tmp1 = 1.0f;
	f->offset_x = 0.0f;
	f->offset_y = 0.0f;
	f->min = init_complex(-2.0f * HGT / WTH, -2.0f);
	f->max.re = 2.0f * HGT / WTH;
	f->max.im = f->min.im + (f->max.re - f->min.re) * HGT / WTH;
	f->factor = init_complex((f->max.re - f->min.re) / (WTH - 1),
							   (f->max.im - f->min.im) / (HGT - 1));
}

void			pthread_calc(t_fcl *f)
{
	t_thr		*t;
	int 		i;

	i = 0;
	if ((t = (t_thr *)malloc(sizeof(t_thr) * THR)) == NULL)
		alert_message(1);
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

void			put_point(t_fcl *f)
{
	int 		x;
	int 		y;
	t_color color;

	color.r = 0;
	color.g = 0;
	color.b = 0;
	y = -1;
	while (++y < HGT)
	{
		x = -1;
		while (++x < WTH)
		{
			if (x == (WTH / 2) || y == (HGT / 2))
			{
				color.r = (char)0xff;
				color.g = (char)0xff;
				color.b = (char)0xff;
				pixel_to_img(f, x, y, color);
			}
		}
	}
}

void			showing(t_fcl *f)
{
	pthread_calc(f);
	put_point(f);
	mlx_put_image_to_window(f->mlx.mlx, f->mlx.win, f->mlx.img, 0, 0);
}

int 			main(int argc, char *argv[])
{
	t_fcl		f;

	if (argc == 2)
	{
		if (change_algorithm(&f, argv[1]))
		{
			init_fcl(&f);
			showing(&f);
			hooks(&f);
			mlx_loop(f.mlx.mlx);
		}
	}
	else
		return (alert_message(0));
	return 0;
}