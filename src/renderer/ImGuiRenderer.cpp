#include "ImGuiRenderer.h"

void ImGuiRenderer::Init(Window &window)
{
    //imgui
    imguiF = std::make_unique<FrameBuffer>();
    imguiF->attachColor(window.GetWidth(), window.GetHeight());
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = "imgui.ini";
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // 允许浮动窗口在多视口中显示
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f; // 直角窗口
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f); // 暗色背景
    //ImGui::StyleColorsLight();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window.GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void ImGuiRenderer::Begin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 创建全屏 DockSpace
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);      // 覆盖工作区
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f); // 无圆角

    // 定义 DockSpace 窗口标志
    ImGuiWindowFlags dockSpaceFlags =
    ImGuiWindowFlags_MenuBar |               // 启用菜单栏 
    ImGuiWindowFlags_NoDocking |             // 禁止自身停靠
    ImGuiWindowFlags_NoTitleBar |            // 无标题栏
    ImGuiWindowFlags_NoCollapse |            // 不可折叠
    ImGuiWindowFlags_NoResize |              // 不可调整大小
    ImGuiWindowFlags_NoMove |                // 不可移动
    ImGuiWindowFlags_NoBringToFrontOnFocus|  // 禁止前置
    ImGuiWindowFlags_NoNavFocus;

    // 创建全屏 DockSpace 容器
    ImGui::Begin("FullscreenDockSpace", nullptr, dockSpaceFlags);
    ImGui::PopStyleVar();
    ImGuiID dockSpaceID = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockSpaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N")){}
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::End();

    //绘制画面
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Viewport");
    ImVec2 size = ImGui::GetContentRegionAvail();
    ImGui::Image((intptr_t)imguiF->GetColorAttachmentID(), size, ImVec2{0, 1}, ImVec2{1, 0});
    ImGui::End();
    ImGui::PopStyleVar();
}

void ImGuiRenderer::End()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    // 多视口支持（若启用）
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void ImGuiRenderer::Destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
