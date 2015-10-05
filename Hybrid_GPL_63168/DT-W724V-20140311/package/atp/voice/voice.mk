ifeq ($(BUILD_VOICE_ATP),y)
include package/atp/voice/voiper/atp/package.mk
endif
ifeq ($(BUILD_VOICE_DT),y)
ifndef GPLREL
include package/atp/voice/voiper/dt/package.mk
include package/atp/voice/netpb/package.mk
include package/atp/voice/rss/package.mk
endif
include package/atp/voice/email/package.mk
endif