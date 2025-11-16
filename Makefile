################################################################################
# \file Makefile
# \version 1.0
#
# \brief
# Top-level application make file.
#
################################################################################
# \copyright
# Copyright 2018-2023, Cypress Semiconductor Corporation (an Infineon company)
# SPDX-License-Identifier: Apache-2.0
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
################################################################################
#LINUX_PATH=$(subst \,/,$(CWD))
include ./lot3_project.mk

################################################################################
# Basic Configuration
################################################################################

# Type of ModusToolbox Makefile Options include:
#
# COMBINED    -- Top Level Makefile usually for single standalone application
# APPLICATION -- Top Level Makefile usually for multi project application
# PROJECT     -- Project Makefile under Application
#
MTB_TYPE=COMBINED
# Target board/hardware (BSP).
# To change the target, it is recommended to use the Library manager
# ('make library-manager' from command line), which will also update Eclipse IDE launch
# configurations.
TARGET=APP_CY8CKIT-041S-MAX

# Defien App Start address and size.
#
#APP_START_ADDR=0x4000
#APP_END_ADDR=0xFFFF
#APP_SIZE_ALL=0xC000
ifdef APP_START_ADDRESS
APP_START_ADDR:=$(subst U,,$(subst u,,$(APP_START_ADDRESS)))
APP_START_ADDR:=$(subst L,,$(subst l,,$(APP_START_ADDR)))
$(info Make--APP_START_ADDR=$(APP_START_ADDR))
else
APP_START_ADDR:=0x4000
endif
ifdef APP_END_ADDRESS
APP_END_ADDR:=$(subst U,,$(subst u,,$(APP_END_ADDRESS)))
APP_END_ADDR:=$(subst L,,$(subst l,,$(APP_END_ADDR)))
$(info Make--APP_END_ADDR=$(APP_END_ADDR))
else
APP_END_ADDR:=0xFFFF
endif
ifdef APP_SIZE
APP_SIZE_ALL:=$(subst U,,$(subst u,,$(APP_SIZE)))
APP_SIZE_ALL:=$(subst L,,$(subst l,,$(APP_SIZE_ALL)))
$(info Make--APP_SIZE_ALL=$(APP_SIZE_ALL))
else
APP_SIZE_ALL:=0x4000
endif


# Name of application (used to derive name of final linked file).
#
# If APPNAME is edited, ensure to update or regenerate launch
# configurations for your IDE.
PRJNAME:=psoc4-bridge-board
APPNAME:=psoc4-bridge-board-app
BOOTNAME:=psoc4-bridge-board-boot
 
# Current path = project path
#
#
PROJECT_PATH = $(CWD)
OUTPUT_PATH=./build/$(TARGET)/$(CONFIG)
APTOOL_PATH=./build/$(TARGET)
$(info Make--PROJECT_PATH=$(PROJECT_PATH))
$(info Make--OUTPUT_PATH=$(OUTPUT_PATH))
$(info Make--LOT3_BOOT_ENABLE=$(LOT3_BOOT_ENABLE))


# Name of toolchain to use. Options include:
#
# GCC_ARM -- GCC provided with ModusToolbox software
# ARM     -- ARM Compiler (must be installed separately)
# IAR     -- IAR Compiler (must be installed separately)
#
# See also: CY_COMPILER_PATH below
TOOLCHAIN=GCC_ARM

# Default build configuration. Options include:
#
# Debug -- build with minimal optimizations, focus on debugging.
# Release -- build with full optimizations
# Custom -- build with custom configuration, set the optimization flag in CFLAGS
#
# If CONFIG is manually edited, ensure to update or regenerate launch configurations
# for your IDE.
#CONFIG=Debug
#ifeq (${BuildMode}, Debug)
#CONFIG=Debug
#else
#CONFIG=Release
#endif
$(info Make--CONFIG=$(CONFIG))

# If set to "true" or "1", display full command-lines when building.
VERBOSE=

################################################################################
# Advanced Configuration
################################################################################

# Enable optional code that is ordinarily disabled by default.
#
# Available components depend on the specific targeted hardware and firmware
# in use. In general, if you have
#
#    COMPONENTS=foo bar
#
# ... then code in directories named COMPONENT_foo and COMPONENT_bar will be
# added to the build
#
COMPONENTS=

# Like COMPONENTS, but disable optional code that was enabled by default.
DISABLE_COMPONENTS=

# By default the build system automatically looks in the Makefile's directory
# tree for source code and builds it. The SOURCES variable can be used to
# manually add source code to the build process from a location not searched
# by default, or otherwise not found by the build system.
SOURCES=

# Like SOURCES, but for include directories. Value should be paths to
# directories (without a leading -I).
INCLUDES= ./source ./source/driver ./source/system ./source/aging \
./source/longrun ./source/testing ./source/bridge ./source/device \
./source/module_api


# Add additional defines to the build process (without a leading -D).
ifeq ($(CONFIG), Debug)
DEFINES:=DEBUG
else
DEFINES:=NDEBUG
endif

# Select softfp or hardfp floating point. Default is softfp.
VFP_SELECT=

# Additional / custom C compiler flags.
#
# NOTE: Includes and defines should use the INCLUDES and DEFINES variable
# above.
ifeq ($(CONFIG), Debug)
CFLAGS=  -O0 -g2 -fstack-protector-all
$(info Make--CFLAGS= -O0 -g2)
else
#---------------------------
ifeq ($(LOT3_BOOT_ENABLE), 1U)
CFLAGS= -Os
else
CFLAGS= -Os -fstack-protector-all
endif
$(info Make--CFLAGS= -Os)
#---------------------------
endif

# Additional / custom C++ compiler flags.
#
# NOTE: Includes and defines should use the INCLUDES and DEFINES variable
# above.
CXXFLAGS=

# Additional / custom assembler flags.
#
# NOTE: Includes and defines should use the INCLUDES and DEFINES variable
# above.
ASFLAGS=

# Additional / custom linker flags.
ifeq ($(CONFIG), Debug)
LDFLAGS= -Wl,--section-start=.text=0x0000
else
ifeq ($(LOT3_BOOT_ENABLE), 1U)
LDFLAGS= -Wl,--section-start=.text=0x0000
else
#rebase section start address from 0x0000 to 0x4000
LDFLAGS= -Wl,--section-start=.text=$(APP_START_ADDR)
endif
endif

# Additional / custom libraries to link in to the application.
LDLIBS=

# Path to the linker script to use (if empty, use the default linker script).
LINKER_SCRIPT=

# Custom pre-build commands to run.
PREBUILD= $(shell ./extract_macros.sh)
# Custom pre-build commands to run.
#PREBUILD=
## Prevent repeated execution
#prebuild:
#	$(shell ./extract_macros.sh)

# Custom post-build commands to run.
ifeq ($(CONFIG), Debug)
#debug#################################################
POSTBUILD= $(CY_TOOLS_PATHS)/gcc/bin/arm-none-eabi-objcopy.exe -O binary $(OUTPUT_PATH)/$(APPNAME).elf $(OUTPUT_PATH)/$(APPNAME).bin
else
#release#################################################
ifeq ($(LOT3_BOOT_ENABLE), 1U)
POSTBUILD= $(CY_TOOLS_PATHS)/gcc/bin/arm-none-eabi-objcopy.exe -O binary $(OUTPUT_PATH)/$(APPNAME).elf $(OUTPUT_PATH)/$(BOOTNAME).bin && \
$(APTOOL_PATH)/srec_cat.exe $(OUTPUT_PATH)/$(BOOTNAME).bin  -Binary -fill 0xFF 0x0000 $(APP_START_ADDR) -offset 0x0000 -o $(OUTPUT_PATH)/$(BOOTNAME)_F.hex -Intel -Output_Block_Size=16 && \
cp $(OUTPUT_PATH)/$(BOOTNAME)_F.hex $(APTOOL_PATH)
else
POSTBUILD= $(CY_TOOLS_PATHS)/gcc/bin/arm-none-eabi-objcopy.exe -O binary $(OUTPUT_PATH)/$(APPNAME).elf $(OUTPUT_PATH)/$(APPNAME).bin && \
$(APTOOL_PATH)/GenBinCRC32.exe $(OUTPUT_PATH)/$(APPNAME).bin -s $(APP_START_ADDR) -e $(APP_END_ADDR) -o $(OUTPUT_PATH)/$(APPNAME)_F.bin && \
$(APTOOL_PATH)/srec_cat.exe $(OUTPUT_PATH)/$(APPNAME)_F.bin -Binary -fill 0xFF 0x0000 $(APP_SIZE_ALL) -offset $(APP_START_ADDR) -o $(OUTPUT_PATH)/$(APPNAME)_F.hex -Intel -obs=16 && \
$(APTOOL_PATH)/srec_cat.exe $(APTOOL_PATH)/$(BOOTNAME)_F.hex -Intel $(OUTPUT_PATH)/$(APPNAME)_F.hex -Intel -o  $(OUTPUT_PATH)/$(PRJNAME)_WL_RELEASE.hex -Intel -Output_Block_Size=16 && \
$(APTOOL_PATH)/srec_cat.exe $(APTOOL_PATH)/SetFlashLock_BootLock.hex -Intel $(OUTPUT_PATH)/$(PRJNAME)_WL_RELEASE.hex -Intel -o  $(OUTPUT_PATH)/$(PRJNAME)_WL_RELEASE.hex -Intel -Output_Block_Size=16
endif
#release end##################################################
endif
################################################################################
# Paths
################################################################################

# Relative path to the project directory (default is the Makefile's directory).
#
# This controls where automatic source code discovery looks for code.
CY_APP_PATH=

# Relative path to the shared repo location.
#
# All .mtb files have the format, <URI>#<COMMIT>#<LOCATION>. If the <LOCATION> field
# begins with $$ASSET_REPO$$, then the repo is deposited in the path specified by
# the CY_GETLIBS_SHARED_PATH variable. The default location is one directory level
# above the current app directory.
# This is used with CY_GETLIBS_SHARED_NAME variable, which specifies the directory name.
CY_GETLIBS_SHARED_PATH=../

# Directory name of the shared repo location.
#
CY_GETLIBS_SHARED_NAME=mtb_shared

# Absolute path to the compiler's "bin" directory. The variable name depends on the
# toolchain used for the build. Refer to the ModusToolbox user guide to get the correct
# variable name for the toolchain used in your build.
#
# The default depends on the selected TOOLCHAIN (GCC_ARM uses the ModusToolbox
# software provided compiler by default).
CY_COMPILER_GCC_ARM_DIR=


# Locate ModusToolbox helper tools folders in default installation
# locations for Windows, Linux, and macOS.
CY_WIN_HOME=$(subst \,/,$(USERPROFILE))
CY_TOOLS_PATHS ?= $(wildcard \
    $(CY_WIN_HOME)/ModusToolbox/tools_* \
    $(HOME)/ModusToolbox/tools_* \
    /Applications/ModusToolbox/tools_*)

# If you install ModusToolbox software in a custom location, add the path to its
# "tools_X.Y" folder (where X and Y are the version number of the tools
# folder). Make sure you use forward slashes.
CY_TOOLS_PATHS+=

# Default to the newest installed tools folder, or the users override (if it's
# found).
CY_TOOLS_DIR=$(lastword $(sort $(wildcard $(CY_TOOLS_PATHS))))

ifeq ($(CY_TOOLS_DIR),)
$(error Unable to find any of the available CY_TOOLS_PATHS -- $(CY_TOOLS_PATHS). On Windows, use forward slashes.)
endif

$(info Tools Directory: $(CY_TOOLS_DIR))


include $(CY_TOOLS_DIR)/make/start.mk
