#include <SDL.h>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;


void log(const char* fmt, ...)
{
    static FILE *log_file;
    
    log_file = fopen("log.txt", "a");

    if (log_file)
    {
        char fmt_[4096] = {0};
        char msg[4096] = {0};

        strcpy(fmt_, fmt);
        strcat(fmt_, "\n");

        va_list args;
        va_start(args, fmt);

        vsnprintf(msg, sizeof(msg), fmt_, args);
        
        va_end(args);

        fprintf(log_file, msg);
        
        fclose(log_file);
        
        log_file = nullptr;
    }
}

bool done = false;

void handle_event()
{
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            done = true;
        }
        else if (e.type == SDL_WINDOWEVENT)
        {
            switch (e.window.event)
            {
                case SDL_WINDOWEVENT_RESIZED:
                    log("Window %d resized to %dx%d",
                            e.window.windowID, e.window.data1,
                            e.window.data2);
                    break;

                default:
                    break;
            }
        }
    }
}

int main(int argc, char* args[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    window = SDL_CreateWindow("tinyrenderer",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_RendererInfo renderer_info;
    SDL_GetRendererInfo(renderer, &renderer_info);
    
    log("info, cache line size: %d", SDL_GetCPUCacheLineSize());


    while (!done)
    {
        handle_event();

        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0xFF);

        SDL_RenderClear(renderer);


        SDL_SetRenderDrawColor(renderer, 0xAA, 0xBB, 0xCC, 0xFF);

        SDL_RenderDrawLine(renderer, 320, 200, 300, 240);
        SDL_RenderDrawLine(renderer, 300, 240, 340, 240);
        SDL_RenderDrawLine(renderer, 340, 240, 320, 200);

        //SDL_RenderCopy(renderer, texture, NULL, NULL);

        SDL_RenderPresent(renderer);

        SDL_Rect rect = {0};
        SDL_RenderGetViewport(renderer, &rect);


        SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}