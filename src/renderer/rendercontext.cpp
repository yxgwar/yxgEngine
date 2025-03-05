#include "rendercontext.h"

std::shared_ptr<FrameBuffer> RenderContext::GetFBO(FBOType type)
{
    auto it = fboPool.find((int)type);
    return it != fboPool.end() ? it->second : nullptr;
}

void RenderContext::GenDepthMap(int width)
{
    std::shared_ptr<FrameBuffer> fbo = std::make_shared<FrameBuffer>();
    fbo->attachDepth(width, width);
    fbo->bindTexture(SHADOWMAP);
    fboPool[(int)FBOType::DepthMap] = fbo;
}

void RenderContext::GenForwad(int width, int height)
{
    std::shared_ptr<FrameBuffer> fbo = std::make_shared<FrameBuffer>();
    fbo->attachMultiple(width, height);
    fboPool[(int)FBOType::Forward] = fbo;
    auto postShader = Import::ShaderPool["PostProcess"];
    postShader->use();
    postShader->setInt("screenTexture", 0);
    postShader->setInt("samples", 4);
    postShader->setVec2("textureSize", width, height);
}

void RenderContext::GenUBO()
{
    cameraUBO = std::make_unique<UniformBuffer>(2 * sizeof(glm::mat4));
    cameraUBO->bind(0, 2 * sizeof(glm::mat4));
}

void RenderContext::UpdateGlobalUBO(Camera *camera)
{
    cameraUBO->setData(0, sizeof(glm::mat4), glm::value_ptr(camera->getProjection()));
    cameraUBO->setData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera->getView()));
}