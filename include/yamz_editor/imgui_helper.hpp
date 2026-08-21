/**
 * @file imgui_helper.hpp
 * @author ddertgw (ddertgw@gmail.com)A
 * @brief Вспомогательные классы/функции для работы с ImGui
 * @date 2026-08-11
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#pragma once
#ifndef IMGUI_HELPER_HPP
#define IMGUI_HELPER_HPP

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

namespace yamz_editor {

class ImGuitor {
public:
    ImGuitor() 
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        io_ = &ImGui::GetIO();
        io_->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();     
    };

    void init(GLFWwindow* p_window) {
        p_window_ = p_window;
        ImGui_ImplGlfw_InitForOpenGL(p_window_, true);
        ImGui_ImplOpenGL3_Init();
    }

    void on_update(float (&v)[3], float (&v2)[3], float (&v3)[3], float (&v4)[3], float (&v5)[3], bool &v6) {
        int w, h;
        glfwGetWindowSize(p_window_, &w, &h);
        // Start the Dear ImGui frame
        io_->IniFilename = nullptr;
        io_->DisplaySize.x = static_cast<float>(w);
        io_->DisplaySize.y = static_cast<float>(h);
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Yammers");
    
        ImGui::SliderFloat3("scale", v, 0.f, 2.f);
        ImGui::SliderFloat3("rotate", v2, 0.f, 360.f);
        ImGui::SliderFloat3("translation", v3, -1.f, 1.f);
        ImGui::SliderFloat3("camera_pos", v4, -10.f, 10.f);
        ImGui::SliderFloat3("camera_rotation", v5, 0.f, 360.f);
        ImGui::Checkbox("Perspective camera", &v6);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    ~ImGuitor() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    };
private:
    GLFWwindow* p_window_ = nullptr;
    ImGuiIO *io_ = nullptr;
};

} // namepsace yamz_editor

#endif // IMGUI_HELPER_HPP