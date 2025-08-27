vlc_SRC = vlc_mlx.c video_callbacks.c vlc_play.c
vlc_OBJ = $(vlc_SRC:.c=.o)
vlc_lib_SRC = shared_flags.c shared_init.c shared_int.c time_random.c vlc_exec.c \
				shared_copy_buff.c shared_copy_resize.c shared_flags_frame.c \
				shared_flags_video.c vlc_file_checks.c vlc_clear.c
vlc_lib_OBJ = $(vlc_lib_SRC:.c=.o)
VLC_NAME=vlc_mlx
VLC_LIB=libvlcmlx
CC = gcc -I$(HOME)/libvlc/include
CFLAGS= -Wall -Wextra -Werror -O3
all : deps $(VLC_LIB) $(VLC_NAME)

deps:
	@$(MAKE) -f ./deps.mk

$(VLC_NAME) : $(vlc_OBJ)
	$(CC) $(vlc_OBJ) -lvlc -L$(HOME)/libvlc/lib $(VLC_LIB) -o $(VLC_NAME)

$(VLC_LIB): $(vlc_lib_OBJ)
	ar rcs $@ $^

clean :
	rm -fr $(OBJ) $(vlc_OBJ) $(vlc_lib_OBJ)

fclean : clean
	rm -fr $(VLC_NAME) $(VLC_LIB)

re : fclean all

.PHONY: all clean fclean re bonus
