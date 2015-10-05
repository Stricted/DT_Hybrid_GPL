#############################################################
#
# ffmpeg
#
#############################################################

#
# Dependencies
#
FFMPEG_DEPENDENCIES = atptypes ffmpeg-extract zlib FFMPEG_CFG FFMPEG_MK

FFMPEG_TAR_SRC = $(TOPDIR)/dl/ffmpeg-0.5.tar.bz2
FFMPEG_AFTER_EXTR_SRC = $(O)/build/ffmpeg-0.5
FFMPEG_TAR_EXTRACT_OPT = tar zxvf

SERVICE_FFMPEG_SRC_DIR = $(FFMPEG_AFTER_EXTR_SRC)/src

FFMPEG_VERSION = 1.01
define FFMPEG_CHANGELOGS
V1.00:
	1. First release by <<ccmm>>.
endef

# Target name
FFMPEG_TARGET = dynamic

# ffmpeg directory

$(O)/package/atp/libraries/opensrc/ffmpeg/.stamp_configured:
ifeq ($(BR2_ARCH),"i386")
	$(Q)if [ -f $(SERVICE_FFMPEG_SRC_DIR)/configure ]; then \
		cd $(SERVICE_FFMPEG_SRC_DIR); \
		./configure --disable-logging --disable-debug \
		--disable-ffmpeg --disable-ffplay --disable-ffserver \
		--disable-vhook --disable-network --disable-ipv6 \
		--disable-mpegaudio-hp --disable-aandct --disable-fft \
		--disable-golomb --disable-mdct --disable-rdft \
		--disable-encoders --disable-decoders --disable-muxers \
		--disable-devices --disable-filters --disable-bsfs \
		--enable-protocol=file 	--disable-demuxers --enable-demuxer=rm \
		--enable-demuxer=matroska --enable-demuxer=mov \
		--enable-demuxer=mp3 --enable-demuxer=aac --enable-demuxer=amr \
		--enable-demuxer=avi --enable-demuxer=asf \
		--enable-demuxer=mpegps --enable-demuxer=mpegts \
		--enable-demuxer=mpegtsraw --enable-demuxer=rawvideo --enable-demuxer=wav \
		--enable-demuxer=mpegvideo --enable-demuxer=image2  \
		--enable-decoder=mjpeg  --enable-decoder=png --enable-decoder=gif \
		--disable-parsers --enable-parser=mpegvideo \
		--enable-parser=mpegaudio --enable-shared \
		--enable-small --enable-zlib; \
		fi
else
	$(Q)if [ -f $(SERVICE_FFMPEG_SRC_DIR)/configure ]; then \
		cd $(SERVICE_FFMPEG_SRC_DIR); \
		./configure --disable-logging --disable-debug \
		--disable-ffmpeg --disable-ffplay --disable-ffserver \
		--disable-vhook --disable-network --disable-ipv6 \
		--disable-mpegaudio-hp --disable-aandct --disable-fft \
		--disable-golomb --disable-mdct --disable-rdft \
		--disable-encoders --disable-decoders --disable-muxers \
		--disable-devices --disable-filters --disable-bsfs \
		--enable-protocol=file 	--disable-demuxers --enable-demuxer=rm \
		--enable-demuxer=matroska --enable-demuxer=mov \
		--enable-demuxer=mp3 --enable-demuxer=aac --enable-demuxer=amr \
		--enable-demuxer=avi --enable-demuxer=asf \
		--enable-demuxer=mpegps --enable-demuxer=mpegts \
		--enable-demuxer=mpegtsraw --enable-demuxer=rawvideo --enable-demuxer=wav \
		--enable-demuxer=mpegvideo --enable-demuxer=image2  \
		--enable-decoder=mjpeg  --enable-decoder=png --enable-decoder=gif \
		--disable-parsers --enable-parser=mpegvideo \
		--enable-parser=mpegaudio --enable-shared \
		--arch=$(ARCH) --cpu=$(BR2_GCC_TARGET_TUNE) \
		--cross-prefix=$(CROSS_COMPILE) \
		--enable-small --enable-zlib \
		--extra-ldflags="-L$(STAGING_DIR)/lib" \
		--extra-libs="-lz" --extra-cflags="-I$(O)/host/usr/include"; \
		fi
endif
	@touch $(O)/package/atp/libraries/opensrc/ffmpeg/.stamp_configured

FFMPEG_CFG: $(O)/package/atp/libraries/opensrc/ffmpeg/.stamp_configured

FFMPEG_MK:
	$(Q)if [ -f $(SERVICE_FFMPEG_SRC_DIR)/Makefile ]; then \
		$(MAKE) -C $(SERVICE_FFMPEG_SRC_DIR); \
		$(MAKE) -C $(SERVICE_FFMPEG_SRC_DIR) myinstall; \
		elif [ -f $(LIB_ATP_DIR)/libavutil.so.49 ];then \
		cd $(LIB_ATP_DIR); \
		rm -f libavutil.so; \
		rm -f libavformat.so; \
		rm -f libavcodec.so; \
		ln -sf libavutil.so.49 libavutil.so; \
		ln -sf libavformat.so.52 libavformat.so; \
		ln -sf libavcodec.so.52 libavcodec.so; \
		else $(error) $(LIB_ATP_DIR)/libavformat.so not exist! exit 1;\
		fi

# Install lib
define FFMPEG_INSTALL_TARGET_CMDS
	install $(STAGING_DIR)/lib/libavformat.so.52 $(TARGET_DIR)/lib; \
	install $(STAGING_DIR)/lib/libavcodec.so.52 $(TARGET_DIR)/lib; \
	install $(STAGING_DIR)/lib/libavutil.so.49 $(TARGET_DIR)/lib; \
	cd $(STAGING_DIR)/lib; rm -f libavformat.so libavcodec.so libavutil.so; \
	ln -s  $(STAGING_DIR)/lib/libavformat.so.52 $(STAGING_DIR)/lib/libavformat.so; \
	ln -s  $(STAGING_DIR)/lib/libavcodec.so.52 $(STAGING_DIR)/lib/libavcodec.so; \
	ln -s  $(STAGING_DIR)/lib/libavutil.so.49 $(STAGING_DIR)/lib/libavutil.so; \
	cd $(TARGET_DIR)/lib; rm -f libavformat.so libavcodec.so libavutil.so; \
	ln -s  $(TARGET_DIR)/lib/libavformat.so.52 $(TARGET_DIR)/lib/libavformat.so; \
	ln -s  $(TARGET_DIR)/lib/libavcodec.so.52 $(TARGET_DIR)/lib/libavcodec.so; \
	ln -s  $(TARGET_DIR)/lib/libavutil.so.49 $(TARGET_DIR)/lib/libavutil.so;
endef

define FFMPEG_BUILD_CMDS
	@echo ffmpeg build done.
endef

FFMPEG_LICENSE="LGPL"
define FFMPEG_CLEAN_CMDS
	$(Q)if [ -f $(SERVICE_FFMPEG_SRC_DIR)/Makefile ]; then \
	$(CLEAN_ATP_ENVS) O=$$($(2)_BDIR)/ PROGRAM_NAME=$$($(2)_REAL_PROGRAM_NAME) $(MAKE1) -C $(SERVICE_FFMPEG_SRC_DIR) clean; \
	fi
endef

$(eval $(call ATPTARGETS,package/atp/libraries/opensrc/ffmpeg,ffmpeg,BUILD_DMS))
