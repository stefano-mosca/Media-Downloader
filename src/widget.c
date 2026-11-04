#include <widget.h>

float ui_get_scale(SDL_Renderer *r)
{
    int pw, ph;
    if (!SDL_GetRenderOutputSize(r, &pw, &ph)) return 1.0f;

    float sx = pw / (float) WIDTH;
    float sy = ph / (float) HEIGHT;
    float s  = sx < sy ? sx : sy;

    return s >= 1.0f ? (float)(int) s : 1.0f;
}

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

            float scale = ui_get_scale(r);
            float tw = s->w / scale;
            float th = s->h / scale;

            SDL_FRect dst = {
                w->rect.x,
                w->rect.y + (w->rect.h - th) / 2.0f,
                tw,
                th
            };

            SDL_RenderTexture(r, t, NULL, &dst);
            SDL_DestroySurface(s);
            SDL_DestroyTexture(t);
            break;
    }
}
