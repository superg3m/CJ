# -------------------------------- GENERATED BY C_BUILD --------------------------------
from c_build.source.Utilities import *
from c_build.source.Manager import *
# --------------------------------------------------------------------------------------
compiler_name = C_BUILD_COMPILER_NAME() if C_BUILD_IS_DEPENDENCY() else "cl"
compiler_std_version = "c11"
compiler_warning_level = ""
compiler_disable_specific_warnings = []
compiler_treat_warnings_as_errors = True
compiler_disable_warnings = False

project_name = "CJ"
project_dependencies = ["ckg"]
project_rebuild_project_dependencies = False
project_debug_with_visual_studio = False
project_executable_procedures = ["cj_test.exe"]

if compiler_name == "cl":
    compiler_warning_level = "3"
    compiler_disable_specific_warnings = ["5105", "4668", "4820", "4996"]
elif compiler_name in ["gcc", "cc", "clang"]:
    compiler_warning_level = "all"
    compiler_disable_specific_warnings = ["deprecated", "parentheses"]

libs = [f"../ckg/build_{compiler_name}/{C_BUILD_LIB("ckg", compiler_name)}"]
if IS_WINDOWS():
	windows_libs = ["User32.lib", "Gdi32.lib"] if compiler_name == "cl" else ["-lUser32", "-lGdi32"]
	libs += windows_libs

procedures_config = {
    "cj_test executable": {
        "build_directory": f"./build_{compiler_name}",
        "output_name": f"cj_test.exe",
        "source_files": ["../cj.c", "../Test/cj_test.c"],
        "additional_libs": libs,
        "compile_time_defines": [],
        "compiler_inject_into_args": [],
        "include_paths": [
             "../",
             "../ckg"
        ],
    },
}

compiler_config = {key: value for key, value in locals().items() if key.startswith('compiler_')}
project_config = {key: value for key, value in locals().items() if key.startswith('project_')}

manager: Manager = Manager(compiler_config, project_config, procedures_config)
manager.build_project()
# -------------------------------------------------------------------------------------
