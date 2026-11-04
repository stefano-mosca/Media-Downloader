#include <widget.h>
#include <SDL3_image/SDL_image.h>
#include <stdlib.h>

static void update_fonts(SDL_Renderer *renderer, TTF_Font *font, float base_size);
static void update_render_scale(SDL_Renderer *renderer);

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

    if (!SDL_CreateWindowAndRenderer("Media Downloader", WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY, &window, &renderer))
    {
        SDL_Log("Error SDL window: %s", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_SetRenderVSync(renderer, 1);
    SDL_SetWindowMinimumSize(window, (int) WIDTH / 1.3, (int) HEIGHT / 1.3);

    SDL_Texture *logo = IMG_LoadTexture(renderer, "assets/Images/logo.png");

    if (!logo)
    {
        SDL_Log("Error loading logo: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    TTF_Font *font_semibold = TTF_OpenFont("assets/Fonts/Inter_18pt-SemiBold.ttf", 15);
    if (!font_semibold)
    {
        SDL_Log("Error loading Bold Font: %s", SDL_GetError());
        SDL_DestroyTexture(logo);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    update_fonts(renderer, font_semibold, 15.0f);
    update_render_scale(renderer);

    Widget ui[] =
    {
        {
            .type = wd_image, .rect = {32.0f, 18.0f, 36.17f, 44.0f}, .visible = true, 
            .texture = logo
        },

        {
          .type = wd_text, .rect = {80.17f, 31.0f, 136.0f, 18.0f}, .visible = true,
          .text = { .text = "Media Downloader", .font = font_semibold, .color = { HEXCOLOR(0xF5F7FAFF) } }
        },

        {
          .type = wd_text, .rect = {180.0f, 124.0f, 64.0f, 16.0f}, .visible = true,
          .text = { .text = "Media link", .font = font_semibold, .color = { HEXCOLOR(0xF5F7FAFF) } }
        },
    };

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

            if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED || event.type == SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED)
            {
                update_render_scale(renderer);
                update_fonts(renderer, font_semibold, 15.0f);
            }
        }

        SDL_SetRenderDrawColor(renderer, HEXCOLOR(0x0F1115FF));
        SDL_RenderClear(renderer);

        size_t ui_cout = sizeof(ui) / sizeof(ui[0]);
        for (size_t i = 0; i < ui_cout; ++i)
        {
            widget_render(&ui[i], renderer);
        }

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font_semibold);
    SDL_DestroyTexture(logo);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

static void update_fonts(SDL_Renderer *renderer, TTF_Font *font, float base_size) 
{
    TTF_SetFontSize(font, base_size * ui_get_scale(renderer));
}

static void update_render_scale(SDL_Renderer *renderer)
{
    float s = ui_get_scale(renderer);
    SDL_SetRenderScale(renderer, s, s);
}
