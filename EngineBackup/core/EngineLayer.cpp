#include "pch.h"
#include "EngineLayer.h"

EngineLayer::EngineLayer(Engine* engine)
{
    m_Engine = engine;
}

Engine* EngineLayer::GetEngine()
{
    return m_Engine;
}

void EngineLayer::OnEvent(SDL_Event& Event)
{
}
