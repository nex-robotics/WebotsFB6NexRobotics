# Webots Makefile system 
#
# You may add some variable definitions hereafter to customize the build process
# See documentation in $(WEBOTS_HOME_PATH)/resources/Makefile.include


CXX_SOURCES = \
  Camera.cpp \
  Communication.cpp \
  Device.cpp \
  DeviceManager.cpp \
  FireBird6InputPacket.cpp \
  FireBird6OutputPacket.cpp \
  Packet.cpp \
  Serial.cpp \
  Time.cpp \
  Wrapper.cpp \
  entry_points.cpp

ifndef OSTYPE
 OSTYPE=$(shell uname)
endif

ifeq ($(OSTYPE),Darwin)
 OSTYPE=darwin
endif

ifeq ($(OSTYPE),darwin)
LIBRARIES += -framework IOKit -framework CoreFoundation
endif

USE_C_API = true

space :=
space +=
WEBOTS_HOME_PATH=$(subst $(space),\ ,$(strip $(subst \,/,$(WEBOTS_HOME))))
include $(WEBOTS_HOME_PATH)/resources/Makefile.include
