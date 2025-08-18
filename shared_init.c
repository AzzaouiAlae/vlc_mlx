#include "vlc_mlx_init.h"

void	init_shared_buffer_parent(void)
{
	int	fd;

	fd = shm_open(buffer_name, O_RDWR, 0666);
	if (fd == -1)
	{
		perror("shm_open buffer child");
		exit(1);
	}
	shared_buffer = mmap(NULL, buff_size(), PROT_READ | PROT_WRITE, MAP_SHARED,
			fd, 0);
	if (shared_buffer == MAP_FAILED)
	{
		perror("mmap buffer child");
		exit(1);
	}
	close(fd);
}

void	init_mutexes(void)
{
	init_mutex(&shared_flags->should_clean);
	init_mutex(&shared_flags->should_play);
	init_mutex(&shared_flags->new_frame_flag);
	init_mutex(&shared_flags->buff_size);
	init_mutex(&shared_flags->played_audio);
	init_mutex(&shared_flags->play_speed);
	shared_int_access(&shared_flags->buff_size, 1, 0);
	shared_int_access(&shared_flags->play_speed, 1, 100);
}

void	init_flags(void)
{
	int	fd;

	if (!flags_name)
		flags_name = generate_random_shm_name();
	fd = shm_open(flags_name, O_CREAT | O_RDWR, 0666);
	if (fd == -1)
	{
		perror("shm_open flags");
		exit(1);
	}
	if (ftruncate(fd, sizeof(shared_flags_t)) == -1)
	{
		perror("ftruncate flags");
		exit(1);
	}
	shared_flags = mmap(NULL, sizeof(shared_flags_t), PROT_READ | PROT_WRITE,
			MAP_SHARED, fd, 0);
	if (shared_flags == MAP_FAILED)
	{
		perror("mmap flags");
		exit(1);
	}
	close(fd);
	init_mutexes();
}

void	copy_frame(void *data_img, size_t img_w, size_t img_h)
{
	uint8_t	*dst;
	int		dst_stride;
	int		src_stride;
	int		rows;
	t_dt2	*win_img;
	uint8_t	*src;

	if (!data_img)
		return ;
	shared_int_access(&shared_flags->new_frame_flag, 1, 0);
	win_img = data_img;
	if (!shared_buffer)
		init_shared_buffer_parent();
	src = shared_buffer;
	dst = (uint8_t *)win_img->addr;
	dst_stride = win_img->line_length;
	src_stride = img_w * 4;
	if (img_w > video_w())
		src_stride = img_w * 4;
	rows = img_h;
	if (img_h > video_h())
		rows = video_h();
	for (int y = 0; y < rows; ++y)
		memcpy(dst + y * dst_stride, src + y * src_stride, src_stride);
}

void	my_vlc_mlx_put_pixel(t_dt2 *data, int x, int y, int color)
{
	char	*dst;

	if (!data || !data->addr)
		return ;
	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

bool init_copy_resized(t_resized_frame_dt *dt, void *data, size_t img_w, size_t img_h)
{
	bzero(dt, sizeof(t_resized_frame_dt));
	dt->data = data;
    if (!dt->data || !dt->data->addr )
	{
        return false;
	}
	if (!shared_buffer)
	{
		init_shared_buffer_parent();
	}
    dt->videoW = video_w();
    dt->videoH = video_h();
    dt->step_x = (dt->videoW << 16) / img_w;
    dt->step_y = (dt->videoH << 16) / img_h;
    dt->src = (unsigned int *)shared_buffer;
    shared_int_access(&shared_flags->new_frame_flag, 1, 0);
	return true;
}

void copy_resized_frame(void *data, size_t img_w, size_t img_h)
{
	t_resized_frame_dt dt;
	
	init_copy_resized(&dt, data, img_w, img_h);
    for (dt.img_y = 0, dt.sy = 0; dt.img_y < (int)img_h; dt.img_y++, dt.sy += dt.step_y)
    {
        dt.y = dt.sy >> 16;
        dt.dst = (unsigned int *)(dt.data->addr + dt.img_y * dt.data->line_length);

        for (dt.img_x = 0, dt.sx = 0; dt.img_x < (int)img_w; dt.img_x++, dt.sx += dt.step_x)
        {
            dt.x = dt.sx >> 16;
            dt.c = dt.src[dt.y * dt.videoW + dt.x];
            dt.r = (dt.c >> 16) & 0xFF;
            dt.g = (dt.c >> 8) & 0xFF;
            dt.b = dt.c & 0xFF;
            dt.c = (dt.r << 16) | (dt.g << 8) | dt.b;
            dt.dst[dt.img_x] = dt.c;
        }
    }
}

void	clear_vlc(void)
{
	if (should_play_video())
	{
		munmap(shared_buffer, buff_size());
		shm_unlink(buffer_name);
		free(buffer_name);
		buffer_name = NULL;
	}
	shared_int_access(&shared_flags->should_clean, 1, 0);
	shared_int_access(&shared_flags->should_play, 1, 0);
	shared_int_access(&shared_flags->video_height, 1, 0);
	shared_int_access(&shared_flags->video_width, 1, 0);
	if (pid)
	{
		waitpid(pid, 0, 0);
		pid = 0;
	}
	if (video_pid)
	{
		waitpid(video_pid, 0, 0);
		video_pid = 0;
	}
}
