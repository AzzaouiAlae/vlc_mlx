/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sound_track.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aazzaoui <aazzaoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 14:37:21 by aazzaoui          #+#    #+#             */
/*   Updated: 2025/09/02 14:37:22 by aazzaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vlc_mlx_init.h"

pid_t	sound_track_pid(int set, pid_t value)
{
	static pid_t	pid;

	if (set)
		pid = value;
	return (pid);
}

void	play_sound_track(void)
{
	static int	i;
	int			len;
	char		**files_name;

	files_name = sound_name(0, 0);
	len = 0;
	while (files_name && files_name[len])
		len++;
	if (!len)
		return ;
	if (exec_sound_track(files_name[i]))
		i++;
	if (i >= len)
		i = 0;
}

bool	exec_sound_track(char *file)
{
	pid_t		pid;
	const char	*home = getenv("HOME");
	char		vlc_path[1024];
	char		**args;

	pid = sound_track_pid(0, 0);
	if (pid && waitpid(pid, 0, WNOHANG) == 0)
		return (false);
	pid = fork();
	sound_track_pid(1, pid);
	if (pid != 0)
		return (true);
	args = make_execv_args(file, false);
	bzero(vlc_path, 1024);
	strcat(vlc_path, home);
	strcat(vlc_path, "/vlc_mlx/vlc_mlx");
	execv(vlc_path, args);
	free(args);
	exit(1);
	return (true);
}

char	**sound_name(int set, char **files)
{
	static char	**files_name;

	if (set)
		files_name = files;
	return (files_name);
}

void	sound_track(char **files)
{
	sound_name(1, files);
}
