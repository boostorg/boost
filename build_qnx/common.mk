ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

#choose release or debug
BOOST_VARIANT ?= release

#set the following to FALSE if generating .pinfo files is causing problems
GENERATE_PINFO_FILES ?= TRUE

#where to install Boost:
#$(INSTALL_ROOT_$(OS)) is pointing to $QNX_TARGET
#by default, unless it was manually re-routed to
#a staging area by setting both INSTALL_ROOT_nto
#and USE_INSTALL_ROOT
BOOST_INSTALL_ROOT ?= $(INSTALL_ROOT_$(OS))

B2_MODULES = --without-mpi --without-graph_parallel
B2_EXTRA_OPTS =

# list of flags passed to the b2 command which are shared by all target architectures
B2_OPTIONS = -q -d2 -j$(firstword $(JLEVEL) 1) \
             ${B2_MODULES} \
             --build-type=minimal target-os=qnxnto toolset=qcc \
             --prefix=$(BOOST_INSTALL_ROOT) --includedir=$(BOOST_INSTALL_ROOT)/usr/include \
             --libdir=$(BOOST_INSTALL_ROOT)/$(CPUVARDIR)/usr/lib \
             --user-config=$(PROJECT_ROOT)/user-config-python38.jam \
             runtime-link=shared link=shared variant=$(BOOST_VARIANT) define=BOOST_SYSTEM_NO_DEPRECATED \
             ${B2_EXTRA_OPTS}

FLAGS   += -g
LDFLAGS += -Wl,--build-id=md5 -lang-c++ -lsocket

FLAGS   += -Vgcc_nto$(CCVER) -Wno-ignored-attributes -I$(PROJECT_ROOT)/libs/predef/include/boost/predef/other
LDFLAGS += -Vgcc_nto$(CCVER)

#override 'all' target to bypass the default QNX build system
ALL_DEPENDENCIES = install

define PINFO
endef
PINFO_STATE=Experimental
USEFILE=

include $(MKFILES_ROOT)/qtargets.mk

ifndef NO_TARGET_OVERRIDE

BUILD_DIR = $(PWD)/build

$(PROJECT_ROOT)/../b2:
	cd $(PROJECT_ROOT)/.. && ./bootstrap.sh --with-python-version=3.8

B2_CMD = export CPUVARDIR=$(CPUVARDIR) && \
         export CCVER=$(CCVER) && \
         cd $(PROJECT_ROOT)/.. && ./b2 $(B2_OPTIONS) --build-dir=$(BUILD_DIR) cflags="$(FLAGS)" linkflags="$(LDFLAGS)"

B2_CMD_TEST = export CPUVARDIR=$(CPUVARDIR) && \
         export CCVER=$(CCVER) && \
         $(PROJECT_ROOT)/../b2 $(B2_OPTIONS) --build-dir=$(BUILD_DIR) cflags="$(FLAGS)" linkflags="$(LDFLAGS)"

install check: $(PROJECT_ROOT)/../b2
	@mkdir -p build
	$(B2_CMD) install
	$(ADD_USAGE_TO_LIBS)

clean iclean spotless:
	rm -fr build

uninstall:

test.%: $(PROJECT_ROOT)/../b2
	cd $(PROJECT_ROOT)/../libs/$(subst test.,,$@)/test && \
	$(B2_CMD_TEST) testing.execute=off


endif

#everything down below deals with the generation of the PINFO
#information for shared objects that is used by the QNX build
#infrastructure to embed metadata in the .so files, for example
#data and time, version number, description, etc. Metadata can
#be retrieved on the target by typing 'use -i <path to boost .so file>'.
#this is optional: setting GENERATE_PINFO_FILES to FALSE will disable
#the insertion of metadata in .so files.
ifeq ($(GENERATE_PINFO_FILES), TRUE)
define ADD_USAGE_TO_LIBS
@for lib in `find $(BOOST_INSTALL_ROOT)/$(CPUVARDIR)/usr/lib -name "libboost*.so.1.82.0"`; do $(MAKE) -B -s --no-print-directory NO_TARGET_OVERRIDE=1 $$lib; done
endef

#the following rule is called by ADD_USAGE_TO_LIBS
#in order to generate the .pinfo files for the shared libraries
%.so.1.82.0:
	$(ADD_PINFO)
	$(ADD_USAGE)
	rm -f $(pinfo_name)

else
define ADD_USAGE_TO_LIBS
endef
endif
