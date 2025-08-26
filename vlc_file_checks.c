/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vlc_file_checks.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aazzaoui <aazzaoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:46:26 by aazzaoui          #+#    #+#             */
/*   Updated: 2025/08/26 18:46:27 by aazzaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vlc_mlx_init.h"

bool	file_exists(char *filename)
{
	return (access(filename, R_OK) == 0);
}

bool	is_video_file(char *path)
{
	char	cmd[512];

	snprintf(cmd, sizeof(cmd), "file --mime-type '%s' | grep -q 'video/'",
		path);
	return (system(cmd) == 0);
}

int	is_audio_file(char *path)
{
	char	cmd[512];

	snprintf(cmd, sizeof(cmd), "file --mime-type '%s' | grep -q 'audio/'",
		path);
	return (system(cmd) == 0);
}
