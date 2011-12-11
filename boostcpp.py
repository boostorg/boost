# Boost.Build support specific for the Boost C++ Libraries.
# Copyright Vladimir Prus 2002-2010.
# Copyright Dave Abrahams 2005-2006.
# Copyright Rene Rivera 2005-2007.
# Copyright Douglas Gregor 2005.
#
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

import b2.util.option as option
import b2.build.feature as feature
import b2.build.virtual_target as virtual_target
import b2.build.targets as targets
import b2.build.property_set as property_set
import b2.build_system as build_system
import b2.build.configure as configure
import b2.tools.stage as stage
import b2.tools.package as package

import b2.util.regex as regex
import b2.util.set as set

from b2.build.alias import AliasTarget, alias
from b2.tools.message import message

from b2.manager import get_manager
from b2.util import bjam_signature, value_to_jam
from b2.util.sequence import unique



import sys
import os

# FIXME: temporary fix until I port the xsltproc module
feature.feature("xsl:param", [], ["free"])


##############################################################################
# 
# 0. General setup. Parse options, check them.
#
##############################################################################  

BOOST_ROOT = get_manager().projects().loaded_tool_module_path_[__name__]

print "HERE I AM"
@bjam_signature((["version"],))
def set_version(version):
    global BOOST_VERSION, BOOST_VERSION_TAG

    BOOST_VERSION = version
    pieces = version.split(".")
    if pieces[2] == "0":
        pieces = pieces[0:2]


    BOOST_VERSION_TAG = "_".join(pieces)

# Option to choose how many variants to build. The default is "minimal".
build_type = option.get("build-type", "minimal")
if not build_type in ["minimal", "complete"]:
    get_manager().errors()("The value of the --build-type option should be either 'complete' or 'minimal'")

# What kind of layout are we doing?
layout = option.get("layout")
# On Windows, we used versioned layout by default in order to
# be compatible with autolink. On other systems, we use system
# layout which is what every other program uses. Note that windows
# check is static, and won't 
if not layout:
    if os.name == "nt":
        layout = "versioned"
    else:
        layout = "system"

if layout == "system" and build_type == "complete":
    get_manager().errors()(
        "Cannot use --layout=system with --build-type complete.\n"
        "Please used either --layout=versioned or --layout=tagged\n"
        "if you wish to build multiple variants.")
    
# Possible stage only location.
stage_locate = option.get("stagedir", "stage")
BOOST_STAGE_LOCATE = stage_locate

# Custom build ID.
build_id = option.get("buildid")
if build_id:
    BUILD_ID = re.sub("[*\\/:.\"\' ]", "_", build_id)

# Python build id (only for Python libraries)
python_id = option.get("python-buildid")
if python_id:
    PYTHON_ID = re.sub("[*\\/:.\"\' ]", "_", python_id)

##############################################################################
#
# 1. The 'tag' function that adds decoration suitable to the properties if
# versioned or tagged layout is requested. This function is called from 
# Jamroot
#
##############################################################################  

def tag(name, type, ps):
    if type in ["STATIC_LIB", "SHARED_LIB", "IMPORT_LIB"]:

        pass


    ##     FIXME
    ##     if $(layout) = versioned
    ##     {
    ##         result = [ common.format-name
    ##             <base> <toolset> <threading> <runtime> -$(BOOST_VERSION_TAG)
    ##             -$(BUILD_ID)
    ##             : $(name) : $(type) : $(property-set) ] ;
    ##     }        
    ##     else if $(layout) = tagged
    ##     {
    ##         result = [ common.format-name
    ##             <base> <threading> <runtime>
    ##             -$(BUILD_ID)
    ##             : $(name) : $(type) : $(property-set) ] ;            
    ##     }
    ##     else if $(layout) = system
    ##     {
    ##         result = [ common.format-name
    ##             <base> 
    ##             -$(BUILD_ID)
    ##             : $(name) : $(type) : $(property-set) ] ;
    ##     }
    ##     else
    ##     {
    ##         ECHO "error: invalid layout '$(layout:E=)'" ;
    ##         EXIT ;               
    ##     }
                
    ##     # Optionally add version suffix. On NT, library with version suffix
    ##     # will not be recognized by linkers. On CYGWIN, we get strage
    ##     # duplicate symbol errors when library is generated with version
    ##     # suffix. On OSX, version suffix is not needed -- the linker expects
    ##     # the libFoo.1.2.3.dylib format. AIX linkers do not accept version
    ##     # suffixes either. Pgi compilers can not accept library with version
    ##     # suffix.
    ##     if $(type) = SHARED_LIB &&
    ##       ( ! ( [ $(property-set).get <target-os> ] in windows cygwin darwin aix ) &&
    ##         ! ( [ $(property-set).get <toolset> ] in pgi ) )
    ##     {
    ##         result = $(result).$(BOOST_VERSION)  ;
    ##     }

    ##     return $(result) ;
    ## }    


##############################################################################
#
# 2. Declare targets that build and install all libraries. Specifically:
#
#    - 'stage-proper' that puts all libraries in stage/lib
#    - 'install-proper' that install libraries and headers to system location
#    - 'stage-unversioned' that creates links to libraries without boost veriosn
#       in name
#    - 'install-unversioned' which creates unversioned linked to installed
#       libraries.
#
##############################################################################  

# Worker function suitable to the 'generate' metatarget. Creates a link
# to 'source', striping any version number from the name.
def make_unversioned_links(project, name, ps, sources):

    result = []
    filtered = []
    nt = os.name == "nt"

    # Collect the libraries that have the version number in 'filtered'.
    for s in sources:
        if nt:
            m = s.name().endswith(".lib")
        else:
            m = re.match("(.*[.]so[.0-9]+)" "(.*[.]dylib)" "(.*[.]a)", s.name())

        if m:
            filtered.append(s)

    # Create links without version.
    for s in filtered:
        a = virtual_target.NonScanningAction(s, "symlink.ln", s.action().properties())
        name = s.name()
        
        if nt:
            m = re.match("(.*)-[0-9_]+(.*[.]lib)", name)
        else:
            m = re.match("(.*)-[0-9_]+(.*[.]so)[.0-9]*", name)
            if not m:
                m = re.match("(.*)-[0-9_]+(.*[.]dylib)", name)
                if not m:
                    m = re.match("(.*)-[0-9_]+(.*[.]a)", name)
                    if not m:
                        m = re.match("(.*)-[0-9_]+(.*[.]dll[.]a)", name)

        new_name = m.group(1) + m.group(2)
        result.append(virtual_target.FileTarget(new_name, s.type(), project, a,
                                                exact=True))

    return result

def declare_install_and_stage_proper_targets(libraries, headers):

    install_requirements = ["<install-source-root>" + BOOST_ROOT + "/boost"]
    if layout == "versioned":
        install_requirements.append("<install-header-subdir>boost-" + BOOST_VERSION_TAG + "/boost")
    else:
        install_requirements.append("<install-header-subdir>boost")

    if os.name == "nt":
        default_prefix = "C:/Boost"
    else:
        default_prefix = "/usr/local"
        
    install_requirements.append("<install-default-prefix>" + default_prefix)

    p = get_manager().projects().current()
    package.install("install-proper",
                    install_requirements + ["<install-no-version-symlinks>on"],
                    [],
                    ["libs/%s/build" % l for l in libraries],
                    headers)
    p.mark_targets_as_explicit(["install-proper"])

    stage.install("stage-proper",
                  ["libs/%s/build" % l for l in libraries],
                  ["<location>" + stage_locate + "/lib",
                   "<install-dependencies>on", "<install-type>LIB",
                   "<install-no-version-symlinks>on"])
    p.mark_targets_as_explicit(["stage-proper"])

    gr = value_to_jam(make_unversioned_links)
    gr = "<generating-rule>" + gr

    
    if layout == "versioned":
        generate("stage-unversioned", ["stage-proper"], [gr])    
        generate("install-unversioned", ["install-proper"], [gr])
    else:
        # Create do-nothing aliases.
        alias("stage-unversioned")
        alias("install-unversioned")
        
    p.mark_targets_as_explicit(["stage-unversioned", "install-unversioned"])


##############################################################################  
#
#  3. Declare top-level targets 'stage' and 'install'. These examine
#  the --build-type option and, in case it's 'complete', build the
#  'install-proper' and 'stage-proper' targets with a number of
#  property sets.
#
##############################################################################  

# The names of all top-level targets that were built.
top_level_targets = []

class TopLevelTarget(AliasTarget):

    def __init__(self, name, project, sources, requirements, default_build,
                 usage_requirements):

        AliasTarget.__init__(self, name, project, sources, requirements, default_build,
                             usage_requirements)

        # On Linux, we build release variant by default, since few users will
        # ever want to debug C++ Boost libraries, and there's no ABI
        # incompatibility between debug and release variants. We build
        # shared and static libraries since that's what most packages
        # seem to provide (.so in libfoo and .a in libfoo-dev).
        self.minimal_properties = property_set.create([
            "<variant>release", "<threading>multi", "<link>shared", "<link>static",
            "<runtime-link>shared"])
        # On Windows, new IDE projects use:
        #
        #   runtime-link=dynamic, threading=multi, variant=(debug|release)
        #
        # and in addition, C++ Boost's autolink defaults to static linking.
        self.minimal_properties_win = property_set.create([
            "<variant>debug", "<variant>release", "<threading>multi", "<link>static",
            "<runtime-link>shared"])

        self.complete_properties = property_set.create([
            "<variant>debug", "<variant>release",
            "<threading>single", "<threading>multi"
            "<link>shared", "<link>static",
            "<runtime-link>shared", "<runtime-link>static"])

    def generate(self, ps):
        global top_level_targets
        top_level_targets.append(self.name())

        if build_type == "minimal":

            os = ps.get('target-os')
            # Because we completely override parent's 'generate'
            # we need to check for default value of feature ourself.
            if not os:
                os = feature.get('target-os').default()

            if os == "windows":
                expanded = targets.apply_default_build(ps, self.minimal_properties_win)
            else:
                expanded = targets.apply_default_build(ps, self.minimal_properties)
            return self.build_multiple(expanded)

        elif build_type == "complete":

            expanded = targets.apply_default_build(ps, self.complete_properties)
            
            # Filter inappopriate combinations
            filtered = []
            for p in expanded:

                # See comment in handle-static-runtime regarding this logic.
                if ps.get("link") == ["shared"] and p.get("runtime-link") == ["static"] \
                   and p.get("toolset") != ["cw"]:
                    # Skip this
                    pass
                else:
                    filtered.append(p)
                    
            return build_multiple(filtered)

        else:

            get_manager().errors("Unknown build type")

    def build_multiple(self, property_sets):

        usage_requirements = property_set.empty()
        result = []
        for p in property_sets:
            r = AliasTarget.generate(self, p)
            if r:
                usage_requirements = usage_requirements.add(r.usage_requirements())
                result.extend(r.targets())

        return targets.GenerateResult(usage_requirements, unique(result))

    
def declare_top_level_targets(libraries, headers):
    
    declare_install_and_stage_proper_targets(libraries, headers)

    targets.create_metatarget(TopLevelTarget,
                              "install",
                              ["install-proper", "install-unversioned"])
    targets.create_metatarget(TopLevelTarget,
                              "stage",
                              ["stage-proper", "stage-unversioned"])
    
    message("explain", "\nBuilding the Boost C++ Libraries.\n\n")

    get_manager().projects().current().mark_targets_as_explicit(
        ["install", "stage", "explain"])
    
    # This target is built by default, and will forward to 'stage'
    # after producing some explanations.
    targets.create_metatarget(TopLevelTarget,
                              "forward",
                              ["explain", "stage"])


stage_abs = os.path.abspath(os.path.join(stage_locate, "lib"))

##############################################################################
#
# 4. Add hook to report configuration before the build, and confirmation
# with setup instructions after the build
#
##############################################################################  

def pre_build():
    if "stage" in top_level_targets or "install" in top_level_targets:
        configure.print_component_configuration()

build_system.add_pre_build_hook(pre_build)

def post_build(ok):
    if "forward" in top_level_targets and ok:
        print """\n\nThe Boost C++ Libraries were successfully built!
          
The following directory should be added to compiler include paths:
          
    %s
      
The following directory should be added to linker library paths:
      
    %s
""" % (BOOST_ROOT, stage_abs)

build_system.set_post_build_hook(post_build)

##############################################################################
# 
# 5. Top-level setup 
#
##############################################################################  


def libraries_to_install(existing_libraries):
    # Decides which libraries are to be installed by looking at --with-<library>
    # --without-<library> arguments. Returns the list of directories under "libs"
    # which must be built and installed.

    with_parameter = regex.transform(sys.argv, "--with-(.*)")
    without_parameter = regex.transform(sys.argv, "--without-(.*)")

    if not with_parameter and not without_parameter:
        # Nothing is specified on command line. See if maybe
        # project-config.jam has some choices.
        project_config_libs = bjam.call("peek", "project-config", "libraries")
        with_parameter = regex.transform(project_config_libs, "--with-(.*)")
        without_parameter = regex.transform(project_config_libs, "--without-(.*)")

    # Do some checks.
    if with_parameter and without_parameter:
        get_manager().errors()("both --with-<library> and --without-<library> specified")

    wrong = set.difference(with_parameter, existing_libraries)
    if wrong:
        get_manager().errors()("wrong library name '" + wrong[0] + "' in the --with-<library> option.")

    wrong = set.difference(without_parameter, existing_libraries)
    if wrong:
        get_manager().errors()("wrong library name '" + wrong[0] + "' in the --without-<library> option.")

    if with_parameter:
        return set.intersection(existing_libraries, with_parameter)
    else:
        return set.difference(existing_libraries, without_parameter)

def declare_targets(all_libraries, headers):
  
    configure.register_components(all_libraries)
    
    # Select the libraries to install.
    libraries = libraries_to_install(all_libraries)
    configure.components_building(libraries)

    if option.get("show-libraries", None, True):

        print "The following libraries require building:"
        for l in libraries:
            print "    - " + l
        sys.exit(0)

    declare_top_level_targets(libraries, headers)
