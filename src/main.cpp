#include "application.h"

int main()
{
    Application app;

    app.Run();

    // float exposure = 1.0f;
        // ImGuiRenderer::Begin();
        // {
        //     ImGui::Begin("Hello, world!");
        //     ImGuiIO& io = ImGui::GetIO(); (void)io;
        //     ImGui::Text("This is some useful text.");
        //     ImGui::DragFloat("X", &lightP.x, 0.1f, 0.0f, 0.0f, "%.2f");
        //     ImGui::DragFloat("Y", &lightP.y, 0.1f, 0.0f, 0.0f, "%.2f");
        //     ImGui::DragFloat("Z", &lightP.z, 0.1f, 0.0f, 0.0f, "%.2f");
        //     ImGui::DragFloat("exposure", &exposure, 0.01f, 0.0f, 0.0f, "%.2f");
        //     ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        //     ImGui::End();
        // }
        // Renderer::DrawSkybox();
    // }
    return 0;
}