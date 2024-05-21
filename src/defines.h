#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef float f32;
typedef double f64;

#define Megabytes(x) (x * 1024 * 1024)
#define Min(a,b) (((a)<(b))?(a):(b))
#define Max(a,b) (((a)>(b))?(a):(b))

#define FRAME_COUNT 3
#define TIMESTEP_MS 0
