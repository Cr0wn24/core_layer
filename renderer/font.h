#ifndef FONT_H
#define FONT_H

typedef struct R_FontAtlasRegion
{
	RectS32 rect;
} R_FontAtlasRegion;

typedef struct R_FreeFontAtlasRegion
{
	struct R_FreeFontAtlasRegion *next;
	struct R_FreeFontAtlasRegion *prev;

	R_FontAtlasRegion region;
} R_FreeFontAtlasRegion;

typedef struct R_FontAtlas
{
	R_FreeFontAtlasRegion *first_free_region;
	R_FreeFontAtlasRegion *last_free_region;

	R_FreeFontAtlasRegion *first_used_region;
	R_FreeFontAtlasRegion *last_used_region;

	Vec2S32 dim;
	void *data;
	R_Texture texture;
} R_FontAtlas;

typedef struct R_FontKey
{
	U64 key;
	String8 font;
} R_FontKey;

typedef struct R_Font
{
	R_Glyph glyphs[128];
	R_Glyph icons[R_IconIndex_COUNT];
	F32 max_height;
	F32 max_width;
	S32 max_ascent;
	S32 max_descent;
	S32 height;
} R_Font;

typedef struct R_FontSizeCollection
{
	R_Font *fonts[512];
	R_FontKey key;
} R_FontSizeCollection;

internal R_FontAtlas *		R_FontAtlasMake(MemoryArena *arena, Vec2S32 dim);
internal R_FontAtlasRegion 	R_FontAtlasRegionAlloc(MemoryArena *arena, R_FontAtlas *atlas, Vec2S32 dim);
internal R_Texture 			R_FillFontAtlasRegionWithBitmap(R_FontAtlas *atlas, R_FontAtlasRegion region, R_LoadedBitmap *bitmap);

#endif