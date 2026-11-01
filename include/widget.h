#ifndef WIDGET_H
#define WIDGET_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

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

#endif
