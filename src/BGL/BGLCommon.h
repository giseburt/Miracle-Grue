//
//  BGLCommon.h
//  Part of the Belfry Geometry Library
//
//  Created by GM on 10/13/10.
//  Copyright 2010 Belfry Software. All rights reserved.
//

#ifndef BGL_COMMON_H
#define BGL_COMMON_H

#include "config.h"

namespace BGL {


extern Scalar EPSILON;
extern Scalar CLOSEENOUGH;

typedef enum {
    USED = 0,
    OUTSIDE = 1,
    INSIDE = 2,
    SHARED = 4,
    UNSHARED = 8
} Relation;


}
#endif

