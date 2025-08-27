/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vlc_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aazzaoui <aazzaoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:46:23 by aazzaoui          #+#    #+#             */
/*   Updated: 2025/08/27 09:26:28 by aazzaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vlc_mlx_init.h"

t_shared_flags	*g_shared_flags;
void			*g_shared_buffer;
char			*g_flags_name;
char			*g_buffer_name;
pid_t			g_pid;
pid_t			g_video_pid;

char	**make_execv_args(const char *filename, bool video_rendering)
{
	int		argc;
	int		i;
	char	**args;

	i = 0;
	if (video_rendering)
		argc = 5;
	else
		argc = 4;
	args = calloc(argc, sizeof(char *));
	if (!args)
	{
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	i = 0;
	args[i++] = "vlc_mlx";
	args[i++] = (char *)filename;
	args[i++] = (char *)g_flags_name;
	if (video_rendering)
		args[i++] = (char *)g_buffer_name;
	args[i] = NULL;
	return (args);
}

void	exec_vlc(char *file, bool video_rendering)
{
	const char	*home = getenv("HOME");
	char		vlc_path[1024];
	char		**args;

	if (g_pid || g_video_pid)
		return ;
	if (video_rendering)
		g_buffer_name = generate_random_shm_name();
	args = make_execv_args(file, video_rendering);
	g_shared_buffer = NULL;
	if (video_rendering)
		g_video_pid = fork();
	else
		g_pid = fork();
	if (g_pid != 0 || g_video_pid != 0)
	{
		free(args);
		return ;
	}
	bzero(vlc_path, 1024);
	strcat(vlc_path, home);
	strcat(vlc_path, "/vlc_mlx/vlc_mlx");
	execv(vlc_path, args);
	free(args);
	exit(1);
}

void	play_video(char *file)
{
	int	i;

	i = 0;
	if (!file_exists(file) || !is_video_file(file))
	{
		write(2, "Error\ninvalid file\n", 19);
		return ;
	}
	while (played_audio() && !should_clean_vlc() && i++ < 200)
		usleep(5000);
	if (should_clean_vlc())
		clear_vlc();
	exec_vlc(file, true);
	i = 0;
	while (i++ < 2000)
	{
		if (should_play_video() || should_clean_vlc())
			break ;
		usleep(500);
	}
}

void	play_audio(char *file)
{
	if (!file_exists(file) || !is_audio_file(file))
	{
		write(2, "Error\ninvalid file\n", 19);
		return ;
	}
	exec_vlc(file, false);
}
