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

# UserConfig defines configuration to be used by other cmake scripts

cmake_minimum_required(VERSION 3.16)

# sets the type of build, expected values: release | debug
# however it affects only dependencies
set(USER_BUILD_TYPE release)

# force system qt6
set(QT6_FORCE_SYSTEM OFF)

# forces cmake to rebuild qt6, conflicts with QT6_FORCE_SYSTEM(takes priority)
set(QT6_FORCE_BUILD OFF)

# force system libressl
set(SSL_FORCE_SYSTEM OFF)

# forces cmake to rebuild libressl, conflicts with SSL_FORCE_SYSTEM(takes priority)
set(SSL_FORCE_BUILD OFF)
