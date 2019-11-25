// SdlWrapper.h

#ifndef _SDL_WRAPPER_H
#define _SDL_WRAPPER_H

#include <vector>
#include "GameEnums.h"
#include "RenderCoords.h"

// forward declarations
struct SDL_Surface;
struct SDL_Window;

class SdlWrapper
{
public:
    SdlWrapper();

    int32_t Initialize(const std::string& windowTitle);
    int32_t LoadBackground(const std::string& path);
    int32_t LoadMedia(const std::string& path, bool centered = false, int32_t offsetX = 0, int32_t offsetY = 0);
    int32_t Run();
    int32_t Shutdown();

private:
    int32_t InitSdl();
    int32_t InitSdlImage();
    int32_t CreateWindow(const std::string& title);
    int32_t CreateRenderer();
    SDL_Surface* LoadSurface(const std::string& path);
    SDL_Surface* ConvertSurface(SDL_Surface* surface);
    void ProcessKeyboardInput(SDL_Event& event);
    SDL_Texture* LoadTexture(const std::string& path, bool centered, int32_t offsetX, int32_t offsetY);
    void FreeTextures();
    void UpdateTextureMovement(int32_t moveX);

    std::vector<SDL_Texture*> m_textures;
    std::vector<RenderCoords> m_textureRects;
    SDL_Surface* m_windowSurface;
    SDL_Texture* m_backgroundTexture;
    SDL_Renderer* m_renderer;
    SDL_Window* m_window;
    uint32_t m_cursor;
    bool m_exit;
};

#endif
