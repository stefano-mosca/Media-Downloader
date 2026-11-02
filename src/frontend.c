#include <widget.h>
#include <SDL3_image/SDL_image.h>
#include <stdlib.h>

#define WIDTH 960
#define HEIGHT 720
#define RED 65
#define GREEN 69
#define BLUE 89

int main(void)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Error SDL: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    if (!TTF_Init())
    {
        SDL_Log("Error TTF: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (!SDL_CreateWindowAndRenderer("Media Downloader", WIDTH, HEIGHT, 0, &window, &renderer))
    {
        SDL_Log("Error SDL window: %s", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_SetRenderVSync(renderer, 1);

    Widget logo =
    {
        .type = wd_image,
        .rect = {24.0f, 12.0f, 36.17f, 44.0f},
        .visible = true,
        .texture = IMG_LoadTexture(renderer, "assets/Images/logo.png"),
    };

    Widget title =
    {
        .type = wd_text,
        .rect = {84.0f, 24.0f, 103.0f, 15.0f},
        .visible = true,
        .text =
        {
            .text = "Media Downloader",
            .font = TTF_OpenFont("assets/Fonts/InterVariable.ttf", 12),
            .color = {198, 208, 245, 255}
        },
    };

    if (!logo.texture || !title.text.font)
    {
        SDL_Log("Error loading logo or text: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    TTF_SetFontStyle(title.text.font, TTF_STYLE_BOLD);

    bool running = true;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, RED, GREEN, BLUE, 255);
        SDL_RenderClear(renderer);

        Widget *ui[] = { &logo, &title };
        size_t ui_cout = sizeof(ui) / sizeof(ui[0]);

        for (size_t i = 0; i < ui_cout; ++i)
        {
            widget_render(ui[i], renderer);
        }
        SDL_RenderPresent(renderer);
    }
    
    TTF_CloseFont(title.text.font);
    SDL_DestroyTexture(logo.texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
