#ifndef RENDERER_H
#define RENDERER_H

#ifndef CORE_PATH
#define CORE_PATH "../code/core_layer/"
#endif

#define RESOURCE_PATH Glue(CORE_PATH, "res/") 

#define CORE_RESOURCE(str) Str8Lit(Glue(RESOURCE_PATH, str))

typedef enum R_IconIndex
{
	R_IconIndex_Null,

	R_IconIndex_Check,
	R_IconIndex_Cogwheel,
	R_IconIndex_Angle_Down,
	R_IconIndex_Angle_Left,
	R_IconIndex_Angle_Right,
	R_IconIndex_Angle_Up,
	R_IconIndex_Play,
	R_IconIndex_Stop,
	R_IconIndex_Pause,
	R_IconIndex_ToEnd,
	R_IconIndex_ToEndAlt,
	R_IconIndex_ToStart,
	R_IconIndex_ToStartAlt,
	R_IconIndex_SkipForward,
	R_IconIndex_SkipBackward,
	R_IconIndex_Cross,

	R_IconIndex_COUNT,
} R_IconIndex;

typedef struct R_Handle
{
	U64 a;
	U64 b;
} R_Handle;

typedef struct R_Texture
{
	Vec2S32 dim;
	Vec2F32 src_p0;
	Vec2F32 src_p1;
	R_Handle handle;
} R_Texture;

typedef struct Rect
{
	Vec2F32 min;
	Vec2F32 max;
	Vec2F32 min_uv;
	Vec2F32 max_uv;
	// NOTE(hampus): Top left, top right, bottom left, bottom right
	Vec4F32 color[4];
	// NOTE(hampus): Top left, top right, bottom left, bottom right
	Vec4F32 corner_radius;
	F32 edge_softness;
	F32 omit_texture;
	F32 is_text;
	F32 border_thickness;
} Rect;

typedef struct LineVertex
{
	Vec2F32 inst_pos;
	Vec2F32 dir;
	F32 thickness;
	Vec4F32 color;
} LineVertex;

typedef struct R_RectParams
{
	union
	{
		Vec4F32 color;

		struct 
		{
			Vec4F32 colors[2];
		};
	};
	B32 gradient;
	R_Texture texture;
	B32 text;
	union
	{
		Vec4F32 corner_radius;
		struct
		{
			F32 r00, r10, r01, r11;
		};
	};
	F32 edge_softness;
	F32 border_thickness;
} R_RectParams;

typedef enum Batch2DInstKind
{
	Batch2DInstKind_Line,
	Batch2DInstKind_Rect,

	Batch2DInstKind_COUNT
} Batch2DInstKind;

typedef struct Batch2D
{
	// TODO(hampus): Refactor: Make this dynamic
	U8 data[4096 * sizeof(Rect)];
	U32 inst_count;
	R_Texture tex;
	RectF32 clip_rect;
	Batch2DInstKind inst_kind;
} Batch2D;

typedef struct Batch2DNode
{
	struct Batch2DNode *next;
	struct Batch2DNode *prev;
	Batch2D *batch;
} Batch2DNode;

typedef struct Batch2DList
{
	Batch2DNode *first;
	Batch2DNode *last;
} Batch2DList;

typedef struct RenderData
{
	Batch2DList *batch_list;
} RenderData;

typedef struct ClipRectNode
{
	struct ClipRectNode *next;
	struct ClipRectNode *prev;

	RectF32 rect;
} ClipRectNode;

typedef struct ClipRectList
{
	ClipRectNode *first;
	ClipRectNode *last;
} ClipRectList, ClipRectStack;

#define GPU_LOAD_TEXTURE(name) R_Handle name(void *data, S32 width, S32 height);
typedef GPU_LOAD_TEXTURE(GPULoadTextureProc);

typedef struct R_Glyph
{
	R_Texture texture;
	S32 advance;
	Vec2S32 size;
	Vec2S32 bearing;
} R_Glyph;

typedef struct R_LoadedBitmap
{
	Vec2S32 dim;
	void *data;
} R_LoadedBitmap;

typedef struct R_State
{
	MemoryArena *arena;

	MemoryArena permanent_arena;

	RenderData render_data;

	R_Texture white_texture;

	ClipRectStack clip_rect_stack;

	struct R_FontAtlas *font_atlas;

	GPULoadTextureProc *GPULoadTexture;

	struct R_FontSizeCollection *fonts[128];
		
	Vec2S32 render_dim;
} R_State;

#define R_PushRect(min, max, ...) R_PushRect_(min, max, (R_RectParams){.texture = r_state->white_texture, .color = V4(1.0f, 1.0f, 1.0f, 1.0f), __VA_ARGS__})
#define R_PushRectGradient(min, max, c0, c1, ...) R_PushRect_(min, max, (R_RectParams){.texture = r_state->white_texture, .gradient = true, .colors[0] = c0, .colors[1] = c1, __VA_ARGS__})
#define R_PushCircle(min, r, ...) R_PushCircle_(min, r, (R_RectParams){.texture = r_state->white_texture, .color = V4(1.0f, 1.0f, 1.0f, 1.0f), __VA_ARGS__})

#endif