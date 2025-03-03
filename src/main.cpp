#include "application.h"

int main()
{
    Application app;

    app.Run();

    // float exposure = 1.0f;
    // while(window.OpenWindow())
    // {
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
        // light.SetPosition(lightP);
        //depthMap
        // light.StartDrawDepthMap(depthShader);
        // raye.StartDrawwithShader(camera, depthShader);
        // roze.StartDrawwithShader(camera, depthShader);
        // light.StopDrawDepthMap();
        //scene
        // Renderer::StartRender();
        // Renderer::StartRenderHDR();
        // shadowMapShader.use();
        // shadowMapShader.setVec3("viewPos", camera.getPosition());
        // shadowMapShader.setVec3("pointLightPos", light.GetPosition());
        // shadowMapShader.setFloat("time", currentTime);
        // light.BindDepthMap(4);
        // raye.StartDrawwithShader(camera, shadowMapShader);
        // roze.StartDrawwithShader(camera, shadowMapShader);
        // light.Draw();
        // Renderer::DrawSkybox();
        // Renderer::EndRenderHDR(exposure, ImGuiRenderer::imguiF->GetColorAttachmentID());
        // ImGuiRenderer::End();
    // }
    return 0;
}