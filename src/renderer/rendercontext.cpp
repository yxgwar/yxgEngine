#include "rendercontext.h"
#include "import/import.h"
#include "glm/gtc/type_ptr.hpp"
#include "log.h"

std::shared_ptr<FrameBuffer> RenderContext::GetFBO(FBOType type)
{
    auto it = fboPool.find((int)type);
    return it != fboPool.end() ? it->second : nullptr;
}

void RenderContext::GenDepthMap(int width)
{
    fboPool[(int)FBOType::DepthMap] = std::make_shared<FrameBuffer>();
    fboPool[(int)FBOType::DepthMap]->attachDepth(width, width);
    fboPool[(int)FBOType::DepthMap]->bindTexture(SHADOWMAP);
}

void RenderContext::GenForwad(int width, int height)
{
    fboPool[(int)FBOType::Forward] = std::make_shared<FrameBuffer>();
    fboPool[(int)FBOType::Forward]->attachMultiple(width, height);
    auto postShader = Import::GetShader("PostProcess");
    if(postShader)
    {
        postShader->use();
        postShader->setInt("screenTexture", 0);
        postShader->setInt("samples", 4);
        postShader->setVec2("textureSize", width, height);
    }
    else
        ERROR("postShader initialization fail!");
}

void RenderContext::GenHDR(int width, int height)
{
    fboPool[(int)FBOType::HDR] = std::make_shared<FrameBuffer>();
    fboPool[(int)FBOType::HDR]->attachHDR(width, height);
    
    auto hdrShader = Import::GetShader("HDR");
    if(hdrShader)
    {
        hdrShader->use();
        hdrShader->setInt("scene", 0);
        hdrShader->setInt("bloomBlur", 1);
    }
    else
        ERROR("hdrshader initialization fail!");

    fboPool[(int)FBOType::BLURH] = std::make_shared<FrameBuffer>();
    fboPool[(int)FBOType::BLURH]->attachPingPong(width, height);
    fboPool[(int)FBOType::BLURV] = std::make_shared<FrameBuffer>();
    fboPool[(int)FBOType::BLURV]->attachPingPong(width, height);
    auto blurShader = Import::GetShader("blur");
    if(blurShader)
    {
        blurShader->use();
        blurShader->setInt("image", 0);
    }
    else
        ERROR("blurshader initialization fail!");
}

void RenderContext::GengBuffer(int width, int height)
{
    fboPool[(int)FBOType::gBUFFER] = std::make_shared<FrameBuffer>();
    fboPool[(int)FBOType::gBUFFER]->attachgBuffer(width, height);
    
    auto defferShader = Import::GetShader("deffer");
    if(defferShader)
    {
        defferShader->use();
        defferShader->setInt("gPosition", 0);
        defferShader->setInt("gNormal", 1);
        defferShader->setInt("gAlbedoSpec", 2);
        defferShader->setInt("gShadow", 3);
        defferShader->setInt("ssao", 4);
    }
    else
        ERROR("deffershader initialization fail!");
}

void RenderContext::GenSSAO(int width, int height, void* noise)
{
    noiseTex = std::make_unique<Texture>(4, 4, noise);

    fboPool[(int)FBOType::SSAO] = std::make_shared<FrameBuffer>();
    fboPool[(int)FBOType::SSAO]->attachSingle(width, height);

    auto ssaoShader = Import::GetShader("ssao");
    if(ssaoShader)
    {
        ssaoShader->use();
        ssaoShader->setInt("gPosition", 0);
        ssaoShader->setInt("gNormal", 1);
        ssaoShader->setInt("texNoise", 2);
    }
    else
        ERROR("ssao shader initialization fail!");
    
    fboPool[(int)FBOType::SSAOblur] = std::make_shared<FrameBuffer>();
    fboPool[(int)FBOType::SSAOblur]->attachSingle(width, height);

    auto ssaoblurShader = Import::GetShader("ssaoblur");
    if(ssaoblurShader)
    {
        ssaoblurShader->use();
        ssaoblurShader->setInt("ssaoInput", 0);
    }
    else
        ERROR("ssaoblur shader initialization fail!");
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