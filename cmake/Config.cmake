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

# Config sets the final and non-user configuration

include(cmake/UserConfig.cmake)

# sets final build types
if(USER_BUILD_TYPE STREQUAL "debug")
	set(INNER_BUILD_TYPE_CAPITAL Debug)
	set(INNER_BUILD_TYPE debug)
else()
	set(INNER_BUILD_TYPE_CAPITAL Release)
	set(INNER_BUILD_TYPE release)
endif()

cmake_path(SET PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
cmake_path(SET PROJECT_BUILD_DIR "${PROJECT_DIR}/build")
