#include "vlc_mlx_init.h"

bool	played_audio(void)
{
	return (shared_int_access(&shared_flags->played_audio, 0, 0));
}

bool	new_frame(void)
{
	int	res;

	res = shared_int_access(&shared_flags->new_frame_flag, 0, 0);
	shared_int_access(&shared_flags->new_frame_flag, 1, 0);
	return (res > 0);
}

size_t	video_w(void)
{
	int	res;

	res = shared_int_access(&shared_flags->video_width, 0, 0);
	return ((size_t)res);
}

size_t	video_h(void)
{
	int	res;

	res = shared_int_access(&shared_flags->video_height, 0, 0);
	return ((size_t)res);
}

size_t	buff_size(void)
{
	int	res;

	res = shared_int_access(&shared_flags->buff_size, 0, 0);
	return ((size_t)res);
}
void set_play_speed(int speed)
{
	shared_int_access(&shared_flags->play_speed, 1, speed);
}

int play_speed()
{
	return shared_int_access(&shared_flags->play_speed, 0, 0);
}