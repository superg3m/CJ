# -------------------------------- GENERATED BY C_BUILD --------------------------------
import os
import sys

def FIND_C_BUILD():
    current_dir = os.path.abspath(os.path.dirname(__file__))

    while True:
        possible_path = os.path.join(current_dir, "c_build")
        if os.path.isdir(possible_path):
            sys.path.insert(0, current_dir)
            return

        parent_dir = os.path.dirname(current_dir)
        if parent_dir == current_dir:
            break
        current_dir = parent_dir

FIND_C_BUILD()
from c_build.source.UserUtilities import *
from c_build.source.Manager import *
# --------------------------------------------------------------------------------------s

pc: ProjectConfig = ProjectConfig(
    project_name = "CJ",
    project_dependencies = [""],
    project_debug_with_visual_studio = True,
    project_rebuild_project_dependencies = False,
    project_executable_procedures  = ["cj_test.exe"]
)

cc: CompilerConfig = CompilerConfig(
    compiler_name = C_BUILD_COMPILER_NAME() if C_BUILD_IS_DEPENDENCY() else "INVALID_COMPILER",
    compiler_std_version = "c11",
    compiler_warning_level = "",
    compiler_disable_specific_warnings = [""],
    compiler_treat_warnings_as_errors = True,
    compiler_disable_warnings  = False,
    compiler_disable_sanitizer = True
)

if IS_WINDOWS() and not C_BUILD_IS_DEPENDENCY():
    cc.compiler_name = "cl"
if IS_DARWIN() and not C_BUILD_IS_DEPENDENCY():
    cc.compiler_name = "clang"
elif IS_LINUX() and not C_BUILD_IS_DEPENDENCY():
    cc.compiler_name = "gcc"

# Do different things depending on the platform
if cc.compiler_name == "cl":
    cc.compiler_warning_level = "4"
    cc.compiler_disable_specific_warnings = ["5105", "4668", "4820", "4996"]
else:
    cc.compiler_warning_level = "all"
    cc.compiler_disable_specific_warnings = ["deprecated", "parentheses", "unreachable-code-generic-assoc"]
    
libs = []
if IS_WINDOWS():
	windows_libs = ["User32.lib", "Gdi32.lib"] if cc.compiler_name == "cl" else ["-lUser32", "-lGdi32"]
	libs += windows_libs

procedures_config = {
    "cj_test executable": ProcedureConfigElement(
        build_directory = f"./build_{cc.compiler_name}",
        output_name = f"cj_test.exe",
        source_files = ["../cj.c", "../Test/cj_test.c"],
        additional_libs = libs,
        compile_time_defines = [],
        compiler_inject_into_args = [],
        include_paths = [
            "../",
        ],
    ),
}

manager: Manager = Manager(cc, pc, procedures_config)
manager.build_project()
# -------------------------------------------------------------------------------------