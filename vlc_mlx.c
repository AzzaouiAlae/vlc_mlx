/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vlc_mlx.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aazzaoui <aazzaoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:46:34 by aazzaoui          #+#    #+#             */
/*   Updated: 2025/08/26 19:24:37 by aazzaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vlc_mlx_init.h"

libvlc_media_t			*g_m;
libvlc_media_player_t	*g_mp;
libvlc_event_manager_t	*g_em;
libvlc_instance_t		*g_inst;

void	clear_vlc_objects(bool is_video)
{
	if (g_mp)
	{
		libvlc_media_player_stop(g_mp);
		libvlc_media_player_release(g_mp);
	}
	if (g_m)
		libvlc_media_release(g_m);
	if (g_inst)
	{
		libvlc_release(g_inst);
	}
	if (is_video)
	{
		munmap(g_shared_buffer, buff_size());
		shm_unlink(g_buffer_name);
	}
}

void	init_shared_flags_child(void)
{
	int	fd;

	fd = shm_open(g_flags_name, O_RDWR, 0666);
	if (fd == -1)
	{
		perror("shm_open flags child");
		exit(1);
	}
	g_shared_flags = mmap(NULL, sizeof(t_shared_flags), PROT_READ | PROT_WRITE,
			MAP_SHARED, fd, 0);
	if (g_shared_flags == MAP_FAILED)
	{
		perror("mmap flags child");
		exit(1);
	}
	close(fd);
}

void	init_shared_buffer_child(void)
{
	int	fd;

	fd = shm_open(g_buffer_name, O_CREAT | O_RDWR, 0666);
	if (fd == -1)
	{
		perror("shm_open buffer");
		exit(1);
	}
	if (ftruncate(fd, buff_size()) == -1)
	{
		perror("ftruncate buffer");
		exit(1);
	}
	g_shared_buffer = mmap(NULL, buff_size(), PROT_READ | PROT_WRITE,
			MAP_SHARED, fd, 0);
	if (g_shared_buffer == MAP_FAILED)
	{
		perror("mmap buffer");
		exit(1);
	}
	close(fd);
}

void	set_speed(void)
{
	float	speed;

	speed = play_speed();
	speed /= 100;
	if (libvlc_media_player_set_rate(g_mp, speed) != 0)
	{
		fprintf(stderr, "⚠️ Failed to set playback rate\n");
	}
}

int	main(int argc, char **argv)
{
	int	res;

	(void)res;
	if (argc == 3)
	{
		g_flags_name = argv[2];
		init_audio(argv[1]);
	}
	else if (argc == 4)
	{
		g_shared_buffer = NULL;
		g_flags_name = argv[2];
		g_buffer_name = argv[3];
		init_video(argv[1]);
	}
	else
		res = write(2, "error\ninvalid args\n", 19);
}
