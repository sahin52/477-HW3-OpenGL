#include <iostream>
#include "parser.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
        glColor3f(scene.materials[scene.meshes[i].material_id-1].ambient.x,scene.materials[scene.meshes[i].material_id-1].ambient.y,scene.materials[scene.meshes[i].material_id-1].ambient.z);//TEMP
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

    gluLookAt(scene.camera.gaze.x,scene.camera.gaze.y,scene.camera.gaze.z,scene.camera.position.x,scene.camera.position.y,scene.camera.position.z,
                            scene.camera.up.x,scene.camera.up.y,scene.camera.up.z);//TODO 
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    while(!glfwWindowShouldClose(window)) {
        //MAIN LOOP
        //glfwWaitEvents(); // not quite sure what this is for
        renderScene();


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);

    return 0;
}
