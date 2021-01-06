#include <iostream>
#include "parser.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "utils.h"
#include <vector>
//////-------- Global Variables -------/////////
using namespace std;
GLuint gpuVertexBuffer;
GLuint gpuNormalBuffer;
GLuint gpuIndexBuffer;
float **** actualNormals;
// Sample usage for reading an XML scene file
parser::Scene scene;
static GLFWwindow* window = NULL;

static void errorCallback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

double lastTime;
int nbFrames;
void showFPS(GLFWwindow *pWindow,char * name)
{
    // Measure speed
     double currentTime = glfwGetTime();
     double delta = currentTime - lastTime;
	 char ss[500] = {};
     nbFrames++;
     if ( delta >= 1.0 ){ // If last cout was more than 1 sec ago
         //cout << 1000.0/double(nbFrames) << endl;

         double fps = ((double)(nbFrames)) / delta;

         sprintf(ss, "CENG 477. %s %lf FPS",name,fps);

         glfwSetWindowTitle(pWindow, ss);

         nbFrames = 0;
         lastTime = currentTime;
     }
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
void renderScene(){
    GLint widthh;
    GLfloat temp;
    int width;
    int height;
    //glEnable(GL_DEPTH_TEST);
    //glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    for(int i=0;i<scene.meshes.size();i++){
        glPushMatrix();
        
        for(int j=scene.meshes[i].transformations.size()-1;j>=0;j--){
            auto transformations = scene.meshes[i].transformations;
            if(transformations[j].transformation_type=="Translation"){
                auto translation = scene.translations[transformations[j].id-1];
                glTranslatef(GLfloat(translation.x),GLfloat(translation.y),GLfloat(translation.z));
            }
            else if(transformations[j].transformation_type=="Rotation"){
                auto rotation  = scene.rotations[transformations[j].id-1];
                glRotatef(GLfloat(rotation.x),GLfloat(rotation.y),GLfloat(rotation.z),GLfloat(rotation.w));
            }else{
                auto scaling =  scene.scalings[transformations[j].id-1];
                glScalef(GLfloat(scaling.x),GLfloat(scaling.y),GLfloat(scaling.z));
            }
        }

        //glColor3f(scene.materials[scene.meshes[i].material_id-1].ambient.x,scene.materials[scene.meshes[i].material_id-1].ambient.y,scene.materials[scene.meshes[i].material_id-1].ambient.z);//TEMP
        GLfloat ambColor [ 4 ] = {scene.materials[scene.meshes[i].material_id-1].ambient.x,scene.materials[scene.meshes[i].material_id-1].ambient.y, scene.materials[scene.meshes[i].material_id-1].ambient.z, 1.0 } ;
        GLfloat diffColor [ 4 ] = {scene.materials[scene.meshes[i].material_id-1].diffuse.x, scene.materials[scene.meshes[i].material_id-1].diffuse.y, scene.materials[scene.meshes[i].material_id-1].diffuse.z, 1.0 } ;
        GLfloat specColor [ 4 ] = {scene.materials[scene.meshes[i].material_id-1].specular.x, scene.materials[scene.meshes[i].material_id-1].specular.y, scene.materials[scene.meshes[i].material_id-1].specular.z, 1.0 } ;
        GLfloat specExp [ 1 ] = {scene.materials[scene.meshes[i].material_id-1].phong_exponent};
        glMaterialfv ( GL_FRONT , GL_AMBIENT , ambColor ) ;
        glMaterialfv ( GL_FRONT , GL_DIFFUSE , diffColor ) ;
        glMaterialfv ( GL_FRONT , GL_SPECULAR , specColor ) ;
        glMaterialfv ( GL_FRONT , GL_SHININESS , specExp ) ;
        if(scene.meshes[i].mesh_type=="Solid"){
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }else if(scene.meshes[i].mesh_type=="Wireframe"){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }else{
            printf("Error! Unknown type of mesh");
        }
        for(int j=0;j<scene.meshes[i].faces.size();j++){
            auto face = scene.meshes[i].faces[j];
            //glShadeModel(GL_SMOOTH);
            glBegin(GL_TRIANGLES);
                
                //glNormal3fv(normal(scene.vertex_data[face.v0_id-1],scene.vertex_data[face.v1_id-1],scene.vertex_data[face.v2_id-1]));
                
                glNormal3fv(actualNormals[i][j][0]);
                glVertex3f(scene.vertex_data[face.v0_id-1].x,scene.vertex_data[face.v0_id-1].y,scene.vertex_data[face.v0_id-1].z);
                
                //glNormal3fv(normal(scene.vertex_data[face.v1_id-1],scene.vertex_data[face.v2_id-1],scene.vertex_data[face.v0_id-1]));
                glNormal3fv(actualNormals[i][j][1]);
                glVertex3f(scene.vertex_data[face.v1_id-1].x,scene.vertex_data[face.v1_id-1].y,scene.vertex_data[face.v1_id-1].z);
                
                //glNormal3fv(normal(scene.vertex_data[face.v2_id-1],scene.vertex_data[face.v0_id-1],scene.vertex_data[face.v1_id-1]));
                
                glNormal3fv(actualNormals[i][j][2]);
                glVertex3f(scene.vertex_data[face.v2_id-1].x,scene.vertex_data[face.v2_id-1].y,scene.vertex_data[face.v2_id-1].z);
            glEnd();
        }  
        
        glPopMatrix();

    }

        
}

float **** getVertexNormalData(const Scene &scene){
    //auto vertices = scene.vertex_data;
    //float*
    float** vertexNormalleri = new float*[scene.vertex_data.size()];
    for(int i=0;i<scene.vertex_data.size();i++){
        vertexNormalleri[i] = nullptr;
    }

    float **** res = new float***[scene.meshes.size()];
    for(int i=0;i<scene.meshes.size();i++){
        auto mesh = scene.meshes[i];
        res[i] = new float**[mesh.faces.size()];
        for(int j=0;j<mesh.faces.size();j++){
            auto face = mesh.faces[j];
            res[i][j] = new float*[3];
            if(vertexNormalleri[face.v0_id-1]==nullptr)
                res[i][j][0]=actualNormal(scene,face.v0_id,mesh,vertexNormalleri);
            else    
                res[i][j][0] = vertexNormalleri[face.v0_id-1];
            if(vertexNormalleri[face.v1_id-1]==nullptr)
                res[i][j][1]=actualNormal(scene,face.v1_id,mesh,vertexNormalleri);
            else 
                res[i][j][1] = vertexNormalleri[face.v1_id-1];
            if(vertexNormalleri[face.v2_id-1]==nullptr)
                res[i][j][2]=actualNormal(scene,face.v2_id,mesh,vertexNormalleri);
            else
                res[i][j][2] = vertexNormalleri[face.v2_id-1];
        }
    }
    return res;
}


int main(int argc, char* argv[]) {
    std::cout<<"Starting\n";
    scene.loadFromXml(argv[1]);
    cout << "Pre-calculating vertex normal data,\nThis can take up to 10 seconds!\nPlease wait!!!!!\n";
    actualNormals = getVertexNormalData(scene);

    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    window = glfwCreateWindow(scene.camera.image_width, scene.camera.image_height, "CENG477 - HW3", NULL, NULL);
    if (!window) {
        printf("Failed to open GLFW window.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, keyCallback);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_INDEX_ARRAY);

    auto gaze = normalize(scene.camera.gaze);

    glMatrixMode(GL_PROJECTION);
    glFrustum(scene.camera.near_plane.x,scene.camera.near_plane.y,scene.camera.near_plane.z,scene.camera.near_plane.w,scene.camera.near_distance,scene.camera.far_distance);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(scene.camera.position.x,scene.camera.position.y,scene.camera.position.z,
        scene.camera.gaze.x*scene.camera.near_distance+scene.camera.position.x, scene.camera.gaze.y*scene.camera.near_distance+scene.camera.position.y,scene.camera.gaze.z*scene.camera.near_distance+scene.camera.position.z,
        scene.camera.up.x,scene.camera.up.y,scene.camera.up.z//TODO not sure
    );

    // gluLookAt(scene.camera.gaze.x,scene.camera.gaze.y,scene.camera.gaze.z,scene.camera.position.x,scene.camera.position.y,scene.camera.position.z,
    //                         scene.camera.up.x,scene.camera.up.y,scene.camera.up.z);//TODO 
    glEnable(GL_LIGHTING);
    if(scene.culling_enabled==1){
        glEnable(GL_CULL_FACE);
        if(scene.culling_face==1){
            glCullFace(GL_FRONT);
        }
        else{
            glCullFace(GL_BACK);
        }
    }
    else{
        glDisable(GL_CULL_FACE);
    }

    // glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    // glEnable(GL_COLOR_MATERIAL);
    //glScalef(0.5,0.5,0.5);
    
    //glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_AMBIENT, vec4(scene.ambient_light,1.0));
    auto lights = scene.point_lights;
    for (int i = 0; i < scene.point_lights.size(); i++) {
        glEnable(GL_LIGHT0 + i+1);
        GLfloat col[] = {lights[i].intensity.x, lights[i].intensity.y, lights[i].intensity.z, 1.0f};
        GLfloat pos[] = {lights[i].position.x, lights[i].position.y, lights[i].position.z, 1.0f};
        glLightfv(GL_LIGHT0 + i+1, GL_POSITION, pos);
//        glLightfv(GL_LIGHT0 + i+1, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0 + i+1, GL_DIFFUSE, col);
        glLightfv(GL_LIGHT0 + i+1, GL_SPECULAR, col);
    }

    glEnable(GL_DEPTH_TEST);
    // // glShadeModel(GL_FLAT);
    // auto meshVertexJointVertices = std::vector<std::vector<std::vector<int>>>();
    // for(auto mesh: scene.meshes){
    //     auto vertexJoints = std::vector<std::vector<int>>();

    //     for(auto face:mesh.faces){
    //         //auto jointFor;
    //         face.v0_id;
    //         face.v1_id;
    //         face.v2_id;
    //     }

    //     meshVertexJointVertices.push_back(vertexJoints);
    // }

    while(!glfwWindowShouldClose(window)) {
        //MAIN LOOP
        //glfwWaitEvents(); // not quite sure what this is for
        renderScene();

        showFPS(window,argv[1]);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDisable(GL_CULL_FACE);
    glfwDestroyWindow(window);
    glfwTerminate();
//     GLfloat ambColor [ 4 ] = {<ar>, <ag>, <ab>, 1 . 0 } ;
// GLfloat diffColor [ 4 ] = {<dr>, <dg>, <db>, 1 . 0 } ;
// GLfloat specColor [ 4 ] = {<sr>, <sg>, <sb>, 1 . 0 } ;
// GLfloat specExp [ 1 ] = {<specular exponent >};
// glMaterialfv ( GL_FRONT , GL_AMBIENT , ambColor ) ;
// glMaterialfv ( GL_FRONT , GL_DIFFUSE , diffColor ) ;
// glMaterialfv ( GL_FRONT , GL_SPECULAR , specColor ) ;
// glMaterialfv ( GL_FRONT , GL_SHININESS , specExp ) ;
    exit(EXIT_SUCCESS);

    return 0;
}
