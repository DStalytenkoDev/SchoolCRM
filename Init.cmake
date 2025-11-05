# Copyright (C) 2025 AUTHORS of SchooliesCave
# SPDX-License-Identifier: GPL-3.0-only
#
# This file is part of SchooliesCave.
# 
# SchooliesCave is free software: you can redistribute it and/or modify it under the terms
# of the GNU General Public License as published by the Free Software Foundation, version 3
# of the License only.
# 
# SchooliesCave is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with SchooliesCave.
# If not, see <https://www.gnu.org/licenses/>.


cmake_minimum_required(VERSION 3.16)

cmake_path(SET proj_dir ${CMAKE_CURRENT_SOURCE_DIR})
cmake_path(SET qt6_dir "${proj_dir}/qt6")
cmake_path(SET qt6_install_dir "${proj_dir}/build/qt6")
cmake_path(SET qt6_prefix_path "${qt6_install_dir}/lib/cmake")
cmake_path(SET proj_cmake_dir "${proj_dir}/build/MainCmakeConf")

cmake_path(SET QT6_INIT_REPO "${qt6_dir}/init-repository")
cmake_path(SET QT6_CONF "${qt6_dir}/configure")

# create dir for qt6 installation and for main cmake configuration
file(MAKE_DIRECTORY ${qt6_install_dir})
file(MAKE_DIRECTORY ${proj_cmake_dir})


if(NOT DEFINED BUILDTYPE)
	set(BUILDTYPE "release")
	message(INFO "The build type was not specified, set to Release")
endif()


if(NOT ${BUILDTYPE} STREQUAL "release" AND NOT ${BUILDTYPE} STREQUAL "debug")
	message(FATAL_ERROR "The BUILDTYPE was set incorrectly")
endif()

execute_process(COMMAND git submodule init qt6 WORKING_DIRECTORY ${proj_dir})
execute_process(COMMAND git submodule update qt6 WORKING_DIRECTORY ${proj_dir})

# init qt6 repo
execute_process(COMMAND ${QT6_INIT_REPO} --no-optional-deps -submodules qtbase,qtscxml -f WORKING_DIRECTORY ${qt6_dir})

# configure qt6 repo
execute_process(COMMAND ${QT6_CONF} -submodules qtbase,qtscxml -${BUILDTYPE}
	-prefix ${qt6_install_dir}
	-nomake examples -nomake tests -nomake benchmarks -nomake manual-tests WORKING_DIRECTORY ${qt6_dir})

# build and install qt6
execute_process(COMMAND cmake --build ${qt6_dir} --parallel WORKING_DIRECTORY ${qt6_dir})
execute_process(COMMAND cmake --install ${qt6_dir} WORKING_DIRECTORY ${qt6_dir})

# just make debug > Debug and release > Release  
if(${BUILDTYPE} EQUAL "debug")
	set(BUILDTYPE_T "Debug")
else()
	set(BUILDTYPE_T "Release")
endif()


# create main cmake configuration
execute_process(COMMAND cmake -DCMAKE_PREFIX_PATH=${qt6_prefix_path}
	-DCMAKE_BUILD_TYPE=${BUILDTYPE_T}
	-G Ninja ${proj_dir} WORKING_DIRECTORY ${proj_cmake_dir})


# print exit message
message(STATUS "The project is inited, configured and dependencies such as qt6 are built.\n \
The project itself is not built, please run \"cmake --build ${proj_cmake_dir}\"")
