#ifndef WIDGET_H
#define WIDGET_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#define WIDTH 960
#define HEIGHT 720
#define HEXCOLOR(color)    \
    (color >> 24) & 0xFF,  \
    (color >> 16) & 0xFF,  \
    (color >> 8)  & 0xFF,  \
    (color >> 0)  & 0xFF   \

typedef enum WidgetType { wd_image, wd_text } WidgetType;

typedef struct
{
    WidgetType type;
    SDL_FRect rect;
    bool visible;

    union
    {
        SDL_Texture *texture;
        struct
        {
            char *text;
            TTF_Font *font;
            SDL_Color color;
        } text;
    };
} Widget;

void widget_render(const Widget *w, SDL_Renderer *r);
float ui_get_scale(SDL_Renderer *renderer);

#endif
