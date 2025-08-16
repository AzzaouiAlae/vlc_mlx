#include "vlc_mlx_init.h"

shared_flags_t	*shared_flags;
void			*shared_buffer;
char			*flags_name;
char			*buffer_name;
pid_t			pid;
pid_t			video_pid;
char			*vlc_path = "./vlc_mlx/vlc_mlx";

char	**make_execv_args(const char *filename, bool video_rendering)
{
	int		argc;
	int		i;
	char	**args;

	i = 0;
	argc = video_rendering ? 5 : 4;
	args = calloc(argc, sizeof(char *));
	if (!args)
	{
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	i = 0;
	args[i++] = "vlc_mlx";
	args[i++] = (char *)filename;
	args[i++] = (char *)flags_name;
	if (video_rendering)
		args[i++] = (char *)buffer_name;
	args[i] = NULL;
	return (args);
}

void	exec_vlc(char *file, bool video_rendering)
{
	char	**args;

	if (pid || video_pid)
		return ;
	if (video_rendering)
		buffer_name = generate_random_shm_name();
	args = make_execv_args(file, video_rendering);
	shared_buffer = NULL;
	if (video_rendering)
		video_pid = fork();
	else
		pid = fork();
	if (pid != 0 || video_pid != 0)
	{
		free(args);
		return ;
	}
	execv(vlc_path, args);
	free(args);
	exit(1);
}

void	play_video(char *file)
{
	int i;

	i = 0;
	while (played_audio() && !should_clean_vlc() && i < 2000)
	{
		usleep(500);
		i++;
	}
	if (should_clean_vlc())
		clear_vlc();
	exec_vlc(file, true);
}

void	play_audio(char *file)
{
	exec_vlc(file, false);
}
