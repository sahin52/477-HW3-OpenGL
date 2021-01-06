#include "parser.h"
#include <vector>
using namespace std;
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
float* vec4(const Vec3f &v3,const float &f){
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
float* normal(const Vec3f &v0,const  Vec3f &v1,const Vec3f &v2){
    //
    float * res = new float[3];
    Vec3f norm = normalize(cross(Vec3fminus(v1,v0),Vec3fminus(v2,v0)));
    res[0]=norm.x;
    res[1]=norm.y;
    res[2]=norm.z;
    return res;
}

float * average(const vector<float*> &normals){
    float * res = new float[3];
    res[0] = 0.;
    res[1] = 0.;
    res[2] = 0.;
    for(auto normal: normals){
        res[0] += normal[0];
        res[1] += normal[1];
        res[2] += normal[2];
    }
    res[0] = res[0]/(float)normals.size();
    res[1] = res[1]/(float)normals.size();
    res[2] = res[2]/(float)normals.size();
    return res;
}

float * actualNormal(const Scene &scene, int vertexId,const Mesh &mesh){
    //o meshin tum vertexIdlerine bakip bu id ile ayni id'ye sahip olan tum ucgenleri alip
    //bu ucgenlerin normallerini bulup ortalamasını almak lazım.
    std::vector<Face> faces;
    vector<int> ids;
    for (int i =0;i<mesh.faces.size();i++){
        auto face = mesh.faces[i];
        if(face.v0_id==vertexId){
            ids.push_back(0);
            faces.push_back(face);
        }else if(face.v1_id==vertexId){
            ids.push_back(1);
            faces.push_back(face);
        }else if(face.v2_id==vertexId){
            ids.push_back(2);
            faces.push_back(face);
        }
    }
    vector<float*> normals;
    for(int i =0;i<faces.size();i++){
        auto face = faces[i];
        auto temp = normal(scene.vertex_data[face.v0_id-1],scene.vertex_data[face.v1_id-1],scene.vertex_data[face.v2_id-1]);
        normals.push_back(temp);
        //delete[] temp;
        //normals.push_back();
    }
    return average(normals);
}

#endif