#pragma once

#include "Core.hpp"
#include <glad/glad.h> 
#include <glfw/glfw3.h>
#include "imgui.h"

struct RenderState 
{
    GLFWwindow* window = nullptr;
    i32 width, height;
    inline static bool glad_loaded = false;
    f32 mx, my;
    f32 rmx, rmy;
    v2f scroll{ 0,0 };
    std::vector<std::string> files;

    void set_position(int x, int y)
    {
        glfwSetWindowPos(window, x, y);
    }

    std::array<int, 2> get_postion() const
    {
        int x, y;
        glfwGetWindowPos(window, &x, &y);
        return {x, y};
    }

    int create_window(int w, int h)
    {
        width = w;
        height = h;
        
        window = glfwCreateWindow(w, h, "Shader Final", NULL, NULL);
        if (window == NULL)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);
        
        if (!glad_loaded && !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            return -1;
        } 
        glad_loaded = true;
        
        glfwSetWindowUserPointer(window, this);
        
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int w, int h)
                                       {
                                           RenderState& self = *static_cast<RenderState*>(glfwGetWindowUserPointer(window));
                                           self.width = w;
                                           self.height = h;
                                           glViewport(0,0,w,h);
                                       });
        
        glfwSetCursorPosCallback(window, [](GLFWwindow* window, f64 x, f64 y){
                                     RenderState& self = *static_cast<RenderState*>(glfwGetWindowUserPointer(window));
                                     
                                     
                                     ImGuiIO& io = ImGui::GetIO();
                                     
                                     if (io.WantCaptureMouse)
                                         return;
                                     
                                     self.rmx = (f32)x - self.mx; 
                                     self.rmy = (f32)y - self.my; 
                                     self.mx = (f32)x;
                                     self.my = (f32)y;
                                     
                                 });
        
        glfwSetScrollCallback(window, [](auto win, auto x, auto y) {
            RenderState& self = *static_cast<RenderState*>(glfwGetWindowUserPointer(win));

            ImGuiIO& io = ImGui::GetIO();

            if (io.WantCaptureMouse)
                return;

            self.scroll = { x,y };

        });

        glfwSetDropCallback(window, [](auto win, int path_count, const char* paths[]) {
            RenderState& self = *static_cast<RenderState*>(glfwGetWindowUserPointer(win));

            for (int i = 0; i < path_count; i++)
                self.files.push_back(paths[i]);
        });

        glViewport(0, 0, 800, 600);
        
        glEnable(GL_DEPTH_TEST); 
        glEnable(GL_BLEND);		
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
        
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        
        
        return 0;
    }
    
    void clear(GLbitfield bit)
    {
        glClear(bit);
    }
    
    void set_clear_color(const v4f& color) { 
        clear_color = color; 
        glClearColor(color.r, color.g, color.b, color.a);
    }
    
    auto get_clear_color() { return clear_color; }
    private:
    v4f clear_color;
};