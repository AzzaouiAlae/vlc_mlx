LIBVLC_DIR := $(HOME)/libvlc
LIBVLC_URL := git@github.com:AzzaouiAlae/lib_vlc_for_mlx_vlc.git
VLC_MLX_DIR := $(HOME)/vlc_mlx
VLC_MLX_URL := git@github.com:AzzaouiAlae/vlc_mlx.git

.PHONY: all libvlc

all: libvlc vlc_mlx

libvlc:
	@if [ -d "$(LIBVLC_DIR)" ]; then \
		echo "✅ libvlc already downloaded in $(LIBVLC_DIR)." >/dev/null; \
	else \
		echo "⚠️ libvlc not found. Downloading precompiled version..." >/dev/null; \
		mkdir -p $(LIBVLC_DIR); \
		git clone $(LIBVLC_URL) $(LIBVLC_DIR); \
	fi
	
vlc_mlx:
	@if [ -d "$(VLC_MLX_DIR)" ]; then \
		if [ -f $(VLC_MLX_DIR)/vlc_mlx ]; then \
			echo "✅ vlc_mlx already downloaded in $(VLC_MLX_DIR)"; \
		else \
			$(MAKE) compile -C $(VLC_MLX_DIR); \
		fi; \
	else \
		echo "⚠️ vlc_mlx not found. Downloading..."; \
		mkdir -p $(VLC_MLX_DIR); \
		git clone $(VLC_MLX_URL) $(VLC_MLX_DIR); \
		$(MAKE) -C $(VLC_MLX_DIR); \
	fi
