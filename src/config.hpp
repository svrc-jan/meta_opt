#pragma once

#include <limits>
#include <assert.h>

#define uint unsigned int

#define fitness_t uint
#define MAX_FITNESS __UINT32_MAX__

#define LB_PENALTY (MAX_FITNESS / 256U)
#define UB_PENALTY (MAX_FITNESS / 512U)