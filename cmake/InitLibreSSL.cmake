# Copyright (C) 2025 Davyd Stalytenko
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

# InitLibreSSL tryes to find, build or fallback to system libressl

cmake_minimum_required(VERSION 3.16)

include(cmake/Config.cmake)

cmake_path(SET SSL_DIR "${PROJECT_DIR}/libressl")
cmake_path(SET SSL_INSTALL_DIR "${PROJECT_BUILD_DIR}/libressl")
cmake_path(SET SSL_CMAKE_DIR "${SSL_INSTALL_DIR}/lib/cmake")

# check if LibreSSL was already built by project
set(SSL_INSTALL_DIR_FOUND FALSE)

if(EXISTS ${SSL_INSTALL_DIR})
	set(SSL_INSTALL_DIR_FOUND TRUE)
endif()

# nothing to do if user forces the use of system libressl
if(${SSL_FORCE_SYSTEM})
	return()
endif()

# sets libressl built by project
if(${SSL_INSTALL_DIR_FOUND})
	set(CMAKE_PREFIX_PATH "${SSL_CMAKE_DIR};${CMAKE_PREFIX_PATH}")
endif()

# if the build is not required, then the one built is used
if(NOT ${SSL_FORCE_BUILD} AND ${SSL_INSTALL_DIR_FOUND})
	return()
endif()

# start build 

# clean up the old installed libressl and save it by renaming it 
if(${SSL_INSTALL_DIR_FOUND})
	string(TIMESTAMP TODAYSDATE "%d-%m-%Y")

	cmake_path(SET TEMP_OLD_SSL_INSTALL_DIR "${PROJECT_BUILD_DIR}/libressl-${TODAYSDATE}-old")

	file(RENAME ${SSL_INSTALL_DIR} ${TEMP_OLD_SSL_INSTALL_DIR})
endif()

file(MAKE_DIRECTORY ${SSL_INSTALL_DIR})

cmake_path(SET SSL_AUTOGEN "${SSL_DIR}/autogen.sh")
cmake_path(SET SSL_CONF "${SSL_DIR}/configure")

# (base) clean & init libressl repo
execute_process(COMMAND git submodule deinit libressl WORKING_DIRECTORY ${PROJECT_DIR})
execute_process(COMMAND git submodule init libressl WORKING_DIRECTORY ${PROJECT_DIR})
execute_process(COMMAND git submodule update libressl WORKING_DIRECTORY ${PROJECT_DIR})

# generate the confirational script
execute_process(COMMAND ${SSL_AUTOGEN} WORKING_DIRECTORY ${SSL_DIR})

# init repo and do the final configuration
execute_process(COMMAND ${SSL_CONF} WORKING_DIRECTORY ${SSL_DIR})

# create cmake configuration, build and install libressl
execute_process(COMMAND cmake -G Ninja
	-DCMAKE_INSTALL_PREFIX=${SSL_INSTALL_DIR}
	-DCMAKE_BUILD_TYPE=${INNER_BUILD_TYPE_CAPITAL}
	${SSL_DIR} WORKING_DIRECTORY ${SSL_DIR})

execute_process(COMMAND cmake --build ${SSL_DIR} --parallel WORKING_DIRECTORY ${SSL_DIR})
execute_process(COMMAND cmake --install ${SSL_DIR} WORKING_DIRECTORY ${SSL_DIR})
