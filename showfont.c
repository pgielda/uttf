/*
  showfont:  An example of using the SDL_ttf library with 2D graphics.
  Copyright (C) 2001-2013 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

/* A simple program to test the text rendering feature of the TTF library */

/* quiet windows compiler warnings */
#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "uttf.h"
#include "SDL.h"

#define DEFAULT_PTSIZE  18
#define DEFAULT_TEXT    "The quick brown fox jumped over the lazy dog"
#define NUM_COLORS      256
#define WIDTH   640
#define HEIGHT  480

static char *Usage =
"Usage: %s [-solid] [-utf8|-unicode] [-b] [-i] [-u] [-s] [-outline size] [-hintlight|-hintmono|-hintnone] [-nokerning] [-fgcol r,g,b] [-bgcol r,g,b] <font>.ttf [ptsize] [text]\n";

typedef struct {
    SDL_Texture *caption;
    SDL_Rect captionRect;
    SDL_Texture *message;
    SDL_Rect messageRect;
} Scene;

static void draw_scene(SDL_Renderer *renderer, Scene *scene)
{
    /* Clear the background to background color */
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, scene->caption, NULL, &scene->captionRect);
    SDL_RenderCopy(renderer, scene->message, NULL, &scene->messageRect);
    SDL_RenderPresent(renderer);
}

static void cleanup(int exitcode)
{
    TTF_Quit();
    SDL_Quit();
    exit(exitcode);
}

Uint32 SDLColorToUint32(SDL_Color col) {
	Uint32 result = col.b;
	result |= col.g << 8;
	result |= col.r << 16;
	result |= col.a << 24;
	return result;
}

int main(int argc, char *argv[])
{
    char *argv0 = argv[0];
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    Uint8 *text;
    Scene scene;
    int ptsize;
    int i, done;
    SDL_Color white = { 0xFF, 0xFF, 0xFF, 0 };
    SDL_Color black = { 0x00, 0x00, 0x00, 0 };
    SDL_Color *forecol;
    SDL_Color *backcol;
    SDL_Event event;
    int rendersolid;
    int renderstyle;
    int outline;
    int hinting;
    int kerning;
    int dump;
    enum {
        RENDER_LATIN1,
        RENDER_UTF8,
        RENDER_UNICODE
    } rendertype;
    char *message, string[128];

    /* Look for special execution mode */
    dump = 0;
    /* Look for special rendering types */
    rendersolid = 0;
    renderstyle = TTF_STYLE_NORMAL;
    rendertype = RENDER_LATIN1;
    outline = 0;
    hinting = TTF_HINTING_NORMAL;
    kerning = 1;
    /* Default is black and white */
    forecol = &black;
    backcol = &white;
    for ( i=1; argv[i] && argv[i][0] == '-'; ++i ) {
        if ( strcmp(argv[i], "-solid") == 0 ) {
            rendersolid = 1;
        } else
        if ( strcmp(argv[i], "-utf8") == 0 ) {
            rendertype = RENDER_UTF8;
        } else
        if ( strcmp(argv[i], "-unicode") == 0 ) {
            rendertype = RENDER_UNICODE;
        } else
        if ( strcmp(argv[i], "-b") == 0 ) {
            renderstyle |= TTF_STYLE_BOLD;
        } else
        if ( strcmp(argv[i], "-i") == 0 ) {
            renderstyle |= TTF_STYLE_ITALIC;
        } else
        if ( strcmp(argv[i], "-u") == 0 ) {
            renderstyle |= TTF_STYLE_UNDERLINE;
        } else
        if ( strcmp(argv[i], "-s") == 0 ) {
            renderstyle |= TTF_STYLE_STRIKETHROUGH;
        } else
        if ( strcmp(argv[i], "-outline") == 0 ) {
            if ( sscanf (argv[++i], "%d", &outline) != 1 ) {
                fprintf(stderr, Usage, argv0);
                return(1);
            }
        } else
        if ( strcmp(argv[i], "-hintlight") == 0 ) {
            hinting = TTF_HINTING_LIGHT;
        } else
        if ( strcmp(argv[i], "-hintmono") == 0 ) {
            hinting = TTF_HINTING_MONO;
        } else
        if ( strcmp(argv[i], "-hintnone") == 0 ) {
            hinting = TTF_HINTING_NONE;
        } else
        if ( strcmp(argv[i], "-nokerning") == 0 ) {
            kerning = 0;
        } else
        if ( strcmp(argv[i], "-dump") == 0 ) {
            dump = 1;
        } else
        if ( strcmp(argv[i], "-fgcol") == 0 ) {
            int r, g, b;
            if ( sscanf (argv[++i], "%d,%d,%d", &r, &g, &b) != 3 ) {
                fprintf(stderr, Usage, argv0);
                return(1);
            }
            forecol->r = (Uint8)r;
            forecol->g = (Uint8)g;
            forecol->b = (Uint8)b;
        } else
        if ( strcmp(argv[i], "-bgcol") == 0 ) {
            int r, g, b;
            if ( sscanf (argv[++i], "%d,%d,%d", &r, &g, &b) != 3 ) {
                fprintf(stderr, Usage, argv0);
                return(1);
            }
            backcol->r = (Uint8)r;
            backcol->g = (Uint8)g;
            backcol->b = (Uint8)b;
        } else {
            fprintf(stderr, Usage, argv0);
            return(1);
        }
    }
    argv += i;
    argc -= i;

    /* Check usage */
    if ( ! argv[0] ) {
        fprintf(stderr, Usage, argv0);
        return(1);
    }

    /* Initialize the TTF library */
    if ( TTF_Init() < 0 ) {
        fprintf(stderr, "Couldn't initialize TTF: %s\n",SDL_GetError());
        SDL_Quit();
        return(2);
    }

    /* Open the font file with the requested point size */
    ptsize = 0;
    if ( argc > 1 ) {
        ptsize = atoi(argv[1]);
    }
    if ( ptsize == 0 ) {
        i = 2;
        ptsize = DEFAULT_PTSIZE;
    } else {
        i = 3;
    }
    font = TTF_OpenFont(argv[0], ptsize);
    if ( font == NULL ) {
        fprintf(stderr, "Couldn't load %d pt font from %s: %s\n",
                    ptsize, argv[0], SDL_GetError());
        cleanup(2);
    }
    TTF_SetFontStyle(font, renderstyle);
    TTF_SetFontOutline(font, outline);
    TTF_SetFontKerning(font, kerning);
    TTF_SetFontHinting(font, hinting);

    if( dump ) {
        for( i = 48; i < 123; i++ ) {
            Uint8* glyph = NULL;

            glyph = TTF_RenderGlyph_Shaded( font, i, SDLColorToUint32(*forecol), SDLColorToUint32(*backcol) );

            if( glyph ) {
                char outname[64];
                sprintf( outname, "glyph-%d.bmp", i );
		SDL_Surface *sdl_glyph = fn_to_sdl_surface(glyph);
                SDL_SaveBMP( sdl_glyph, outname );
		SDL_FreeSurface(sdl_glyph);
            }

        }
        cleanup(0);
    }

    /* Create a window */
    if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer) < 0) {
        fprintf(stderr, "SDL_CreateWindowAndRenderer() failed: %s\n", SDL_GetError());
        cleanup(2);
    }

    /* Show which font file we're looking at */
    sprintf(string, "Font file: %s", argv[0]);  /* possible overflow */
    if ( rendersolid ) {
        text = TTF_RenderText_Solid(font, string, SDLColorToUint32(*forecol));
    } else {
        text = TTF_RenderText_Shaded(font, string, SDLColorToUint32(*forecol), SDLColorToUint32(*backcol));
    }
    if ( text != NULL ) {
        scene.captionRect.x = 4;
        scene.captionRect.y = 4;
        scene.captionRect.w = fn_w(text);
        scene.captionRect.h = fn_h(text);
	SDL_Surface *sdl_text = fn_to_sdl_surface(text);
	printf("surf is here! %s\n", __func__);
            SDL_Palette *palette = sdl_text->format->palette;
	Uint32 fg = SDLColorToUint32(*forecol);
	Uint32 bg = SDLColorToUint32(*backcol);
    int rdiff = cl_r(fg) - cl_r(bg);
    int gdiff = cl_g(fg) - cl_g(bg);
    int bdiff = cl_b(fg) - cl_b(bg);

    for ( int index = 0; index < 256; ++index ) {
        palette->colors[index].r = cl_r(bg) + (index*rdiff) / (255);
        palette->colors[index].g = cl_g(bg) + (index*gdiff) / (255);
        palette->colors[index].b = cl_b(bg) + (index*bdiff) / (255);
    }

        scene.caption = SDL_CreateTextureFromSurface(renderer, sdl_text);
	SDL_FreeSurface(sdl_text);
        free(text);
    }

    /* Render and center the message */
    if ( argc > 2 ) {
        message = argv[2];
    } else {
        message = DEFAULT_TEXT;
    }
    switch (rendertype) {
        case RENDER_LATIN1:
        if ( rendersolid ) {
            text = TTF_RenderText_Solid(font,message,SDLColorToUint32(*forecol));
        } else {
            text = TTF_RenderText_Shaded(font,message,SDLColorToUint32(*forecol),SDLColorToUint32(*backcol));
        }
        break;

        case RENDER_UTF8:
        if ( rendersolid ) {
            text = TTF_RenderUTF8_Solid(font,message,SDLColorToUint32(*forecol));
        } else {
            text = TTF_RenderUTF8_Shaded(font,message,SDLColorToUint32(*forecol),SDLColorToUint32(*backcol));
        }
        break;

        case RENDER_UNICODE:
        {
            Uint16 *unicode_text = SDL_iconv_utf8_ucs2(message);
            if ( rendersolid ) {
                text = TTF_RenderUNICODE_Solid(font,
                    unicode_text, SDLColorToUint32(*forecol));
            } else {
                text = TTF_RenderUNICODE_Shaded(font,
                    unicode_text, SDLColorToUint32(*forecol), SDLColorToUint32(*backcol));
            }
            free(unicode_text);
        }
        break;
        default:
        text = NULL; /* This shouldn't happen */
        break;
    }
    if ( text == NULL ) {
        fprintf(stderr, "Couldn't render text: %s\n", SDL_GetError());
        TTF_CloseFont(font);
        cleanup(2);
    }
    scene.messageRect.x = (WIDTH - fn_w(text))/2;
    scene.messageRect.y = (HEIGHT - fn_h(text))/2;
    scene.messageRect.w = fn_w(text);
    scene.messageRect.h = fn_h(text);
    SDL_Surface *sdl_text = fn_to_sdl_surface(text);
    printf("surf is here %s\n", __func__);
            SDL_Palette *palette = sdl_text->format->palette;
        Uint32 fg = SDLColorToUint32(*forecol);
        Uint32 bg = SDLColorToUint32(*backcol);
    int rdiff = cl_r(fg) - cl_r(bg);
    int gdiff = cl_g(fg) - cl_g(bg);
    int bdiff = cl_b(fg) - cl_b(bg);

    for ( int index = 0; index < 256; ++index ) {
        palette->colors[index].r = cl_r(bg) + (index*rdiff) / (255);
        palette->colors[index].g = cl_g(bg) + (index*gdiff) / (255);
        palette->colors[index].b = cl_b(bg) + (index*bdiff) / (255);
    }
    scene.message = SDL_CreateTextureFromSurface(renderer, sdl_text);
    printf("Font is generally %d big, and string is %hd big\n",
                        TTF_FontHeight(font), (short)(fn_h(text)));

    draw_scene(renderer, &scene);

    /* Wait for a keystroke, and blit text on mouse press */
    done = 0;
    while ( ! done ) {
        if ( SDL_WaitEvent(&event) < 0 ) {
            fprintf(stderr, "SDL_PullEvent() error: %s\n",
                                SDL_GetError());
            done = 1;
            continue;
        }
        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
                scene.messageRect.x = event.button.x - fn_w(text)/2;
                scene.messageRect.y = event.button.y - fn_h(text)/2;
                scene.messageRect.w = fn_w(text);
                scene.messageRect.h = fn_h(text);
                draw_scene(renderer, &scene);
                break;

            case SDL_KEYDOWN:
            case SDL_QUIT:
                done = 1;
                break;
            default:
                break;
        }
    }
    SDL_FreeSurface(sdl_text);
    free(text);
    TTF_CloseFont(font);
    SDL_DestroyTexture(scene.caption);
    SDL_DestroyTexture(scene.message);
    cleanup(0);

    /* Not reached, but fixes compiler warnings */
    return 0;
}

/* vi: set ts=4 sw=4 expandtab: */
