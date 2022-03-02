/*
 *  This file is part of RawTherapee.
 *
 *  Copyright (c) 2004-2010 Gabor Horvath <hgabor@rawtherapee.com>
 *
 *  RawTherapee is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  RawTherapee is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with RawTherapee.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "improcfun.h"

#include "imagefloat.h"
#include "rt_math.h"
// #include "procparams.h"

//#define PROFILE

#ifdef PROFILE
#   include <iostream>
#endif


namespace rtengine
{


void ImProcFunctions::resizeWidth (Imagefloat* src, Imagefloat* dst, float dScale)
{
#ifdef PROFILE
    time_t t1 = clock();
#endif

    Lanczos (src, dst, dScale, 1.0f);

#ifdef PROFILE
    time_t t2 = clock();
    std::cout << "Resize: " << params->resize.method << ": "
              << (float) (t2 - t1) / CLOCKS_PER_SEC << std::endl;
#endif
}


void ImProcFunctions::resizeHeight (Imagefloat* src, Imagefloat* dst, float dScale)
{
#ifdef PROFILE
    time_t t1 = clock();
#endif

    float hScale = dScale > 1. ? 1. / dScale : dScale;
    int end = int(src->getHeight() * hScale);

    // Nearest neighbour algorithm
#ifdef _OPENMP
#pragma omp parallel for if (multiThread)
#endif

    for (int i = 0; i < end; i++) {
      int sy = i / hScale;
      sy = LIM (sy, 0, src->getHeight() - 1);

      for (int j = 0; j < dst->getWidth(); j++) {
        int sx = LIM (j, 0, src->getWidth() - 1);
        dst->r (i, j) = src->r (sy, sx);
        dst->g (i, j) = src->g (sy, sx);
        dst->b (i, j) = src->b (sy, sx);
      }
    }

    for (int i = end; i < dst->getHeight(); i++) {
      for (int j = 0; j < dst->getWidth(); j++) {
        dst->r (i, j) = 0;
        dst->g (i, j) = 0;
        dst->b (i, j) = 0;
      }
    }


#ifdef PROFILE
    time_t t2 = clock();
    std::cout << "Resize: " << params->resize.method << ": "
              << (float) (t2 - t1) / CLOCKS_PER_SEC << std::endl;
#endif
}

}
