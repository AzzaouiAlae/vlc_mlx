/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vlc_mlx.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aazzaoui <aazzaoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:46:34 by aazzaoui          #+#    #+#             */
/*   Updated: 2025/09/02 14:35:17 by aazzaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vlc_mlx_init.h"

void	clear_vlc_objects(bool is_video)
{
	if (vars()->mp)
	{
		libvlc_media_player_stop(vars()->mp);
		libvlc_media_player_release(vars()->mp);
	}
	if (vars()->m)
		libvlc_media_release(vars()->m);
	if (vars()->inst)
	{
		libvlc_release(vars()->inst);
	}
	if (is_video)
	{
		munmap(vars()->shared_buffer, buff_size());
		shm_unlink(vars()->buffer_name);
	}
}

void	init_shared_flags_child(void)
{
	int	fd;

	fd = shm_open(vars()->flags_name, O_RDWR, 0666);
	if (fd == -1)
	{
		perror("shm_open flags child");
		exit(1);
	}
	vars()->shared_flags = mmap(NULL, sizeof(t_shared_flags),
		PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (vars()->shared_flags == MAP_FAILED)
	{
		perror("mmap flags child");
		exit(1);
	}
	close(fd);
}

void	init_shared_buffer_child(void)
{
	int	fd;

	fd = shm_open(vars()->buffer_name, O_CREAT | O_RDWR, 0666);
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
	vars()->shared_buffer = mmap(NULL, buff_size(), PROT_READ | PROT_WRITE,
		MAP_SHARED, fd, 0);
	if (vars()->shared_buffer == MAP_FAILED)
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
	if (libvlc_media_player_set_rate(vars()->mp, speed) != 0)
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
		vars()->flags_name = argv[2];
		init_audio(argv[1]);
	}
	else if (argc == 4)
	{
		vars()->shared_buffer = NULL;
		vars()->flags_name = argv[2];
		vars()->buffer_name = argv[3];
		init_video(argv[1]);
	}
	else
		res = write(2, "error\ninvalid args\n", 19);
}
