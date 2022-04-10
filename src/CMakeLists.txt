# ======---------------------------------------------------------------====== #
#                                                                             #
#  Copyright 2022 Evan Cox <evanacox00@gmail.com>. All rights reserved.       #
#                                                                             #
#  Use of this source code is governed by a BSD-style license that can be     #
#  found in the LICENSE.txt file at the root of this project, or at the       #
#  following link: https://opensource.org/licenses/BSD-3-Clause               #
#                                                                             #
# ======---------------------------------------------------------------====== #

add_executable(indigo
        ./native/halt.S
        ./stubs/frt.cc
        ./boot.S
        ./entry.cc
        ./device_tree.cc)
indigo_configure_target(indigo)
indigo_handle_linker_script(indigo ./layout.ld)
indigo_create_image(indigo)
target_link_libraries(indigo PRIVATE indigo_kio indigo_klog)
indigo_copy_image()
