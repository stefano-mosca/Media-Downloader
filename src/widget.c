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
            break;
    }
}
