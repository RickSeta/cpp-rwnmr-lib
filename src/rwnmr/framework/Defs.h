#ifndef NMR_DEFS_H
#define NMR_DEFS_H

// include string stream manipulation functions
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <iomanip>
#include <random>
#include <vector>
#include <string>
#include <random>
#include <cmath>
#include <limits>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

using namespace std;
// include OpenCV core functions

// include OpenMP for multicore implementation
#include <omp.h>

// include other project libs
#include "../utils/Utils.h"
#include "../math/Math.h"
#include "../math/RNG/mRNG.h"
#include "../math/RNG/xorshift.h"
#include "../math/RNG/randomIndex.h"
#include "../math/Laplace/tikhonov.h"
#include "../math/Laplace/include/nmrinv_core.h"
#include "../configFiles/ConfigFiles.h"

using namespace std;

#define MPI_NMR_T2_SIZE 128
#define TWO_PI (6.2831853071795864769252867665590057683943)

#ifndef DIRECTION_ENUM
#define DIRECTION_ENUM
typedef enum Direction
{
    None = 0,
    North = 1,
    West = 2,
    South = 3,
    East = 4,
    Up = 5,
    Down = 6
} direction;
#endif

#define NORTH 1
#define WEST 2
#define SOUTH 3
#define EAST 4
#define UP 5
#define DOWN 6

#define LOG_XIRATE false
#define WALKER_INITIAL_ENERGY 1.0
#define WALKER_DEFAULT_RHO 0.0

#endif