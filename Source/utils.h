#include "parser.h"

#ifndef __UTILSOPENGLHW3__
#define __UTILSOPENGLHW3__
using namespace parser;








parser::Vec3f Vec3fminus(const parser::Vec3f &v1,const  parser::Vec3f &v2){
    parser::Vec3f res;
    res.x = v1.x-v2.x;
    res.y = v1.y-v2.y;
    res.z = v1.z-v2.z;
    return res;
}

Vec3f Vec3fSum(const Vec3f &v1,const  Vec3f &v2){
    Vec3f res;
    res.x = v1.x+v2.x;
    res.y = v1.y+v2.y;
    res.z = v1.z + v2.z; 
    return res;
}

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
Vec3f cross(const Vec3f &v1,const Vec3f &v2)
{
    float x = v1.y*v2.z - v1.z*v2.y;
    float y = v1.z*v2.x - v1.x*v2.z;
    float z = v1.x*v2.y - v1.y*v2.x;
    Vec3f result;
    result.x = x; result.y = y; result.z = z;
    return result;
}
float* normal(Vec3f v0, Vec3f v1, Vec3f v2){
    //
    float * res = new float[3];
    Vec3f norm = normalize(cross(Vec3fminus(v1,v0),Vec3fminus(v2,v0)));
    res[0]=norm.x;
    res[1]=norm.y;
    res[2]=norm.z;
    return res;
}

float * actualNormal(const Scene &scene){
    
}

#endif