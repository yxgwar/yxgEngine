#include "ImGuiRenderer.h"

void ImGuiRenderer::Init(Window &window)
{
    m_width = window.GetWidth();
    m_height = window.GetHeight();
    //imgui
    imguiF = std::make_unique<FrameBuffer>();
    imguiF->attachColor(m_width, m_height);
    
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
    ImVec2 displaySize = size;
    float aspectRatio = (float)imguiF->GetWidth() / (float)imguiF->GetHeight();

    // 计算保持宽高比的显示尺寸
    if (size.x / aspectRatio < size.y)
        displaySize.y = size.x / aspectRatio;
    else
        displaySize.x = size.y * aspectRatio;

    // 居中显示
    ImGui::SetCursorPosX((size.x - displaySize.x) * 0.5f);
    ImGui::SetCursorPosY((size.y - displaySize.y) * 0.5f);
    
    ImGui::Image((intptr_t)imguiF->GetColorAttachmentID(), displaySize, ImVec2{0, 1}, ImVec2{1, 0});
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
