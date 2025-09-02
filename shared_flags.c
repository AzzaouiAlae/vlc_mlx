/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared_flags.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aazzaoui <aazzaoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:45:55 by aazzaoui          #+#    #+#             */
/*   Updated: 2025/08/26 18:49:06 by aazzaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vlc_mlx_init.h"

bool	played_audio(void)
{
	return (shared_int_access(&(vars()->shared_flags)->played_audio, 0, 0));
}

size_t	buff_size(void)
{
	int	res;

	res = shared_int_access(&(vars()->shared_flags)->buff_size, 0, 0);
	return ((size_t)res);
}

void	set_play_speed(int speed)
{
	shared_int_access(&(vars()->shared_flags)->play_speed, 1, speed);
}

int	play_speed(void)
{
	return (shared_int_access(&(vars()->shared_flags)->play_speed, 0, 0));
}
