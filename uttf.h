/*
  SDL_ttf:  A companion library to SDL for working with TrueType (tm) fonts
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

/* This library is a wrapper around the excellent FreeType 2.0 library,
   available at:
    http://www.freetype.org/
*/

#ifndef _UTTF_H
#define _UTTF_H

#define DECLSPEC __attribute__ ((visibility("default")))
#define SDLCALL

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* ZERO WIDTH NO-BREAKSPACE (Unicode byte order mark) */
#define UNICODE_BOM_NATIVE  0xFEFF
#define UNICODE_BOM_SWAPPED 0xFFFE

#include <stdint.h>

#define Uint8 uint8_t
#define Uint16 uint16_t
#define Uint32 uint32_t
#define Sint32 int32_t
#define Sint64 int64_t
#define fn_w(ptr) (((uint16_t*)ptr)[0])
#define fn_h(ptr) (((uint16_t*)ptr)[1])
#define fn_d(ptr) (((uint16_t*)ptr)[2])
#define fn_p(ptr) (((uint16_t*)ptr)[3])
#define fn_set_w(ptr, w) { ((uint16_t*)ptr)[0] = w; }
#define fn_set_h(ptr, h) { ((uint16_t*)ptr)[1] = h; }
#define fn_set_d(ptr, d) { ((uint16_t*)ptr)[2] = d; }
#define fn_set_p(ptr, p) { ((uint16_t*)ptr)[3] = p; }
#define fn_to_sdl_surface(ptr) SDL_CreateRGBSurfaceFrom((void*)((Uint8*)((Uint8*)ptr + 8)), fn_w(ptr), fn_h(ptr), fn_d(ptr), fn_p(ptr), 0,0,0,0 )
#define cl_g(color) (((color & 0x0000FF00) >> 8) & 0xFF)
#define cl_r(color) (((color & 0x00FF0000) >> 16) & 0xFF)
#define cl_b(color) (((color & 0x000000FF) & 0xFF))
#define cl_a(color) (((color & 0xFF000000) >> 24) & 0xFF)


//0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

/* This function tells the library whether UNICODE text is generally
   byteswapped.  A UNICODE BOM character in a string will override
   this setting for the remainder of that string.
*/
extern DECLSPEC void SDLCALL TTF_ByteSwappedUNICODE(int swapped);

/* The internal structure containing font information */
typedef struct _TTF_Font TTF_Font;

/* Initialize the TTF engine - returns 0 if successful, -1 on error */
extern DECLSPEC int SDLCALL TTF_Init(void);

extern DECLSPEC char* SDLCALL TTF_GetError(void);

/* Open a font file and create a font of the specified point size.
 * Some .fon fonts will have several sizes embedded in the file, so the
 * point size becomes the index of choosing which size.  If the value
 * is too high, the last indexed size will be the default. */
extern DECLSPEC TTF_Font * SDLCALL TTF_OpenFont(const char *file, int ptsize);
extern DECLSPEC TTF_Font * SDLCALL TTF_OpenFontIndex(const char *file, int ptsize, long index);
extern DECLSPEC TTF_Font * SDLCALL TTF_OpenFontRW(FILE *src, int freesrc, int ptsize);
extern DECLSPEC TTF_Font * SDLCALL TTF_OpenFontIndexRW(FILE *src, int freesrc, int ptsize, long index);

/* Set and retrieve the font style */
#define TTF_STYLE_NORMAL        0x00
#define TTF_STYLE_BOLD          0x01
#define TTF_STYLE_ITALIC        0x02
#define TTF_STYLE_UNDERLINE     0x04
#define TTF_STYLE_STRIKETHROUGH 0x08
extern DECLSPEC int SDLCALL TTF_GetFontStyle(const TTF_Font *font);
extern DECLSPEC void SDLCALL TTF_SetFontStyle(TTF_Font *font, int style);
extern DECLSPEC int SDLCALL TTF_GetFontOutline(const TTF_Font *font);
extern DECLSPEC void SDLCALL TTF_SetFontOutline(TTF_Font *font, int outline);

/* Set and retrieve FreeType hinter settings */
#define TTF_HINTING_NORMAL    0
#define TTF_HINTING_LIGHT     1
#define TTF_HINTING_MONO      2
#define TTF_HINTING_NONE      3
extern DECLSPEC int SDLCALL TTF_GetFontHinting(const TTF_Font *font);
extern DECLSPEC void SDLCALL TTF_SetFontHinting(TTF_Font *font, int hinting);

/* Get the total height of the font - usually equal to point size */
extern DECLSPEC int SDLCALL TTF_FontHeight(const TTF_Font *font);

/* Get the offset from the baseline to the top of the font
   This is a positive value, relative to the baseline.
 */
extern DECLSPEC int SDLCALL TTF_FontAscent(const TTF_Font *font);

/* Get the offset from the baseline to the bottom of the font
   This is a negative value, relative to the baseline.
 */
extern DECLSPEC int SDLCALL TTF_FontDescent(const TTF_Font *font);

/* Get the recommended spacing between lines of text for this font */
extern DECLSPEC int SDLCALL TTF_FontLineSkip(const TTF_Font *font);

/* Get/Set whether or not kerning is allowed for this font */
extern DECLSPEC int SDLCALL TTF_GetFontKerning(const TTF_Font *font);
extern DECLSPEC void SDLCALL TTF_SetFontKerning(TTF_Font *font, int allowed);

/* Get the number of faces of the font */
extern DECLSPEC long SDLCALL TTF_FontFaces(const TTF_Font *font);

/* Get the font face attributes, if any */
extern DECLSPEC int SDLCALL TTF_FontFaceIsFixedWidth(const TTF_Font *font);
extern DECLSPEC char * SDLCALL TTF_FontFaceFamilyName(const TTF_Font *font);
extern DECLSPEC char * SDLCALL TTF_FontFaceStyleName(const TTF_Font *font);

/* Check wether a glyph is provided by the font or not */
extern DECLSPEC int SDLCALL TTF_GlyphIsProvided(const TTF_Font *font, Uint16 ch);

/* Get the metrics (dimensions) of a glyph
   To understand what these metrics mean, here is a useful link:
    http://freetype.sourceforge.net/freetype2/docs/tutorial/step2.html
 */
extern DECLSPEC int SDLCALL TTF_GlyphMetrics(TTF_Font *font, Uint16 ch,
                     int *minx, int *maxx,
                                     int *miny, int *maxy, int *advance);

/* Get the dimensions of a rendered string of text */
extern DECLSPEC int SDLCALL TTF_SizeText(TTF_Font *font, const char *text, int *w, int *h);
extern DECLSPEC int SDLCALL TTF_SizeUTF8(TTF_Font *font, const char *text, int *w, int *h);
extern DECLSPEC int SDLCALL TTF_SizeUNICODE(TTF_Font *font, const Uint16 *text, int *w, int *h);

/* Create an 8-bit palettized surface and render the given text at
   fast quality with the given font and color.  The 0 pixel is the
   colorkey, giving a transparent background, and the 1 pixel is set
   to the text color.
   This function returns the new surface, or NULL if there was an error.
*/
extern DECLSPEC Uint8 * SDLCALL TTF_RenderText_Solid(TTF_Font *font,
                const char *text, Uint32 fg);
extern DECLSPEC Uint8 * SDLCALL TTF_RenderUTF8_Solid(TTF_Font *font,
                const char *text, Uint32 fg);
extern DECLSPEC Uint8 * SDLCALL TTF_RenderUNICODE_Solid(TTF_Font *font,
                const Uint16 *text, Uint32 fg);

/* Create an 8-bit palettized surface and render the given glyph at
   fast quality with the given font and color.  The 0 pixel is the
   colorkey, giving a transparent background, and the 1 pixel is set
   to the text color.  The glyph is rendered without any padding or
   centering in the X direction, and aligned normally in the Y direction.
   This function returns the new surface, or NULL if there was an error.
*/
extern DECLSPEC Uint8 * SDLCALL TTF_RenderGlyph_Solid(TTF_Font *font,
                    Uint16 ch, Uint32 fg);

/* Create an 8-bit palettized surface and render the given text at
   high quality with the given font and colors.  The 0 pixel is background,
   while other pixels have varying degrees of the foreground color.
   This function returns the new surface, or NULL if there was an error.
*/
extern DECLSPEC Uint8 * SDLCALL TTF_RenderText_Shaded(TTF_Font *font,
                const char *text, Uint32 fg, Uint32 bg);
extern DECLSPEC Uint8 * SDLCALL TTF_RenderUTF8_Shaded(TTF_Font *font,
                const char *text, Uint32 fg, Uint32 bg);
extern DECLSPEC Uint8 * SDLCALL TTF_RenderUNICODE_Shaded(TTF_Font *font,
                const Uint16 *text, Uint32 fg, Uint32 bg);

/* Create an 8-bit palettized surface and render the given glyph at
   high quality with the given font and colors.  The 0 pixel is background,
   while other pixels have varying degrees of the foreground color.
   The glyph is rendered without any padding or centering in the X
   direction, and aligned normally in the Y direction.
   This function returns the new surface, or NULL if there was an error.
*/
extern DECLSPEC Uint8 * SDLCALL TTF_RenderGlyph_Shaded(TTF_Font *font,
                Uint16 ch, Uint32 fg, Uint32 bg);

/* Create a 32-bit ARGB surface and render the given text at high quality,
   using alpha blending to dither the font with the given color.
   This function returns the new surface, or NULL if there was an error.
*/
extern DECLSPEC Uint8 * SDLCALL TTF_RenderText_Blended(TTF_Font *font,
                const char *text, Uint32 fg);
extern DECLSPEC Uint8 * SDLCALL TTF_RenderUTF8_Blended(TTF_Font *font,
                const char *text, Uint32 fg);
extern DECLSPEC Uint8 * SDLCALL TTF_RenderUNICODE_Blended(TTF_Font *font,
                const Uint16 *text, Uint32 fg);


/* Create a 32-bit ARGB surface and render the given text at high quality,
   using alpha blending to dither the font with the given color.
   Text is wrapped to multiple lines on line endings and on word boundaries
   if it extends beyond wrapLength in pixels.
   This function returns the new surface, or NULL if there was an error.
*/
extern DECLSPEC Uint8 * SDLCALL TTF_RenderText_Blended_Wrapped(TTF_Font *font,
                const char *text, Uint32 fg, Uint32 wrapLength);
extern DECLSPEC Uint8 * SDLCALL TTF_RenderUTF8_Blended_Wrapped(TTF_Font *font,
                const char *text, Uint32 fg, Uint32 wrapLength);
extern DECLSPEC Uint8 * SDLCALL TTF_RenderUNICODE_Blended_Wrapped(TTF_Font *font,
                const Uint16 *text, Uint32 fg, Uint32 wrapLength);

/* Create a 32-bit ARGB surface and render the given glyph at high quality,
   using alpha blending to dither the font with the given color.
   The glyph is rendered without any padding or centering in the X
   direction, and aligned normally in the Y direction.
   This function returns the new surface, or NULL if there was an error.
*/
extern DECLSPEC Uint8 * SDLCALL TTF_RenderGlyph_Blended(TTF_Font *font,
                        Uint16 ch, Uint32 fg);

/* For compatibility with previous versions, here are the old functions */
#define TTF_RenderText(font, text, fg, bg)  \
    TTF_RenderText_Shaded(font, text, fg, bg)
#define TTF_RenderUTF8(font, text, fg, bg)  \
    TTF_RenderUTF8_Shaded(font, text, fg, bg)
#define TTF_RenderUNICODE(font, text, fg, bg)   \
    TTF_RenderUNICODE_Shaded(font, text, fg, bg)

/* Close an opened font file */
extern DECLSPEC void SDLCALL TTF_CloseFont(TTF_Font *font);

/* De-initialize the TTF engine */
extern DECLSPEC void SDLCALL TTF_Quit(void);

/* Check if the TTF engine is initialized */
extern DECLSPEC int SDLCALL TTF_WasInit(void);

/* Get the kerning size of two glyphs */
extern DECLSPEC int TTF_GetFontKerningSize(TTF_Font *font, int prev_index, int index);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#endif /* _UTTF_H */
