/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aazzaoui <aazzaoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:46:13 by aazzaoui          #+#    #+#             */
/*   Updated: 2025/08/26 19:12:27 by aazzaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vlc_mlx_init.h"

void	init_shared_buffer_parent(void)
{
	int	fd;

	fd = shm_open(g_buffer_name, O_RDWR, 0666);
	if (fd == -1)
	{
		perror("shm_open buffer child");
		exit(1);
	}
	g_shared_buffer = mmap(NULL, buff_size(), PROT_READ | PROT_WRITE,
			MAP_SHARED, fd, 0);
	if (g_shared_buffer == MAP_FAILED)
	{
		perror("mmap buffer child");
		exit(1);
	}
	close(fd);
}

void	init_mutexes(void)
{
	init_mutex(&g_shared_flags->should_clean);
	init_mutex(&g_shared_flags->should_play);
	init_mutex(&g_shared_flags->new_frame_flag);
	init_mutex(&g_shared_flags->buff_size);
	init_mutex(&g_shared_flags->played_audio);
	init_mutex(&g_shared_flags->play_speed);
	shared_int_access(&g_shared_flags->buff_size, 1, 0);
	shared_int_access(&g_shared_flags->play_speed, 1, 100);
}

void	init_flags(void)
{
	int	fd;

	if (!g_flags_name)
		g_flags_name = generate_random_shm_name();
	fd = shm_open(g_flags_name, O_CREAT | O_RDWR, 0666);
	if (fd == -1)
	{
		perror("shm_open flags");
		exit(1);
	}
	if (ftruncate(fd, sizeof(t_shared_flags)) == -1)
	{
		perror("ftruncate flags");
		exit(1);
	}
	g_shared_flags = mmap(NULL, sizeof(t_shared_flags), PROT_READ | PROT_WRITE,
			MAP_SHARED, fd, 0);
	if (g_shared_flags == MAP_FAILED)
	{
		perror("mmap flags");
		exit(1);
	}
	close(fd);
	init_mutexes();
}
