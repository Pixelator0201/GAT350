#include "Engine.h"
#include "Core/File.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Pipeline.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/euler_angles.hpp>
#include <glm/glm/gtx/transform.hpp>


#include "Resources/ResourceManager.h"

using namespace nu;

namespace nu
{
    struct Color4
    {
        float r, g, b, a;
    };
}

struct Vertex
{
    float x, y, z;
    //Vector3 position;
    //Color4 color;
};

std::vector<Vertex> vertices =
{
    Vertex{ -0.67f, -1.0f, 0.0f}, // Bottom-Left
    //Vertex{ Vector3{-1.0f, -1.0f, 0.0f}, Color4{1, 0, 0, 1}}, // Bottom-Left
    //Vertex{ Vector3{1.0f, -1.0f, 0.0f}, Color4{1, 1, 0, 1}}, // Bottom-Right
    //Vertex{ Vector3{0.0f, 1.0f, 0.0f}, Color4{1, 0, 1, 1}}, // Top-Middle
    Vertex{  1.0f, -0.67f, 0.0f}, // Bottom-Right
    Vertex{  0.0f,  0.75f, 0.0f}, // Top-Middle
};

std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };

int main()
{
    SetWorkingDirectory("Assets");

    // INITIALIZATION
    Engine::Instance().Initialize();

    struct vertexUniform
    {
        glm::mat4 model;
    };

    vertexUniform uniform;

    auto vb = std::make_shared<VertexBuffer>();
    vb->Create<Vertex>(vertices, Engine::Instance().GetRenderer().GetGPUDevice());

    //auto ib = std::make_shared<IndexBuffer>();
    //ib->Create(indices, Engine::Instance().GetRenderer());

    auto vshader = Resources().Get<nu::Shader>("shaders/position.vert", Engine::Instance().GetRenderer());
    auto fshader = Resources().Get<nu::Shader>("shaders/color.frag", Engine::Instance().GetRenderer());

    auto pipeline = std::make_shared<Pipeline>();
    pipeline->AddVertexBuffer(sizeof(Vertex));
    //position
    pipeline->AddVertexAttribute(
        0,
        SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
        offsetof(Vertex, x));
    //Color
    //pipeline->AddVertexAttribute(
    //    0,
    //    SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
    //    offsetof(Vertex, color));

    pipeline->Create(*vshader.get(), *fshader.get(),
        Engine::Instance().GetRenderer().GetGPUDevice(),
        Engine::Instance().GetRenderer().GetWindow());

    float scale = 0.5f;
    Color4 color{0, 0, 0, 1};

    // MAIN LOOP
    bool quit = false;
    while (!quit) 
    {
        // UPDATE
        SDL_Event event;
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_EVENT_QUIT) 
            {
                quit = true;
            }
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE)
            {
                quit = true;
            }
        }

        // ENGINE
        Engine::Instance().Update();
        float dt = Engine::Instance().GetTime().GetDeltaTime();
        float time = Engine::Instance().GetTime().GetTime();

        scale = sin(Engine::Instance().GetTime().GetDeltaTime());
        color.r = sin(Engine::Instance().GetTime().GetDeltaTime()) * 0.5f + 0.5f;


        // 1 0 0 0
        // 0 1 0 0
        // 0 0 1 0
        // 0 0 0 1

        uniform.model = glm::mat4(1.0f);

        auto mxt = glm::translate(glm::mat4{ 1.0f }, glm::vec3(0.5f, 0.0f, 0.0f));
        //auto mxrx = glm::rotate(glm::mat4{ 1.0f }, time * 3.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        //auto mxry = glm::rotate(glm::mat4{ 1.0f }, time * 3.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        //auto mxr = mxrx * mxry;
        auto mxr = glm::eulerAngleYXZ(1.2f, time * 3.0f, 0.0f );
        auto mxs = glm::scale(glm::mat4{ 1.0f }, glm::vec3(0.5f, 0.5f, 0.5f));

        uniform.model = glm::translate(uniform.model, glm::vec3(0.5f, 0.0f, 0.0f));
        uniform.model = glm::rotate(uniform.model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        uniform.model = glm::scale(uniform.model, glm::vec3(0.5f, 0.5f, 0.5f));

        uniform.model = mxs * mxr * mxt;


        // RENDER
        Engine::Instance().GetRenderer().BeginFrame();

        Engine::Instance().GetRenderer().SetPipeline(*pipeline);
        Engine::Instance().GetRenderer().SetVertexBuffer(*vb);
        //Engine::Instance().GetRenderer().SetIndexBuffer(*ib);
        //Engine::Instance().GetRenderer().SetVertexUniform(0, &uniform, sizeof(uniform));
        //Engine::Instance().GetRenderer().SetFragmentUniform(0, &color, sizeof(color));
        Engine::Instance().GetRenderer().Draw(vb->GetVertexCount());
        //Engine::Instance().GetRenderer().DrawIndex(*ib);

        Engine::Instance().GetRenderer().EndFrame();
    }


    // SHUTDOWN
    Engine::Instance().Shutdown();    

    return 0;
}

