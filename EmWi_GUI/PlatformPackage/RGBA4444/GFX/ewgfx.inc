
EMWI_GFX_LIB = ewgfx-eval

ifeq ($(ROTATION),90)
  EMWI_GFX_LIB := $(EMWI_GFX_LIB)-r90
else ifeq ($(ROTATION),180)
  EMWI_GFX_LIB := $(EMWI_GFX_LIB)-r180
else ifeq ($(ROTATION),270)
  EMWI_GFX_LIB := $(EMWI_GFX_LIB)-r270
endif

ifeq ($(MEMORY_USAGE),1)
  EMWI_GFX_LIB := $(EMWI_GFX_LIB)-mu
endif

EMWI_GFX_FILES :=                                                              \
                    ewextgfx.c                                                 \

