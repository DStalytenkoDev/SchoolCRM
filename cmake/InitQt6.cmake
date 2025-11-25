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

# InitQt6 tryes to find, build or fallback to system qt

cmake_minimum_required(VERSION 3.16)

include(cmake/Config.cmake)

cmake_path(SET QT6_DIR "${PROJECT_DIR}/qt6")
cmake_path(SET QT6_INSTALL_DIR "${PROJECT_BUILD_DIR}/qt6")
cmake_path(SET QT6_PREFIX_PATH "${QT6_INSTALL_DIR}/lib/cmake")

# find Qt6
set(QT6_INSTALL_DIR_FOUND FALSE)

if(EXISTS ${QT6_INSTALL_DIR})
	set(QT6_INSTALL_DIR_FOUND TRUE)
endif()

set(QT6_PREFIX_PATH_FOUND FALSE)

if(EXISTS ${QT6_PREFIX_PATH})
	set(QT6_PREFIX_PATH_FOUND TRUE)
endif()

# nothing to do if user forces the use of system qt6
if(${QT6_FORCE_SYSTEM})
	return()
endif()

# sets qt6 built by project
if(${QT6_PREFIX_PATH_FOUND})
	set(CMAKE_PREFIX_PATH ${QT6_PREFIX_PATH})
endif()

# if the build is not required, then the one built used
if(NOT ${QT6_FORCE_BUILD} AND ${QT6_PREFIX_PATH_FOUND})
	return()
endif()

# start build 

# clean up the old installed qt6 and save it by renaming it
if(${QT6_INSTALL_DIR_FOUND})
	string(TIMESTAMP TODAYSDATE "%d-%m-%Y")

	cmake_path(SET TEMP_OLD_QT_INSTALL_DIR "${PROJECT_BUILD_DIR}/qt6-${TODAYSDATE}-old")

	file(RENAME ${QT6_INSTALL_DIR} ${TEMP_OLD_QT_INSTALL_DIR})
endif()

file(MAKE_DIRECTORY ${QT6_INSTALL_DIR})

cmake_path(SET QT6_INIT_REPO "${QT6_DIR}/init-repository")
cmake_path(SET QT6_CONF "${QT6_DIR}/configure")

# (base) clean & init qt6 repo
execute_process(COMMAND git submodule deinit qt6 WORKING_DIRECTORY ${PROJECT_DIR})
execute_process(COMMAND git submodule init qt6 WORKING_DIRECTORY ${PROJECT_DIR})
execute_process(COMMAND git submodule update qt6 WORKING_DIRECTORY ${PROJECT_DIR})

# (full) init qt6 repo
execute_process(COMMAND ${QT6_INIT_REPO} --no-optional-deps -submodules qtbase,qtscxml -f WORKING_DIRECTORY ${QT6_DIR})

# configure qt6 repo
execute_process(COMMAND ${QT6_CONF} -submodules qtbase,qtscxml -${INNER_BUILD_TYPE}
	-prefix ${QT6_INSTALL_DIR}
	-nomake examples -nomake tests -nomake benchmarks -nomake manual-tests WORKING_DIRECTORY ${QT6_DIR})

# build and install qt6
execute_process(COMMAND cmake --build ${QT6_DIR} --parallel WORKING_DIRECTORY ${QT6_DIR})
execute_process(COMMAND cmake --install ${QT6_DIR} WORKING_DIRECTORY ${QT6_DIR})
