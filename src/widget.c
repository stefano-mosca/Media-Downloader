#include <widget.h>

void widget_render(const Widget *w, SDL_Renderer *r)
{
    if (!w || !w->visible) return;

    switch (w->type) 
    {
        case wd_image:
            SDL_RenderTexture(r, w->texture, NULL, &w->rect);
            break;
        case wd_text:
            SDL_Surface *s = TTF_RenderText_Blended(w->text.font, w->text.text, strlen(w->text.text), w->text.color);
            if (!s)
            {
                SDL_Log("Text rendering error: %s", SDL_GetError());
                break;
            }

            SDL_Texture *t = SDL_CreateTextureFromSurface(r, s);
            if (!t)
            {
                SDL_Log("Creating texture error: %s", SDL_GetError());
                SDL_DestroySurface(s);
                break;
            }

            SDL_RenderTexture(r, t, NULL, &w->rect);
            SDL_DestroySurface(s);
            SDL_DestroyTexture(t);
            break;
    }
}
