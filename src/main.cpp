
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Core.hpp"
#include "RenderState.hpp"
#include "Texture2D.hpp"
#include "Texture3D.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Screen.hpp"
#include "Camera.hpp"
#include "Scene.hpp"

#define ZAML_IMPLEMENTATION
#include "Zaml.hpp"

void GLAPIENTRY
MessageCallback( GLenum source,
                GLenum type,
                GLuint id,
                GLenum severity,
                GLsizei length,
                const GLchar* message,
                const void* userParam )
{
    if (severity <= GL_DEBUG_SEVERITY_MEDIUM) return;
    
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}



int main()
{
    std::cout << "Running in " << std::filesystem::current_path() << std::endl;
    
    if (!std::filesystem::exists("./assets"))
    {
        std::cerr << "Warning! asset dir not found!" << std::endl;
    }
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    std::string config_path = ASSETS_PATH;
    config_path += "data/config.zaml";
    
    Zaml::Node config = Zaml::LoadFile(config_path);
    
    RenderState gl_state;
    gl_state.create_window(config["window"]["width"].as<int>(), config["window"]["height"].as<int>());
    gl_state.set_position(config["window"]["x"].as<int>(), config["window"]["y"].as<int>());
    
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback( MessageCallback, 0 );
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    ImGui::StyleColorsDark();
    
    ImGui_ImplGlfw_InitForOpenGL(gl_state.window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    
    
    gl_state.set_clear_color({0.2f, 0.3f, 0.3f, 1.0f});
    
    auto scene = Scene::voxel_cone_trace();
    scene->run(gl_state);
    
    auto [win_x, win_y] = gl_state.get_postion();
    
    config["window"]["width"] = gl_state.width;
    config["window"]["height"] = gl_state.height;
    config["window"]["x"] = win_x;
    config["window"]["y"] = win_y;
    
    Zaml::SaveFile(config, config_path);
    
    glfwTerminate();
}