internal R_FontAtlas *
R_FontAtlasMake(MemoryArena *arena, Vec2S32 dim)
{
	R_FontAtlas *atlas = PushStruct(arena, R_FontAtlas);
	atlas->dim = dim;
	atlas->data = PushArray(arena, dim.width * dim.height * 4, U8);
	atlas->texture.dim = dim;
	atlas->texture.src_p0 = V2(0, 0);
	atlas->texture.src_p1 = V2(1, 1);
	atlas->texture.handle = r_state->GPULoadTexture(atlas->data, atlas->dim.width, atlas->dim.height);
	R_FreeFontAtlasRegion *first = PushStruct(arena, R_FreeFontAtlasRegion);
	first->region.rect = R_MakeRectS32(0, 0, dim.x, dim.y);
	DLL_PushBack(atlas->first_free_region, atlas->last_free_region, first);

	return(atlas);
}

internal R_FontAtlasRegion
R_FontAtlasRegionAlloc(MemoryArena *arena, R_FontAtlas *atlas, Vec2S32 dim)
{
	Assert(arena);
	Assert(atlas);

	R_FreeFontAtlasRegion *free_region = atlas->first_free_region;

	Vec2S32 region_dim = V2S(free_region->region.rect.x1 - free_region->region.rect.x0, 
	                         free_region->region.rect.y1 - free_region->region.rect.y0);

	B32 found_region = false;

	// Get the next free region that 
	// fits the required size
	while (free_region)
	{
		if (region_dim.width >= dim.width && 
		    region_dim.height >= dim.height)
		{
			// We found a region
			found_region = true;
			break;
		}

		free_region = free_region->next;

		region_dim = V2S(free_region->region.rect.x1 - free_region->region.rect.x0, 
		                 free_region->region.rect.y1 - free_region->region.rect.y0);
	}

	Assert(found_region);

	R_FontAtlasRegion result = free_region->region;

	// See if we can make the region smaller
	// in order to save space

	B32 can_halve_size = region_dim.width >= (dim.width * 2) &&
						 region_dim.height >= (dim.height * 2);

	while (can_halve_size)
	{
		R_FreeFontAtlasRegion *free_regions = PushArray(arena, 4, R_FreeFontAtlasRegion);

		free_regions[0].region.rect = R_MakeRectS32(result.rect.x0, 
		                                            result.rect.y0, 
		                                            result.rect.x0 + region_dim.width/2,
		                                            result.rect.y0 + region_dim.height/2);

		free_regions[1].region.rect = R_MakeRectS32(result.rect.x0 + region_dim.width/2, 
		                                            result.rect.y0, 
		                                            result.rect.x0 + region_dim.width,
		                                            result.rect.y0 + region_dim.height/2);

		free_regions[2].region.rect = R_MakeRectS32(result.rect.x0 + region_dim.width/2, 
		                                            result.rect.y0 + region_dim.height/2, 
		                                            result.rect.x0 + region_dim.width,
		                                            result.rect.y0 + region_dim.height);

		free_regions[3].region.rect = R_MakeRectS32(result.rect.x0, 
		                                            result.rect.y0 + region_dim.height/2, 
		                                            result.rect.x0 + region_dim.width/2,
		                                            result.rect.y0 + region_dim.height);

		free_regions[0].prev = free_region->prev;
		free_regions[0].next = &free_regions[1];

		free_regions[1].prev = &free_regions[0];
		free_regions[1].next = &free_regions[2];

		free_regions[2].prev = &free_regions[1];
		free_regions[2].next = &free_regions[3];

		free_regions[3].prev = &free_regions[2];
		free_regions[3].next = free_region->next;

		if (atlas->first_free_region == free_region)
		{
			atlas->first_free_region = &free_regions[0];
		}

		if (free_region->next)
 		{
			free_region->next->prev = &free_regions[3];
		}

		if (free_region->prev)
		{
			free_region->prev->next= &free_regions[0];
		}

		free_region = &free_regions[0];

		result = free_region->region;

		region_dim = V2S(result.rect.x1 - result.rect.x0, 
		                 result.rect.y1 - result.rect.y0);

		can_halve_size = (region_dim.width >= (dim.width * 2)) && (region_dim.height >= (dim.height * 2));
	}

	result.rect.x0 += (region_dim.x - dim.x) / 2;
	result.rect.y0 += (region_dim.y - dim.y) / 2;

	result.rect.x1 = result.rect.x0 + dim.width;
	result.rect.y1 = result.rect.y0 + dim.height;

	DLL_Remove(atlas->first_free_region, atlas->last_free_region, free_region);

	DLL_PushBack(atlas->first_used_region, atlas->last_used_region, free_region);

	return(result);
}

internal R_Texture
R_FillFontAtlasRegionWithBitmap(R_FontAtlas *atlas, R_FontAtlasRegion region, R_LoadedBitmap *bitmap)
{
	Assert(atlas);
	Assert(bitmap);

	// NOTE(hampus): Copy bitmap into atlas
	U8 *src = bitmap->data;
	U8 *dst = (U8 *)atlas->data + (region.rect.x0 + region.rect.y0 * atlas->dim.width) * 4;
	for (S32 y = 0; y < bitmap->dim.height; ++y)
	{
		U32 *src_row = (U32 *)src;
		U32 *dst_row = (U32 *)dst;
		for (S32 x = 0; x < bitmap->dim.width; ++x)
		{
			*dst_row++ = *src_row++;
		}

		dst += atlas->dim.width * 4;
		src += bitmap->dim.width * 4;
	}

	R_Texture texture;
	texture.dim = bitmap->dim;
	texture.src_p0 = V2((F32)region.rect.x0 / (F32)atlas->dim.x,
	                    (F32)region.rect.y0 / (F32)atlas->dim.y);

	texture.src_p1 = V2((F32)region.rect.x1 / (F32)atlas->dim.x,
	                    (F32)region.rect.y1 / (F32)atlas->dim.y);
	return(texture);
}

internal R_FontKey
R_FontKeyFromString(String8 string)
{
	R_FontKey result = {0};

	for (U64 i = 0; i < string.size; ++i)
	{
		result.key += string.str[i];
	}

	result.font = string;

	return(result);
}

internal void
R_DEBUG_DrawFontAtlas(R_FontAtlas *atlas)
{
	R_PushRect(V2(0, 0), 
	           V2((F32)atlas->dim.width, 
	              (F32)atlas->dim.height),
	              .texture = atlas->texture);

	R_FreeFontAtlasRegion *first_region = atlas->first_used_region;
	while(first_region)
	{
		Vec2F32 p0 = V2((F32)first_region->region.rect.x0, (F32)first_region->region.rect.y0);
		Vec2F32 p1 = V2((F32)first_region->region.rect.x1, (F32)first_region->region.rect.y1);
		R_PushRect(p0, p1, .color = V4(1.0f, 0, 0, 1.0f), .border_thickness = 1);

		first_region = first_region->next;
	}

	first_region = atlas->first_free_region;
	while(first_region)
	{
		Vec2F32 p0 = V2((F32)first_region->region.rect.x0, (F32)first_region->region.rect.y0);
		Vec2F32 p1 = V2((F32)first_region->region.rect.x1, (F32)first_region->region.rect.y1);
		R_PushRect(p0, p1, .color = V4(1.0f, 0, 0, 1.0f), .border_thickness = 1);

		first_region = first_region->next;
	}
}