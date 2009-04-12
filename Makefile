# Makefile for the Oscar Framework.
# Copyright (C) 2008 Supercomputing Systems AG
# 
# This library is free software; you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by the Free
# Software Foundation; either version 2.1 of the License, or (at your option)
# any later version.
# 
# This library is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
# details.
# 
# You should have received a copy of the GNU Lesser General Public License along
# with this library; if not, write to the Free Software Foundation, Inc., 51
# Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

# Disable make's built-in rules
MAKE += -RL --no-print-directory
SHELL := $(shell which bash)

# This includes the framework configuration
-include .config

# All the supported board configurations.
BOARDS := LEANXCAM INDXCAM MESA_SR4K

# Module lists for each board configuration.
MODULES_MESA_SR4K := log bmp ipc sup dspl dma
MODULES_LEANXCAM := $(MODULES_MESA_SR4K) cam cpld sim swr srd frd hsm cfg clb vis gpio jpg
MODULES_INDXCAM := $(MODULES_LEANXCAM) lgx

# Generate list modules for the chosen board configuration and a list with all possible moduels (for clean and such).
MODULES := $(MODULES_$(CONFIG_BOARD))
MODULES_ALL := $(sort $(wildcard $(foreach i, $(BOARDS), $(MODULES_$(i)))))

# Header files needed by an application using this framework
HEADERS = oscar_types_host.h oscar_types_target.h oscar_host.h oscar_target.h oscar.h oscar_error.h oscar_dependencies.h oscar_version.h oscar_target_type.h

# Executable to create the static library
AR_host = ar -rcs
AR_target = bfin-uclinux-ar -rcs

# Modes to compile this module in.
MODES := host target target_sim
MODES += $(addsuffix _dbg, $(MODES))

# Helper function to access stacked, mode-dependent variables.
varnames = $(filter $(.VARIABLES), $(foreach i, $(shell seq 1 $(words $(subst _, , $(1)))), $(subst $() ,_,$(wordlist 1, $i, $(subst _, , $(1))))))
firstvar = $($(lastword $(call varnames, $(1))))
allvars = $(foreach i, $(call varnames, $(1)), $($i))

# Default target which builds all modules for the selected board configuration.
.PHONY: all $(MODES)
all: $(MODES)

# Targets to build in a specific build mode and create the library.
$(MODES): %: copy_headers staging/lib/libosc_%.a needs_config
staging/lib/libosc_%.a: modules_% oscar_%
	mkdir -p $(dir $@)
	$(call firstvar, AR_$*) $@ $(addsuffix /*_$*.o, $(MODULES) .)

# Routing individual object file requests directly to the compile Makefile
.PHONY: %.o
%.o:
	$(MAKE) -f Makefile_module $@

# Copy all neccessary header files to the staging directory.
.PHONY: copy_headers
copy_headers:
	rm -rf staging/inc
	mkdir -p staging/inc
	cp -r $(HEADERS) include staging/inc
	cp $(wildcard $(addsuffix /*_pub.h, $(MODULES))) staging/inc

# Targets to compile only the modules in a specific mode.
MODULE_TARGETS := $(addprefix modules_, $(MODES))
.PHONY: $(MODULE_TARGETS)
$(MODULE_TARGETS): modules_%: $(addsuffix /%, $(MODULES)) needs_config

# Call this as oscar_$(MODE) to only build them main oscar files.
.PHONY: oscar_%
oscar_%:
	$(MAKE) -f Makefile_module $*

# Produce a target of the form "foo/%" for every directory foo that contains a Makefile
define subdir_target
$(1)%:
	$(MAKE) -C $(1) $$*
endef
$(foreach i, $(wildcard */Makefile), $(eval $(call subdir_target,$(dir $(i)))))

# Use this target as a prerequisite in a target that should fail if the framework has not yet been configured.
.PHONY: needs_config
needs_config:
	@ [ -e ".config" ] || { echo "The framework has not yet been configured. The configuration process starts now."; ./configure; }

# Target to explicitly start the configuration process.
.PHONY: config
config:
	@ ./configure

# Target that gets called by the configure script after the configuration.
.PHONY: reconfigure
reconfigure:
	{ echo "/* Automatically generated file. Do not edit. */"; echo "#define TARGET_TYPE_$(CONFIG_BOARD)"; } > oscar_target_type.h
ifeq '$(CONFIG_USE_FIRMWARE)' 'y'
	rm -rf "lgx"
	cp -r $(CONFIG_FIRMWARE_PATH)/lgx .
endif

# Builds the doxygen documentation.
.PHONY: doc
doc:
	rm -rf doc/{html,latex}
	doxygen documentation/oscar.doxygen
	ln -sf html/index.html documentation/index.html

# Cleans the framework and all modules
.PHONY: clean
clean: %: $(addsuffix /%, $(MODULES_ALL)) oscar_clean
	rm -rf staging
	rm -rf doc/{html,latex,index.html}

# Cleans everything not intended for source distribution
.PHONY: distclean
distclean: clean
	rm -f .config
	rm -rf lgx
