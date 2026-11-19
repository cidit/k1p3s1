#pragma once

#define ROBOT_3A

#ifdef ROBOT_3A
#include "robots/calib_3a_wheels.h"
#else
#include "robots/calib_3b_wheels.h"
#endif