#include "Scene.hpp"

#include "imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Camera.hpp"

Scene* Scene::voxel_cone_trace()
{
    auto scene = new VoxelScene();
    
    return scene;
}

void window_input(GLFWwindow *window, Camera& camera,v3f& target, f32 dt)
{
    const auto get_key = [&](auto key) -> bool {
        return glfwGetKey(window, key) == GLFW_PRESS;
    };
    
    constexpr f32 cam_speed = 50.0f;
    
    if(get_key(GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);
    if(get_key(GLFW_KEY_W))
        target += camera.GetForward() * dt * cam_speed;
    if(get_key(GLFW_KEY_S))
        target += camera.GetForward() * dt * -cam_speed;
    if(get_key(GLFW_KEY_A))
        target += camera.GetRight() * dt * cam_speed;
    if(get_key(GLFW_KEY_D))
        target += camera.GetRight() * dt * -cam_speed;
    if(get_key(GLFW_KEY_Q))
        target += camera.GetUp() * dt * cam_speed;
    if(get_key(GLFW_KEY_E))
        target += camera.GetUp() * dt * -cam_speed;
}

void VoxelScene::run(RenderState& gl_state)
{
    Camera camera(glm::radians(45.0f),(f32) gl_state.width,(f32) gl_state.height, 0.1f, 1000.0f);
    camera.MakeProjection();
    
    Shader* shader = new Shader("base", {"voxel_light.vs", "voxel_light.fs"});
    Shader voxel_pass_shader("voxel_pass", {"voxel.vs", "voxel.gs", "voxel.fs"});
    Shader unlit_shader("unlit", { "base.vs", "flat.fs" });
    
    const int vox_size = 512/2;
    Texture3D voxel_map(vox_size,vox_size,vox_size);
    
    MultiMesh mesh("./assets/sponza.obj");
    StaticMesh sun(ASSETS_PATH + std::string("sun.obj"));
    
    Texture2D texture("./assets/UVgrid.png");
    texture.set_texture_slot(0);
    
    struct VoxelUniforms
    {
        v3f light_dir = glm::normalize(v3f(0.1f,1.0f,0.1f));
    } uVoxel;
    
    
    auto unlit_uniforms = unlit_shader.get_uniform_variables();
    auto voxel_uniforms = shader->get_uniform_variables();
    
    shader->bind();
    shader->set_int("uTexture1", 0);
    
    texture.bind(gl_state);
    
    m44 m(1.0f);
    
    v3f cam_target = {0,0,0};
    
    auto scene_size = (mesh.bounding_box.max - mesh.bounding_box.min);
    auto scene_scale = glm::vec3(1.0)/(scene_size);
    m = glm::translate(m, glm::vec3(0,-scene_size.y*0.3/8.0,0));
    scene_scale *= 2.0; // todo - center model
    m = glm::scale(m, glm::vec3(0.3));
    
    m44 scene_voxel_transform = m44(1.0);
    scene_voxel_transform = 
        glm::scale(scene_voxel_transform, scene_scale); // scale to -1 : 1
    scene_voxel_transform = scene_voxel_transform *
        glm::translate(m44(1.0), glm::vec3(0,-scene_size.y/2.50,0)); // move to center
    
    
    v3f spherical = {0.5,0.5,125};
    
    auto sphere_to_cart = [](const v3f r) -> v3f
    {
        return v3f{
            r.z * std::sinf(r.y) * std::cosf(r.x),
            r.z * std::cosf(r.y),
            r.z * std::sinf(r.y) * std::sinf(r.x),
        };
    };
    
    
    
    camera.position = cam_target + sphere_to_cart(spherical);
    camera.LookAt(cam_target);
    
    auto voxel_pass = [&]() {
        
        auto vp_time = glfwGetTime();
        //std::cout << "Begin voxel pass" << std::endl;
        
        glViewport(0, 0, voxel_map.width, voxel_map.height);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        
        camera.width = (f32) gl_state.width;
        camera.height = (f32) gl_state.height;
        camera.MakeProjection();
        
        
        auto ortho = glm::ortho(-voxel_map.width*0.5f, voxel_map.width*0.5f, -voxel_map.width*0.5f, voxel_map.width*0.5f, voxel_map.width*0.0000005f, voxel_map.width*.5f);//* camera.view;
        
        camera.position = cam_target + sphere_to_cart(spherical);
        camera.LookAt(cam_target);
        
        voxel_pass_shader.bind();
        
        voxel_map.set_texture_slot(1);
        voxel_map.bind(gl_state);
        
        voxel_pass_shader.set_int("uTexture1", 0);
        voxel_pass_shader.set_int("uVoxelMap", 1);
        
        voxel_pass_shader.set_vec3("light_dir", uVoxel.light_dir);
        
        
        glBindImageTexture(1, voxel_map.texture, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);
        
        voxel_map.clear();
        voxel_map.set_texture_slot(1);
        voxel_map.bind(gl_state);
        
        voxel_pass_shader.set_mat4("uM", scene_voxel_transform); 
        voxel_pass_shader.set_mat4("uV", camera.view); 
        voxel_pass_shader.set_mat4("uVP", ortho); 
        
        
        for (auto& submesh : mesh.meshes)
        {
            
            if (submesh->textures.diffuse)
            {
                submesh->textures.diffuse->set_texture_slot(0);
                submesh->textures.diffuse->bind(gl_state);
            }
            else
            {
                texture.bind(gl_state);
            }
            
            
            submesh->draw(gl_state);
        }
        
        voxel_map.unbind(gl_state);
        
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT); 
        
        voxel_map.generate_mipmaps();
        
        
        auto vp_end_time = glfwGetTime();
        //std::cout << "End voxel pass: (" << vp_end_time - vp_time << ")" << std::endl;
        
        glViewport(0, 0, gl_state.width, gl_state.height);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        
        
        
        shader->bind();
    };
    voxel_pass();
    
    auto time = (f32)glfwGetTime();
    auto last_time = time;
    
    while(!glfwWindowShouldClose(gl_state.window))
    {
        ////////////////////////////////
        time = (f32)glfwGetTime();
        f32 dt = time - last_time;
        ////////////////////////////////
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        ////////////////////////////////
        
        ImGui::Begin("Voxel Settings");
        
        ImGui::Text("Fps:: %f", ImGui::GetIO().Framerate);
        
        if(ImGui::SliderFloat3("light dir", glm::value_ptr(uVoxel.light_dir), -1.0, 1.0f))
        {
            uVoxel.light_dir = glm::normalize(uVoxel.light_dir);
        }   
        
        static bool dynamic_lighting = true;
        
        ImGui::Checkbox("Dynamic Lighting", &dynamic_lighting);
        
        ImGui::Separator();
        ImGui::Text("Voxel Light Uniforms");
        if (ImGui::Button("Reload"))
        {
            delete shader; 
            shader = new Shader("base", {"voxel_light.vs", "voxel_light.fs"});
            voxel_uniforms = shader->get_uniform_variables();
        }
        
        shader->uniform_edit(voxel_uniforms);
        
        ImGui::Separator();
        ImGui::Text("Unlit Uniforms");
        unlit_shader.uniform_edit(unlit_uniforms);
        
        
        ImGui::End();
        
        ////////////////////////////////
        
        window_input(gl_state.window, camera, cam_target, dt);
        
        camera.width = (f32) gl_state.width;
        camera.height = (f32) gl_state.height;
        camera.MakeProjection();
        
        auto l_state = glfwGetMouseButton(gl_state.window, GLFW_MOUSE_BUTTON_LEFT);
        
        if (l_state == GLFW_PRESS)
        {
            spherical.x += gl_state.rmx/100.f;
            spherical.y += gl_state.rmy/100.f;
        }
        
        spherical.z -= 10 * gl_state.scroll.y;
        
        
        gl_state.scroll = {};
        gl_state.rmx = 0.0f;
        gl_state.rmy = 0.0f;
        
        
        gl_state.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Render here
        
        if (dynamic_lighting)
            voxel_pass();
        
        gl_state.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        camera.position = cam_target + sphere_to_cart(spherical);
        camera.LookAt(cam_target);
        
        //m = glm::rotate(m, dt*8.f, v3f{ 0,0,1 });
        
        shader->bind();
        shader->set_int("uTexture1", 0);
        voxel_map.set_texture_slot(1);
        shader->set_int("uVoxelMap", 1);
        voxel_map.bind(gl_state);
        
        shader->set_mat4("uM", m); 
        shader->set_mat4("uV", camera.view); 
        shader->set_mat4("uVP", camera.ViewProjection()); 
        shader->set_vec3("uCamPos", camera.position); 
        shader->set_mat4("uSceneScale", scene_voxel_transform); 
        
        shader->set_vec3("light_dir", uVoxel.light_dir);
        shader->set_uniforms(voxel_uniforms);
#if 1       
        for (auto& submesh : mesh.meshes)
        {
            if (submesh->textures.diffuse)
            {
                submesh->textures.diffuse->set_texture_slot(0);
                submesh->textures.diffuse->bind(gl_state);
            }
            else
            {
                texture.bind(gl_state);
            }
            
            
            
            submesh->draw(gl_state);
        }
#else
        mesh.draw(gl_state);
#endif
        
        unlit_shader.bind();
        
        m44 sun_mat = glm::transpose(glm::lookAt({ 0,0,0 }, uVoxel.light_dir, { 0,1,0 }));
        
        
        unlit_shader.set_mat4("uM", sun_mat);
        unlit_shader.set_mat4("uV", camera.view);
        unlit_shader.set_mat4("uVP", camera.ViewProjection());
        unlit_shader.set_uniforms(unlit_uniforms);
        
        sun.draw(gl_state);
        ////////////////////////////////
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        
        glfwSwapBuffers(gl_state.window);
        glfwPollEvents();    
        ////////////////////////////////
        last_time = time;
    }
}