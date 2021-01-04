#include <iostream>
#include "parser.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "utils.h"
//////-------- Global Variables -------/////////

GLuint gpuVertexBuffer;
GLuint gpuNormalBuffer;
GLuint gpuIndexBuffer;

// Sample usage for reading an XML scene file
parser::Scene scene;
static GLFWwindow* window = NULL;

static void errorCallback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
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

    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    for(int i=0;i<scene.meshes.size();i++){
        //glColor3f(scene.materials[scene.meshes[i].material_id-1].ambient.x,scene.materials[scene.meshes[i].material_id-1].ambient.y,scene.materials[scene.meshes[i].material_id-1].ambient.z);//TEMP
        GLfloat ambColor [ 4 ] = {scene.materials[scene.meshes[i].material_id-1].ambient.x,scene.materials[scene.meshes[i].material_id-1].ambient.y, scene.materials[scene.meshes[i].material_id-1].ambient.z, 1.0 } ;
        GLfloat diffColor [ 4 ] = {scene.materials[scene.meshes[i].material_id-1].diffuse.x, scene.materials[scene.meshes[i].material_id-1].diffuse.x, scene.materials[scene.meshes[i].material_id-1].diffuse.x, 1.0 } ;
        GLfloat specColor [ 4 ] = {scene.materials[scene.meshes[i].material_id-1].specular.x, scene.materials[scene.meshes[i].material_id-1].specular.y, scene.materials[scene.meshes[i].material_id-1].specular.z, 1.0 } ;
        GLfloat specExp [ 1 ] = {scene.materials[scene.meshes[i].material_id-1].phong_exponent};
        glMaterialfv ( GL_FRONT , GL_AMBIENT , ambColor ) ;
        glMaterialfv ( GL_FRONT , GL_DIFFUSE , diffColor ) ;
        glMaterialfv ( GL_FRONT , GL_SPECULAR , specColor ) ;
        glMaterialfv ( GL_FRONT , GL_SHININESS , specExp ) ;

        for(auto face: scene.meshes[i].faces){
            glBegin(GL_TRIANGLES);
                glVertex3f(scene.vertex_data[face.v0_id-1].x,scene.vertex_data[face.v0_id-1].y,scene.vertex_data[face.v0_id-1].z);
                glVertex3f(scene.vertex_data[face.v1_id-1].x,scene.vertex_data[face.v1_id-1].y,scene.vertex_data[face.v1_id-1].z);
                glVertex3f(scene.vertex_data[face.v2_id-1].x,scene.vertex_data[face.v2_id-1].y,scene.vertex_data[face.v2_id-1].z);
            glEnd();
        }  
         
    }

        
}



int main(int argc, char* argv[]) {
    std::cout<<"basladi\n";
    scene.loadFromXml(argv[1]);

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



    while(!glfwWindowShouldClose(window)) {
        //MAIN LOOP
        //glfwWaitEvents(); // not quite sure what this is for
        renderScene();

        //showFPS(window)

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

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
