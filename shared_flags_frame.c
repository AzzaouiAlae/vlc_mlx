/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared_flags_frame.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aazzaoui <aazzaoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:46:05 by aazzaoui          #+#    #+#             */
/*   Updated: 2025/08/27 10:25:58 by aazzaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vlc_mlx_init.h"

size_t	video_w(void)
{
	int	res;

	res = shared_int_access(&(vars()->shared_flags)->video_width, 0, 0);
	return ((size_t)res);
}

void	set_frame_start_end(int start_x, int start_y, int end_x, int end_y)
{
	shared_int_access(&(vars()->shared_flags)->img_start_x, 1, start_x);
	shared_int_access(&(vars()->shared_flags)->img_start_y, 1, start_y);
	shared_int_access(&(vars()->shared_flags)->img_end_x, 1, end_x);
	shared_int_access(&(vars()->shared_flags)->img_end_y, 1, end_y);
}

bool	new_frame(void)
{
	int	res;

	play_sound_track();
	res = shared_int_access(&(vars()->shared_flags)->new_frame_flag, 0, 0);
	shared_int_access(&(vars()->shared_flags)->new_frame_flag, 1, 0);
	return (res > 0);
}
