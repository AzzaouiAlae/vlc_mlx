#include "vlc_mlx_init.h"

void	init_mutex(shared_int_t *sint)
{
	pthread_mutexattr_t	attr;

	pthread_mutexattr_init(&attr);
	pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&sint->mutex, &attr);
	pthread_mutexattr_destroy(&attr);
	sint->value = 0;
}

int	shared_int_access(shared_int_t *shared, int set, int value)
{
	int	val;

	pthread_mutex_lock(&shared->mutex);
	if (set)
		shared->value = value;
	val = shared->value;
	pthread_mutex_unlock(&shared->mutex);
	return (val);
}

bool	should_clean_vlc(void)
{
	return (shared_int_access(&shared_flags->should_clean, 0, 0));
}

bool	should_play_video(void)
{
	return (shared_int_access(&shared_flags->should_play, 0, 0));
}

void	exit_clear_vlc(void)
{
	if (pid)
		kill(pid, SIGTERM);
	if (video_pid)
		kill(video_pid, SIGTERM);
	if (buffer_name)
		free(buffer_name);
	if (shared_flags) 
	{
        munmap(shared_flags, sizeof(*shared_flags));
        shared_flags = NULL;
    }
    if (flags_name) {
        shm_unlink(flags_name);
        free(flags_name);
        flags_name = NULL;
    }
	waitpid(pid, 0, 0);
	waitpid(video_pid, 0, 0);
}
