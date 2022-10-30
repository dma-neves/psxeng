#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <sys/types.h>
#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>

typedef struct TimParam
{
    RECT prect;
    RECT crect;
    int mode;

    int uoffs;
    int voffs;

} TimParam;

#endif