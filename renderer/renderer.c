#pragma comment(lib, "freetype.lib")

#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb_image.h"

// NOTE(hampus): freetype has variables named "internal"
#ifdef internal
#undef internal
#endif

#include <freetype/freetype.h>

#define internal static

global R_State *r_state;

internal void
R_Init()
{
#if 0
	U32 white = 0xffffffff;
	r_state->white_texture.handle = D3D11_LoadTexture(&white, 1, 1);
	r_state->white_texture.dim.height = 1;
	r_state->white_texture.dim.width = 1;
#endif
}

internal R_Texture
R_LoadTexture(String8 path)
{
	R_Texture result = {0};

	S32 channels;
	void *data = stbi_load((const char *)path.str, &result.dim.width, &result.dim.height, &channels, 0);

	result.handle = r_state->GPULoadTexture(data, result.dim.width, result.dim.height);

	stbi_image_free(data);

	return(result);
}

internal B32
R_HandleMatch(R_Handle a, R_Handle b)
{
	return a.a == b.a && a.b == b.b;
}

internal B32
R_RectF32Match(RectF32 a, RectF32 b)
{
	return (a.x0 == b.x0 && a.x1 == b.x1 && a.y0 == b.y0 && a.y1 == b.y1);
}

internal void
R_PushRect_(Vec2F32 min, Vec2F32 max, R_RectParams params)
{
	RenderData *render_data = &r_state->render_data;
	Batch2DNode *batch_node = render_data->batch_list->last;

	R_Texture texture = params.texture;

	B32 rect_match = false;
	if (batch_node)
	{
		rect_match = R_RectF32Match(batch_node->batch->clip_rect, r_state->clip_rect_stack.first->rect);

		if (batch_node->batch->inst_count == 4096)
		{
			batch_node = 0;
		}
		else if (batch_node->batch->inst_kind == Batch2DInstKind_Line)
		{
			batch_node = 0;
		}
	}

	if (!batch_node ||
	    (!R_HandleMatch(texture.handle, batch_node->batch->tex.handle) &&
	    !R_HandleMatch(texture.handle, r_state->white_texture.handle)) ||
	    !rect_match)
	{
		// NOTE(hampus): If the previus batch just contained a white texture,
		// we'll just replace it with our texture instead
		B32 previous_was_white_texture = false;
		if (batch_node)
		{
			if (R_HandleMatch(batch_node->batch->tex.handle, r_state->white_texture.handle) && rect_match)
			{
				previous_was_white_texture = true;
			}
		}

		if (!previous_was_white_texture)
		{
			batch_node = PushStruct(r_state->arena, Batch2DNode);
			batch_node->batch = PushStruct(r_state->arena, Batch2D);
			batch_node->batch->inst_count = 0;
			batch_node->batch->clip_rect = r_state->clip_rect_stack.first->rect;

			DLL_PushBack(render_data->batch_list->first, render_data->batch_list->last, batch_node);

			batch_node->batch->inst_kind = Batch2DInstKind_Rect;
		}

		batch_node->batch->tex = texture;
	}

	Batch2D *batch = batch_node->batch;

	Rect *rect = (Rect *)batch->data + batch->inst_count;

	rect->min = V2(roundf(min.x), roundf(min.y));
	rect->max = V2(roundf(max.x), roundf(max.y));
	if (params.gradient)
	{
		rect->color[0] = params.colors[0];
		rect->color[1] = params.colors[0];
		rect->color[2] = params.colors[1];
		rect->color[3] = params.colors[1];
	}
	else
	{
		rect->color[0] = params.color;
		rect->color[1] = params.color;
		rect->color[2] = params.color;
		rect->color[3] = params.color;
	}
	rect->min_uv = params.texture.src_p0;
	rect->max_uv = params.texture.src_p1;
	rect->corner_radius = params.corner_radius;
	rect->omit_texture = R_HandleMatch(params.texture.handle, r_state->white_texture.handle) ? 1.0f : 0.0f;
	rect->is_text = (F32)!!params.text;
	rect->edge_softness = params.edge_softness;
	rect->border_thickness = params.border_thickness;

	batch->inst_count++;
}

internal void
R_PushLine(Vec2F32 p0, Vec2F32 p1, F32 thickness, Vec4F32 color)
{
	RenderData *render_data = &r_state->render_data;
	Batch2DNode *batch_node = render_data->batch_list->last;

	B32 rect_match = true;
	if (batch_node)
	{
		if (batch_node->batch->inst_kind == Batch2DInstKind_Rect)
		{
			batch_node = 0;
		}
		else if (batch_node->batch->inst_kind == Batch2DInstKind_Line)
		{
			if (batch_node->batch->inst_count == 4096)
			{
				batch_node = 0;
			}
			else
			{
				rect_match = R_RectF32Match(batch_node->batch->clip_rect, r_state->clip_rect_stack.first->rect);
			}

		}
		else
		{
			InvalidCodePath;
		}
	}

	if (!batch_node || !rect_match)
	{
		batch_node = PushStruct(r_state->arena, Batch2DNode);
		batch_node->batch = PushStruct(r_state->arena, Batch2D);
		batch_node->batch->inst_count = 0;
		batch_node->batch->clip_rect = r_state->clip_rect_stack.first->rect;

		DLL_PushBack(render_data->batch_list->first, render_data->batch_list->last, batch_node);

		batch_node->batch->inst_kind = Batch2DInstKind_Line;
	}

	Batch2D *batch = batch_node->batch;

	LineVertex *line = (LineVertex *)batch->data + batch->inst_count;

	Vec2F32 inst_pos = V2DivF32(V2AddV2(p0, p1), 2);
	Vec2F32 dir = V2SubV2(p1, p0);

	line->inst_pos = inst_pos;
	line->dir = dir;
	line->thickness = thickness;
	line->color = color;

	batch->inst_count++;
}

internal void 
R_LoadGlyph(MemoryArena *arena, R_FontAtlas *atlas, U32 glyph_index, FT_Face face, R_Font *font)
{
	R_Glyph *glyph = font->glyphs + glyph_index;
	if (glyph_index >= 0xE800)
	{
		glyph = font->icons + (glyph_index - 0xE800);
	}
	else
	{
		glyph = font->glyphs + glyph_index;
	}

	Assert(glyph->advance == 0);
	FT_Load_Char(face, glyph_index, FT_LOAD_RENDER | FT_LOAD_TARGET_LCD);

	FT_Render_Glyph(face->glyph, FT_RENDER_MODE_LCD);

	R_LoadedBitmap glyph_bitmap = {0};
	glyph_bitmap.data = face->glyph->bitmap.buffer;

	// NOTE(hampus): Divide by 3 for LCD
	S32 width = face->glyph->bitmap.width / 3;
	// S32 width = face->glyph->bitmap.width;
	S32 height = face->glyph->bitmap.rows;

	glyph_bitmap.dim.width = width;
	glyph_bitmap.dim.height = height;

	U8 *new_teture_data = PushArrayNoZero(arena, glyph_bitmap.dim.width * glyph_bitmap.dim.height * 4, U8);
	U8 *src = glyph_bitmap.data;
	U8 *dst = new_teture_data;

	for (S32 y = 0; y < glyph_bitmap.dim.height; ++y)
	{
		U8 *dst_row = dst;
		U8 *src_row = src;
		U8 *test2 = (U8 *)src_row;
		for (S32 x = 0; x < glyph_bitmap.dim.width; ++x)
		{
			U8 r = *test2++;
			U8 g = *test2++;
			U8 b = *test2++;
			*dst_row++ = r;
			*dst_row++ = g;
			*dst_row++ = b;
			*dst_row++ = 0xff;
		}

		dst += (S32)(glyph_bitmap.dim.width * 4);

		// NOTE(hampus): Freetype actually adds padding
		// so the pitch is the correct width to increment
		// by. 
		src += (S32)(face->glyph->bitmap.pitch);
	}

	// TODO(hampus): Memory leak here
	glyph_bitmap.data = new_teture_data;

	glyph->size = V2S(width, height);
	glyph->bearing = V2S(face->glyph->bitmap_left, face->glyph->bitmap_top);
	glyph->advance = face->glyph->advance.x >> 6;
	if (glyph->size.y > font->max_height)
	{
		font->max_height = (F32)glyph->size.y;
	}

	R_FontAtlasRegion atlas_region = R_FontAtlasRegionAlloc(arena, atlas, glyph_bitmap.dim);
	glyph->texture = R_FillFontAtlasRegionWithBitmap(atlas, atlas_region, &glyph_bitmap);
}

internal void
R_LoadFont(MemoryArena *arena, R_Font *font, R_FontAtlas *atlas, String8 font_path, String8 icon_path, S32 size)
{
	// TODO(hampus): This is temporary for now. Rewrite

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		InvalidCodePath;
		return;
	}

	FT_Face face;
	if (FT_New_Face(ft, (const char *)font_path.str, 0, &face))
	{
		InvalidCodePath;
		return;
	}

	FT_Set_Pixel_Sizes(face, 0, size);
	font->height = size;

	font->max_ascent = ((S32)(face->ascender * (face->size->metrics.y_scale / 65536.0f))) >> 6;
	font->max_descent = ((S32)Abs((F32)face->descender * ((F32)face->size->metrics.y_scale / 65536.0f))) >> 6;

	for (U32 glyph_index = 32;
		glyph_index < 127;
		++glyph_index)
	{
		R_LoadGlyph(arena, atlas, glyph_index, face, font);
	}

	FT_Done_Face(face);

	if (FT_New_Face(ft, (const char *)icon_path.str, 0, &face))
	{
		InvalidCodePath;
		return;
	}

	FT_Set_Pixel_Sizes(face, 0, size);

	for (U32 icon_index = 1;
		icon_index < R_IconIndex_COUNT;
		++icon_index)
	{
		R_LoadGlyph(arena, atlas, 0xE800 + (icon_index-1), face, font);
	}

	atlas->texture.handle = r_state->GPULoadTexture(atlas->data, atlas->dim.x, atlas->dim.y);

	for (U32 glyph_index = 32;
		glyph_index < 127;
		++glyph_index)
	{
		R_Glyph *glyph = font->glyphs + glyph_index;
		glyph->texture.handle = atlas->texture.handle;
	}

	for (U32 icon_index = 1;
		icon_index < R_IconIndex_COUNT;
		++icon_index)
	{
		R_Glyph *glyph = font->icons + (icon_index-1);
		glyph->texture.handle = atlas->texture.handle;
	}
}

internal void
R_PushCircle_(Vec2F32 p, F32 radius, R_RectParams params)
{
	F32 cr = radius / 2;
	R_RectParams params2 = params;
	params2.r00 = cr;
	params2.r01 = cr;
	params2.r10 = cr;
	params2.r11 = cr;
	R_PushRect_(p, V2AddV2(p, V2(radius, radius)), params2);
}

internal void
R_PushGlyph(Vec2F32 pos, R_Font *font, R_Glyph *glyph, Vec4F32 color)
{
	F32 xpos = pos.x + glyph->bearing.x;
	F32 ypos = pos.y + (-glyph->bearing.y) + (font->max_ascent);

	F32 width = (F32)glyph->size.x;
	F32 height = (F32)glyph->size.y;

	R_PushRect(V2(xpos, ypos),
	           V2(xpos + width,
	              ypos + height),
	              .texture = glyph->texture,
	              .color = color,
	              .text = true);

}

internal void
R_PushGlyphIndex(Vec2F32 pos, R_Font *font, U32 index, Vec4F32 color)
{
	R_Glyph *glyph = index >= 0xE800 ? font->icons + ((index-1) - 0xE800) : font->glyphs + index;
	R_PushGlyph(pos, font, glyph, color);
}

internal R_Font *
R_GetFontFromKey(R_FontKey font_key, S32 height)
{
	U64 slot_index = font_key.key % ArrayCount(r_state->fonts);
	R_FontSizeCollection *font_collection = 0;
	if (!r_state->fonts[slot_index])
	{
		r_state->fonts[slot_index] = PushStruct(&r_state->permanent_arena, R_FontSizeCollection);
		r_state->fonts[slot_index]->key = font_key;
	}

	font_collection = r_state->fonts[slot_index];

	Assert(height >= 0);
	Assert(height < ArrayCount(font_collection->fonts));

	Assert(font_collection->key.key == font_key.key);

	if(!font_collection->fonts[height])
	{
		font_collection->fonts[height] = PushStruct(&r_state->permanent_arena, R_Font);
		R_LoadFont(&r_state->permanent_arena, 
		           font_collection->fonts[height], 
		           r_state->font_atlas, 
		           font_key.font, 
		           CORE_RESOURCE("font/icon/fontello.ttf"), 
		           height);
	}

	R_Font *font = font_collection->fonts[height];

	Assert(font);

	return(font);
}

internal void
R_PushText(Vec2F32 pos, R_FontKey font_key, S32 height, String8 text, Vec4F32 color)
{
	R_Font *font = R_GetFontFromKey(font_key, height);

	for (U64 i = 0; i < text.size; ++i)
	{
		R_Glyph *glyph = font->glyphs + text.str[i];
		R_PushGlyph(pos, font, glyph, color);
		pos.x += (glyph->advance);
	}
}

internal void
R_PushClipRect(RectF32 rect)
{
	Assert(rect.x1 >= rect.x0);
	Assert(rect.y1 >= rect.y0);

	ClipRectNode *node = PushStruct(r_state->arena, ClipRectNode);
	node->rect = rect;
	StackPush(r_state->clip_rect_stack.first, node);
}

internal void
R_PopClipRect()
{
	StackPop(r_state->clip_rect_stack.first);
}

internal RectF32
R_MakeRectF32(F32 x0, F32 y0, F32 x1, F32 y1)
{
	RectF32 result = {x0, y0, x1, y1};

	return result;
}

internal RectS32
R_MakeRectS32(S32 x0, S32 y0, S32 x1, S32 y1)
{
	RectS32 result = {x0, y0, x1, y1};

	return result;
}

internal void
R_Begin(MemoryArena *arena)
{
	r_state->arena = arena;

	r_state->render_data.batch_list = PushStruct(r_state->arena, Batch2DList);

	r_state->render_dim.width = 2560;
	r_state->render_dim.height = 1440;

	R_PushClipRect(R_MakeRectF32(0, 0, 2560, 1440));
}

internal void
R_End()
{
	R_PopClipRect();
}

internal Vec2F32
R_GetGlyphDim(R_Font *font, R_Glyph *glyph)
{
	Vec2F32 result;

	result.x = (F32)glyph->size.x;
	result.y = (F32)glyph->size.y;

	return result;
}

internal Vec2F32
R_GetTextDim(R_Font *font, String8 text)
{
	Vec2F32 result = {0};

	for (U64 i = 0; i < text.size; ++i)
	{
		result.x += font->glyphs[text.str[i]].advance;
	}
	result.y = (F32)(font->max_height);

	return(result);
}

internal B32
R_PointInsideRect(Vec2F32 p, RectF32 rect)
{
	B32 result = p.x >= rect.x0 && p.x < rect.x1 && p.y >= rect.y0 && p.y < rect.y1;

	return result;
}

internal RectF32
R_IntersectRectF32(RectF32 dest, RectF32 src)
{
	RectF32 result = {0};

	result.x0 = Clamp(dest.x0, src.x0, dest.x1);
	result.x1 = Clamp(dest.x0, src.x1, dest.x1);
	result.y0 = Clamp(dest.y0, src.y0, dest.y1);
	result.y1 = Clamp(dest.y0, src.y1, dest.y1);

	return(result);
}

internal void
R_SelectState(R_State *state)
{
	r_state = state;
}