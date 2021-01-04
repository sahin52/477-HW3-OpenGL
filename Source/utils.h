#include "parser.h"

#ifndef __UTILSOPENGLHW3__
#define __UTILSOPENGLHW3__
using namespace parser;
Vec3f normalize(const Vec3f &v)
{
    Vec3f result;
    float uzunluk =  sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    result.x = v.x / uzunluk;
    result.y = v.y / uzunluk;
    result.z = v.z / uzunluk;
    return result;
}
float* vec4(Vec3f v3, float f){
    float * res = new float[4];
    res[0] = v3.x;
    res[1] = v3.y;
    res[2] = v3.z;
    res[3] = f;
    return res;
}



#endif