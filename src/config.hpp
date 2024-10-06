#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#pragma once

#include <limits>
#include <assert.h>

#define STDOUT_ENABLED true
#define STDOUT_LOCAL_OP false

#define VERIFY_PERM_FREQ true
#define VERIFY_LBS_UBS true

#define fitness_t uint
#define MAX_FITNESS __UINT32_MAX__

#define LB_PENALTY (MAX_FITNESS >> 12)
#define UB_PENALTY (MAX_FITNESS >> 13)

#endif