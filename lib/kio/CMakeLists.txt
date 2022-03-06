# ======---------------------------------------------------------------====== #
#                                                                             #
#  Copyright 2022 Evan Cox <evanacox00@gmail.com>. All rights reserved.       #
#                                                                             #
#  Use of this source code is governed by a BSD-style license that can be     #
#  found in the LICENSE.txt file at the root of this project, or at the       #
#  following link: https://opensource.org/licenses/BSD-3-Clause               #
#                                                                             #
# ======---------------------------------------------------------------====== #

add_library(indigo_kio lib/gpio.cc lib/uart.cc lib/sleep.cc)
target_include_directories(indigo_kio PUBLIC ./include)
indigo_configure_target(indigo_kio)