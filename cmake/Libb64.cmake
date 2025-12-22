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

# Just compiles the Libb64 as a static lib

cmake_minimum_required(VERSION 3.16)

include(cmake/Config.cmake)

cmake_path(SET Libb64_DIR "${PROJECT_DIR}/libb64")

add_library(Libb64 STATIC
	"${Libb64_DIR}/include/b64/cdecode.h"
	"${Libb64_DIR}/include/b64/cencode.h"
	"${Libb64_DIR}/include/b64/decode.h"
	"${Libb64_DIR}/include/b64/encode.h"

	${Libb64_DIR}/src/cdecode.c
	${Libb64_DIR}/src/cencode.c
)

target_include_directories(Libb64 PUBLIC ${Libb64_DIR}/include)
target_compile_definitions(Libb64 PUBLIC BUFFERSIZE=16777216)
