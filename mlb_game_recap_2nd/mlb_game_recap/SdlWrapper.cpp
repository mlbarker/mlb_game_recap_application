// SdlWrapper.cpp

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "SdlWrapper.h"
#include "Constants.h"

SdlWrapper::SdlWrapper()
    : m_textures()
    , m_backgroundSurface(nullptr)
    , m_windowSurface(nullptr)
    , m_renderer(nullptr)
    , m_window(nullptr)
    , m_exit(false)
{
}

int32_t SdlWrapper::Initialize(const std::string& windowTitle)
{
    int32_t result{ 0 };

    result = InitSdl();
    if (result == EXIT_FAILURE)
    {
        return result;
    }

    result = InitSdlImage();
    if (result == EXIT_FAILURE)
    {
        return result;
    }

    result = CreateWindow(windowTitle);
    if (result == EXIT_FAILURE)
    {
        return result;
    }

    result = CreateRenderer();
    if (result == EXIT_FAILURE)
    {
        return result;
    }

    return EXIT_SUCCESS;
}

int32_t SdlWrapper::LoadMedia(const std::string& path, bool centered, int32_t offsetX, int32_t offsetY)
{
    SDL_Texture* texture = LoadTexture(path.c_str(), centered, offsetX, offsetY);
    if (texture == nullptr)
    {
        std::cout << "Failed to load a texture" << std::endl;
        m_textureRects.pop_back();
        return EXIT_FAILURE;
    }

    m_textures.push_back(texture);

    return EXIT_SUCCESS;
}

int32_t SdlWrapper::Run()
{
    SDL_Event windowEvent;
    while (!m_exit)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (SDL_QUIT == windowEvent.type)
            {
                m_exit = true;
            }

            else if (SDL_KEYDOWN == windowEvent.type)
            {
                ProcessKeyboardInput(windowEvent);
            }
        }

        // clear, render, update
        SDL_RenderClear(m_renderer);
        SDL_Rect renderRect;
        for (size_t index = 0; index < m_textures.size(); ++index)
        {
            renderRect.x = m_textureRects[index].x;
            renderRect.y = m_textureRects[index].y;
            renderRect.w = m_textureRects[index].width;
            renderRect.h = m_textureRects[index].height;
            SDL_RenderCopyEx(m_renderer, m_textures[index], nullptr, &renderRect, 0.0, nullptr, SDL_FLIP_NONE);
        }
        SDL_RenderPresent(m_renderer);

        // make sure the background fits the window
        /*SDL_Rect stretchRect;
        stretchRect.x = 0;
        stretchRect.y = 0;
        stretchRect.w = Constants::WIDTH;
        stretchRect.h = Constants::HEIGHT;
        SDL_BlitScaled(m_backgroundSurface, nullptr, m_windowSurface, &stretchRect);
        SDL_UpdateWindowSurface(m_window);*/
    }

    return EXIT_SUCCESS;
}

int32_t SdlWrapper::Shutdown()
{
    // clean up
    FreeTextures();
    SDL_FreeSurface(m_backgroundSurface);
    SDL_FreeSurface(m_windowSurface);
    m_backgroundSurface = nullptr;
    m_windowSurface = nullptr;

    // quitting...
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    m_renderer = nullptr;
    m_window = nullptr;

    IMG_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}

int32_t SdlWrapper::InitSdl()
{
    // startup SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << "SDL Error: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int32_t SdlWrapper::InitSdlImage()
{
    // Init PNG loading
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        std::cout << "Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int32_t SdlWrapper::CreateWindow(const std::string& title)
{
    // SDL window creation
    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Constants::WIDTH, Constants::HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    if (m_window == nullptr)
    {
        std::cout << "Window Creation Failed: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    // the surface to draw on
    m_windowSurface = SDL_GetWindowSurface(m_window);
    return EXIT_SUCCESS;
}

int32_t SdlWrapper::CreateRenderer()
{
    // SDL renderer creation
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (m_renderer == nullptr)
    {
        std::cout << "Renderer Creation Failed: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    // render redraw color
    SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    return EXIT_SUCCESS;
}

SDL_Surface* SdlWrapper::LoadSurface(const std::string& path)
{
    SDL_Surface* surface = nullptr;
    surface = IMG_Load(path.c_str());

    // failed to load the image
    if (surface == nullptr)
    {
        std::cout << "Image failed to load: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    return surface;
}

SDL_Surface* SdlWrapper::ConvertSurface(SDL_Surface* surface)
{
    // optimizing the image surface
    SDL_Surface* newSurface = nullptr;
    newSurface = SDL_ConvertSurface(surface, m_windowSurface->format, 0);
    if (surface == nullptr)
    {
        std::cout << "Failed to optimitize surface: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    // release the old surface
    // return the new one
    SDL_FreeSurface(surface);
    return newSurface;
}

void SdlWrapper::ProcessKeyboardInput(SDL_Event& event)
{
    switch (event.key.keysym.sym)
    {
    case SDLK_LEFT:
        UpdateTextureMovement(-500);
        break;

    case SDLK_RIGHT:
        UpdateTextureMovement(500);
        break;

    default:
        break;
    }
}

SDL_Texture* SdlWrapper::LoadTexture(const std::string& path, bool centered, int32_t offsetX, int32_t offsetY)
{
    if (m_renderer == nullptr)
    {
        return nullptr;
    }

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == nullptr)
    {
        std::cout << "Image failed to load for texture creation: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    // create texture from surface pixels
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    if (texture == nullptr)
    {
        std::cout << "Failed to load texture from surface: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    RenderCoords textureCoords;
    textureCoords.width = surface->w;
    textureCoords.height = surface->h;
    
    if (centered)
    {
        textureCoords.x = (Constants::WIDTH >> 1) - (textureCoords.width >> 1);
        textureCoords.y = (Constants::HEIGHT >> 1) - (textureCoords.height >> 1);
    }

    // applying the offset
    if (offsetX != 0)
    {
        textureCoords.x += (offsetX + textureCoords.width);
    }
    if (offsetY != 0)
    {
        textureCoords.y += (offsetY + textureCoords.height);
    }

    m_textureRects.push_back(textureCoords);

    // Release the loaded surface
    SDL_FreeSurface(surface);

    return texture;
}

void SdlWrapper::FreeTextures()
{
    for (size_t index = 0; index < m_textures.size(); ++index)
    {
        SDL_DestroyTexture(m_textures[index]);
        m_textures[index] = nullptr;
    }
    m_textures.clear();
}

void SdlWrapper::UpdateTextureMovement(int32_t moveX)
{
    for (size_t index = 1; index < m_textures.size(); ++index)
    {
        m_textureRects[index].x += moveX;
    }
}
