/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared_flags_video.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aazzaoui <aazzaoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:46:08 by aazzaoui          #+#    #+#             */
/*   Updated: 2025/08/26 19:08:16 by aazzaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vlc_mlx_init.h"

int	img_start_x(void)
{
	return (shared_int_access(&(vars()->shared_flags)->img_start_x, 0, 0));
}

int	img_start_y(void)
{
	return (shared_int_access(&(vars()->shared_flags)->img_start_y, 0, 0));
}

int	img_end_x(void)
{
	return (shared_int_access(&(vars()->shared_flags)->img_end_x, 0, 0));
}

int	img_end_y(void)
{
	return (shared_int_access(&(vars()->shared_flags)->img_end_y, 0, 0));
}

size_t	video_h(void)
{
	int	res;

	res = shared_int_access(&(vars()->shared_flags)->video_height, 0, 0);
	return ((size_t)res);
}
