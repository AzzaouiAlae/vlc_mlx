#include "vlc_mlx_init.h"

libvlc_media_t			*m;
libvlc_media_player_t	*mp;
libvlc_event_manager_t	*em;
libvlc_instance_t		*inst;

void init_shared_flags_child(void) 
{
    int fd;

	fd = shm_open(flags_name, O_RDWR, 0666);
    if (fd == -1) 
	{ 
		perror("shm_open flags child"); 
		exit(1); 
	}
    shared_flags = mmap(NULL, sizeof(shared_flags_t),
                        PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_flags == MAP_FAILED) 
	{ 
		perror("mmap flags child"); 
		exit(1); 
	}
    close(fd);
}

void	init_shared_buffer_child(void)
{
	int	fd;

	fd = shm_open(buffer_name, O_CREAT | O_RDWR, 0666);
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
	shared_buffer = mmap(NULL, buff_size(), PROT_READ | PROT_WRITE, MAP_SHARED,
			fd, 0);
	if (shared_buffer == MAP_FAILED)
	{
		perror("mmap buffer");
		exit(1);
	}
	close(fd);
}

void set_speed()
{
	float speed;

	speed = play_speed();
	speed /= 100;
    if (libvlc_media_player_set_rate(mp, speed) != 0) {
        fprintf(stderr, "⚠️ Failed to set playback rate\n");
    }
}

int	main(int argc, char **argv)
{
	int res;

	(void)res;
	if (argc == 3)
	{
		flags_name = argv[2];
		init_audio(argv[1]);
	}
	else if (argc == 4)
	{
		shared_buffer = NULL;
		flags_name = argv[2];
        buffer_name = argv[3];
		init_video(argv[1]);
	}
	else
		res = write(2, "error\ninvalid args\n", 19);
}
