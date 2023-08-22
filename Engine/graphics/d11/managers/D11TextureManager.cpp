#include "pch.h"
#include "D11TextureManager.h"

#include "graphics/d11/resources/D11Texture2D.h"
#include "graphics/d11/D11GraphicsContext.h"

#include "resources/AssetManager.h"

#include "threading/ThreadPool.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void D11TextureManager::LoadTexture(std::shared_ptr<D11Texture2D> tex, TextureRef ref, std::string src, std::string meta)
{
    if (tex->IsInitialized()) {
        return; //TODO: *sigh*
    }
    if (src.empty()) {
        return; //TODO: ERRORS!!
    }

    Texture2DOptions options{};
    options.Context = m_Context;

    if (!meta.empty()) {
        auto config = Configurations::LoadConfig(meta);

        auto tconfig = config->GetChild("Texture");
        options.Filtering = tconfig->GetValue<int>("Filtering");
        options.Repeating = tconfig->GetValue<int>("Repeating");
    }

    int w, h, ch;
    options.Pixels = stbi_load_from_memory((uchar*)src.c_str(), src.size(), &w, &h, &ch, 4);

    if (options.Pixels == NULL) {
        return; //TODO: ERORORRORORORSS!
    }

    options.Width = w;
    options.Height = h;
    options.Channels = ch;

    tex->Initialize(options);
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

void D11TextureManager::Initialize(std::shared_ptr<IGraphicsContext> Context, std::shared_ptr<IAssetManager> Assets, bool IsThreaded)
{
    m_Context = (D11GraphicsContext*)Context.get();
    m_isThreaded = IsThreaded;
    m_assetManager = Assets;

    m_TextureReference = std::vector<std::pair<std::string, TextureRef>>();
    m_TextureList = std::vector<std::pair<TextureRef, std::shared_ptr<D11Texture2D>>>();
}

void D11TextureManager::Shutdown()
{
    Clear();
    m_Context = NULL;
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

    std::string src = m_assetManager->GetAsset(path);
    std::string meta = m_assetManager->GetAsset(path + ".astral");

    if (m_isThreaded) {
        m_resourcePool->QueueJob(std::bind(&D11TextureManager::LoadTexture, this, texture, ref, src, meta));
    }
    else {
        LoadTexture(texture, ref, src, meta);
    }

    return ref;
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
