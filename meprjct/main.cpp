// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <iostream> 
//#include "sqlite3/sqlite3.h"
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
using namespace std;
// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}


// Main code
int main(int, char**)
{
   /* sqlite3* db;
    char* zErrMsg = 0;
    int rc;

    rc = sqlite3_open("test.db", &db);
    std::cout << rc;

    sqlite3_close(DB);*/
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "DailyRoutine", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    bool show_demo_window = false;
    bool show_another_window = false;
    bool show_Sun = false;
    bool show_Mon = false;
    bool show_Tue = false;
    bool show_Wed = false;
    bool show_Thur = false;
    bool show_Fri = false;
    bool show_Sat = false;
    bool add_cal = false;
    bool add_ex = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    
    // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("DailyRoutine");
            if (ImGui::BeginTable("table1", 2)) {
                ImGui::TableNextColumn();
                ImGui::Checkbox("SUNDAY", &show_Sun);
                ImGui::Checkbox("MONDAY", &show_Mon);
                ImGui::Checkbox("TUESDAY", &show_Tue);
                ImGui::Checkbox("WEDNESDAY", &show_Wed);
                ImGui::Checkbox("THURSDAY", &show_Thur);
                ImGui::Checkbox("FRIDAY", &show_Fri);
                ImGui::Checkbox("SATURDAY", &show_Sat);
                ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
                if (ImGui::Button("Delete.."))
                    ImGui::OpenPopup("Delete?");
                if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!");
                    ImGui::Separator();

                    //static int unused_i = 0;
                    //ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

                    static bool dont_ask_me_next_time = false;
                    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                    ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
                    ImGui::PopStyleVar();

                    if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                    ImGui::SetItemDefaultFocus();
                    ImGui::SameLine();
                    if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                    ImGui::EndPopup();
                }
                ImGui::TableNextColumn();
                ImGui::Text("Calories");
                static int clicked = 0;
                if (ImGui::Button("+"))
                    clicked++;
                if (clicked & 1)
                {   ImGui::Begin("+", &add_cal);
                    ImGui::Text("hello");
                    if (ImGui::Button("Close Me"))
                        clicked = 0;
                    ImGui::End();
                    
                }
                //выводится список с блюдами за день, на + можно добавить в новом окне
                ImGui::Text("Expenses");
                static int click = 0;
                if (ImGui::Button("++"))
                    click++;
                if (click & 1)
                {
                    ImGui::Begin("++", &add_ex);
                    ImGui::Text("hello");
                    if (ImGui::Button("Close Me"))
                        click = 0;
                    ImGui::End();
                }
                    
                //выводится список с тратами за день, на + можно добавить в новом окне
                ImGui::EndTable();
            }
            //ImGui::PushTabStop(false);
            //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            //ImGui::ColorEdit3(ImGuiColorEditFlags_NoInputs, &clear_color); // Edit 3 floats representing a color
            /*if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);*/

            //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }
        if (show_Sun) {
            ImGui::Begin("SUNDAY", &show_Sun);
            static char str1[128] = "";
            ImGui::InputTextWithHint("deal", "enter text here", str1, IM_ARRAYSIZE(str1));
            if (ImGui::Button("Close Me"))
                show_Sun = false;
            ImGui::End();
        }
        if (show_Mon) {
            ImGui::Begin("MONDAY", &show_Mon);
            static char str2[128] = "";
            ImGui::InputTextWithHint("deal", "enter text here", str2, IM_ARRAYSIZE(str2));
            if (ImGui::Button("Close Me"))
                show_Mon = false;
            ImGui::End();
        }
        if (show_Tue) {
            ImGui::Begin("TUESDAY", &show_Tue);
            static char str3[128] = "";
            ImGui::InputTextWithHint("deal", "enter text here", str3, IM_ARRAYSIZE(str3));
            if (ImGui::Button("Close Me"))
                show_Tue = false;
            ImGui::End();
        }
        if (show_Wed) {
            ImGui::Begin("WEDNESDAY", &show_Wed);
            static char str4[128] = "";
            ImGui::InputTextWithHint("deal", "enter text here", str4, IM_ARRAYSIZE(str4));
            if (ImGui::Button("Close Me"))
                show_Wed = false;
            ImGui::End();
        }
        if (show_Thur) {
            ImGui::Begin("THURSDAY", &show_Thur);
            static char str5[128] = "";
            ImGui::InputTextWithHint("deal", "enter text here", str5, IM_ARRAYSIZE(str5));
            if (ImGui::Button("Close Me"))
                show_Thur = false;
            ImGui::End();
        }
        if (show_Fri) {
            ImGui::Begin("FRIDAY", &show_Fri);
            static char str6[128] = "";
            ImGui::InputTextWithHint("deal", "enter text here", str6, IM_ARRAYSIZE(str6));
            if (ImGui::Button("Close Me"))
                show_Fri = false;
            ImGui::End();
        }
        if (show_Sat) {
            ImGui::Begin("SATURDAY", &show_Tue);
            static char str7[128] = "";
            ImGui::InputTextWithHint("deal", "enter text here", str7, IM_ARRAYSIZE(str7));
            if (ImGui::Button("Close Me"))
                show_Tue = false;
            ImGui::End();
        }
       

        // Always center this window when appearing
        //ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        //ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        
        // 3. Show another simple window.
        /*if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
        */
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
