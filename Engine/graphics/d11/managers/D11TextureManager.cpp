#include "pch.h"
#include "D11TextureManager.h"

#include "graphics/d11/resources/D11Texture2D.h"
#include "graphics/d11/D11GraphicsContext.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void D11TextureManager::LoadTexture()
{
    while (!m_killThread)
    {
        for (auto pair : m_TextureList) {
            if (!pair.second->IsInitialized()) {
                Texture2DOptions options{};

                //TODO: get astral file from code

                std::string dataBuffer = "";//CODE

                int w, h, ch;
                options.Pixels = stbi_load_from_memory((uchar*)dataBuffer.c_str(), dataBuffer.size(), &w, &h, &ch, 4);

                options.Width = w;
                options.Height = h;
                options.Channels = ch;

                pair.second->Initialize(options);

                m_remainingTextures--; //TODO: maybe mutex because of modification stuff
            }

            if (m_remainingTextures <= 0)
                break;
        }

        if (m_isThreaded) {
            std::unique_lock<std::mutex> lock(m_textureListMutex);
            while (m_remainingTextures <= 0) m_textureListCondition.wait(lock);
        }
        else {
            break;
        }
    }
}

void D11TextureManager::ReadjustReferences()
{
    uint32_t i = 0;
    for (auto pair : m_TextureList)
    {
        pair.first->ReconsiderRef(i);

        i++;
    }
}

void D11TextureManager::Initialize(std::shared_ptr<IGraphicsContext> Context, bool IsThreaded)
{
    m_Context = (D11GraphicsContext*)Context.get();
    m_isThreaded = IsThreaded;

    if (m_isThreaded) {
        m_loadingThread = std::thread(D11TextureManager::LoadTexture, this);
        m_loadingThread.detach();
    }
}

void D11TextureManager::Shutdown()
{
    Clear();
    m_Context = NULL;

    if (m_isThreaded) {
        m_killThread = true;
        m_loadingThread.join();
    }
}

TextureRef D11TextureManager::GetTexture(std::string path)
{
    for (auto entry : m_TextureReference)
    {
        if (entry.first == path) {
            return entry.second;
        }
    }

    TextureRef ref = TextureRef_(m_TextureList.size());

    std::shared_ptr<D11Texture2D> texture = std::make_shared<D11Texture2D>();

    m_TextureList.push_back(std::make_pair(ref, texture));
    m_TextureReference.push_back(std::make_pair(path, ref));

    m_remainingTextures++;

    if (m_isThreaded) {
        m_textureListCondition.notify_all();
    }
    else {
        LoadTexture();
    }
}

std::shared_ptr<ITexture2D> D11TextureManager::GetTextureFromRef(TextureRef ref)
{
    if (ref->IsValid() && ref->GetRefId() < m_TextureList.size()) {
        return m_TextureList[ref->GetRefId()].second;
    }

    return NULL;
}

void D11TextureManager::LookForUnused()
{
    bool needReadjust = false;

    uint32_t i = 0;
    for (auto pair : m_TextureList)
    {
        if (pair.second.use_count() <= 1 && pair.first.use_count() <= 2) {
            pair.first->Invalidate();
        }
        if (!pair.first->IsValid()) {
            m_TextureList.erase(m_TextureList.begin() + i);
            m_TextureReference.erase(m_TextureReference.begin() + i);

            needReadjust = true;
        }

        i++;
    }

    if (needReadjust)
        ReadjustReferences();
}

void D11TextureManager::Clear()
{
    for (auto pair : m_TextureList)
    {
        pair.first->Invalidate();
    }

    m_TextureList.clear();
    m_TextureReference.clear();
}
