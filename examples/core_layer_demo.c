// This needs to be set where the core layer is relative to the build folder. See "renderer.h".
// It is needed to properly initialize resources at run time
#define CORE_PATH "../"

#include "core_layer/base/base_inc.h"
#include "core_layer/os/os_win32_inc.h"
#include "core_layer/renderer/r_inc.h"
#include "core_layer/renderer/backends/d3d11/d3d11.h"
#include "core_layer/ui/ui_inc.h"

#include "core_layer/base/base_inc.c"
#include "core_layer/os/os_win32_inc.c"
#include "core_layer/renderer/r_inc.c"
#include "core_layer/renderer/backends/d3d11/d3d11.c"
#include "core_layer/ui/ui_inc.c"

internal void
UITest()
{
	UI_Spacer(UI_Em(0.5f));
	UI_NextSize2(UI_Em(40.0f), UI_SumOfChildren());
	UI_NextChildLayoutAxis(Axis2_X);
	UI_NextRelativePos2(300, 300);
	UI_NextBackgroundColor(ui_state->theme.window_color);
	UI_Box *box2 = UI_BoxMake(UI_BoxFlag_DrawBackground |
							  UI_BoxFlag_DrawBorder |
							  UI_BoxFlag_DrawDropShadow |
							  UI_BoxFlag_FixedX |
							  UI_BoxFlag_FixedY |
							  UI_BoxFlag_AnimateWidth |
							  UI_BoxFlag_AnimateHeight |
							  UI_BoxFlag_Clip,
							  Str8Lit("My box2"));
	UI_Size tree_spacing = UI_Em(0.3f);

	local_persist Vec4F32 color_test = {1.0f, 1.0f, 1.0f, 1.0f};

	UI_Parent(box2)
	{
		UI_Spacer(UI_Em(0.5f));
		UI_NextSize2(UI_SumOfChildren(), UI_SumOfChildren());
		UI_Column()
		{
			UI_Spacer(UI_Em(0.5f));
			UI_NextFontSize(50);
			UI_Text(Str8Lit("Column 1"));
			UI_Spacer(UI_Em(0.2f));

			UI_NextSize2(UI_Em(5.0f), UI_Pixels(1));
			UI_NextBackgroundColor(ui_state->theme.text_color);
			UI_BoxMake(UI_BoxFlag_DrawBackground, Str8Lit(""));
			UI_Spacer(UI_Em(0.5f));

			local_persist F32 test_value = 5.0f;

			UI_Row()
			{
				UI_SliderF32(&test_value, -10, 10, Str8Lit("F32 slider"));
				UI_Spacer(UI_Em(0.5f));
				UI_Text(Str8Lit("F32 Slider"));
			}

			UI_Spacer(UI_Em(0.5f));

			local_persist S32 test_value2 = 5;

			UI_Row()
			{
				UI_SliderS32(&test_value2, -50, 100, Str8Lit("S32 slider"));
				UI_Spacer(UI_Em(0.5f));
				UI_Text(Str8Lit("S32 Slider"));
			}
			UI_Spacer(UI_Em(0.5f));

			UI_PushSize2(UI_Em(12.0f), UI_Em(1.0f));
			UI_Spacer(UI_Em(0.5f));

			UI_Text(Str8Lit("Hello2##7"));

			UI_Spacer(UI_Em(0.5f));
			UI_ButtonF("Button %d###1", 5);
			UI_Spacer(UI_Em(0.5f));

			UI_PopSize2();

			UI_Divider();
			UI_Spacer(UI_Em(0.2f));
			UI_Text(Str8Lit("Settings"));
			UI_Spacer(UI_Em(0.2f));
			UI_Divider();

			UI_Spacer(UI_Em(0.5f));
			UI_Row()
			{
				UI_Check(Str8Lit("Show debug lines"), &ui_state->show_debug_lines);
				UI_Spacer(UI_Em(0.5f));
				UI_Text(Str8Lit("Show debug lines"));
			}
			UI_Spacer(UI_Em(0.5f));

			local_persist B32 check_value2 = false;
			UI_Row()
			{
				UI_Check(Str8Lit("Test check##11"), &check_value2);
				UI_Spacer(UI_Em(0.5f));
				UI_Text(Str8Lit("Test check##11"));
			}
			UI_Spacer(UI_Em(0.5f));

			UI_ColorPicker(&color_test, Str8Lit("Color picker"));

			UI_Spacer(UI_Em(0.5f));
			UI_ColorPicker(&color_test, Str8Lit("Color picker##1"));
			UI_Spacer(UI_Em(0.5f));
			local_persist B32 b0 = false;
			local_persist B32 b1 = false;
			local_persist B32 b2 = false;
			local_persist B32 b3 = false;

			UI_RadioData radio_data[] =
			{
				{ &b0, Str8Lit("Option 1") },
				{ &b1, Str8Lit("Option 2") },
				{ &b2, Str8Lit("Option 3") },
				{ &b3, Str8Lit("Option 4") },
			};

			UI_Radio(radio_data, 4, Str8Lit("Radio"));
		}

		UI_Spacer(UI_Em(1.0f));

		UI_Divider();

		UI_Spacer(UI_Em(1.0f));
		UI_NextSize2(UI_SumOfChildren(), UI_SumOfChildren());
		UI_Column()
		{
			UI_Spacer(UI_Em(0.5f));
			UI_Text(Str8Lit("Column 2"));
			UI_Spacer(UI_Em(0.2f));

			UI_NextSize2(UI_Em(5.0f), UI_Pixels(1));
			UI_NextBackgroundColor(ui_state->theme.text_color);
			UI_BoxMake(UI_BoxFlag_DrawBackground, Str8Lit(""));
			UI_Spacer(UI_Em(0.5f));

			UI_Button(Str8Lit("Button"));

			UI_Spacer(UI_Em(0.5f));

			UI_Tree(Str8Lit("Entities"))
			{
				UI_Spacer(tree_spacing);
				UI_Text(Str8Lit("Text inside a tree"));
				UI_Spacer(tree_spacing);
				UI_Text(Str8Lit("Text inside a tree"));
				UI_Spacer(tree_spacing);

				UI_Tree(Str8Lit("Entity 1"))
				{
					UI_Spacer(tree_spacing);
					UI_TextF("Pos: (%.02f, %.02f)", 1.2f, 0.5f);
					UI_Spacer(tree_spacing);
					UI_TextF("Type: %s ", "static");
					UI_Spacer(tree_spacing);
					local_persist F32 health = 0;
					UI_SliderF32(&health, -10.0f, 10, Str8Lit("Health"));
					UI_Spacer(tree_spacing);
				}
				UI_Spacer(tree_spacing);

				UI_Tree(Str8Lit("Entity 2"))
				{
					UI_Spacer(tree_spacing);
					UI_Text(Str8Lit("Text inside a tree2"));
					UI_Spacer(tree_spacing);
					UI_Text(Str8Lit("Text inside a tree2"));
					UI_Spacer(tree_spacing);
					UI_Tree(Str8Lit("My tree4"))
					{
						UI_Spacer(tree_spacing);
						UI_Text(Str8Lit("Text inside a tree2"));
						UI_Spacer(tree_spacing);
						UI_Text(Str8Lit("Text inside a tree2"));
						UI_Spacer(tree_spacing);
						local_persist B32 check_value = false;
						UI_Check(Str8Lit("Test check##10"), &check_value);

						UI_Spacer(tree_spacing);
					}
				}
			}
			UI_Spacer(UI_Em(0.5f));

			UI_NextSize2(UI_Em(10), UI_Em(10));
			UI_PushScrollableContainer(Str8Lit("scrollablecontainer3"));
			UI_NextSize2(UI_SumOfChildren(), UI_SumOfChildren());
			UI_Column()
			{
				for (U32 i = 0; i < 100; ++i)
				{
					UI_Text(Str8Lit("Hello"));
				}
			}

			UI_PopScrollableContainer();
			UI_Spacer(UI_Em(0.5f));

			local_persist char input_buffer[256] = {0};
			UI_NextSize2(UI_Em(10), UI_Em(1.5f));
			if (UI_TextInput(input_buffer, sizeof(input_buffer), Str8Lit("Your username")).enter)
			{
				printf("Enter\n");
			}

			UI_Spacer(UI_Em(0.5f));
			UI_Divider();
			UI_Spacer(UI_Em(0.5f));
			UI_Text(Str8Lit("Default widget sizes test"));
			UI_Spacer(UI_Em(0.5f));
			UI_Divider();
			UI_Spacer(UI_Em(0.5f));
			UI_Button(Str8Lit("Default button size, g"));
			local_persist B32 check_test = false;
			UI_Check(Str8Lit("Default check size"), &check_test);
			local_persist F32 slider_value = 0;
			UI_SliderF32(&slider_value, 0, 10, Str8Lit("Default slider size"));
			local_persist Vec4F32 color = {1, 1, 1, 1};
			UI_ColorPicker(&color, Str8Lit("Default color picker size"));
		}
		UI_Spacer(UI_Em(0.5f));
	}
}

internal S32
EntryPoint(String8List args)
{

	MemoryArena permanent_arena;
	ArenaInit(&permanent_arena, OS_AllocMem(MEGABYTES(128)), MEGABYTES(128));

	OS_Init();

	OS_Window *window = OS_CreateWindow(Str8Lit("Test"), 0, 0, 800, 800, true);

	R_State *renderer = R_Init(window);
	R_SelectState(renderer);

	R_FontAtlas *font_atlas = R_FontAtlasMake(&permanent_arena, V2S(2048, 2048));
	r_state->font_atlas = font_atlas;

	UI_State *state = UI_Init(R_FontKeyFromString(CORE_RESOURCE("font/Inter-Regular.ttf")),
							  window);
	UI_SelectState(state);

	R_FontAtlas *tile_atlas = R_FontAtlasMake(&permanent_arena, V2S(1024, 1024));
	{
		R_LoadedBitmap loaded_bitmaps[16] = {0};
		TempMemoryArena scratch = GetScratch(0, 0);

		for (U32 i = 0; i < 16; ++i)
		{
			S32 channels = 0;
			String8 path = {0};
			if (i < 10)
			{
				path = PushStr8F(scratch.arena, "../res/test/Tiles/tile_000%d.png", i);
			}
			else
			{
				path = PushStr8F(scratch.arena, "../res/test/Tiles/tile_00%d.png", i);
			}

			loaded_bitmaps[i].data = stbi_load((const char *)path.str, &loaded_bitmaps[i].dim.width, &loaded_bitmaps[i].dim.height, &channels, 0);
			R_FontAtlasRegion region = R_FontAtlasRegionAlloc(&permanent_arena, tile_atlas, loaded_bitmaps[i].dim);
			R_FillFontAtlasRegionWithBitmap(tile_atlas, region, &loaded_bitmaps[i]);

		}
		ReleaseScratch(scratch);
		tile_atlas->texture.handle = r_state->GPULoadTexture(tile_atlas->data, tile_atlas->dim.width, tile_atlas->dim.height);

		for (U32 i = 0; i < 16; ++i)
		{
			stbi_image_free(loaded_bitmaps[i].data);
		}
	}

	F64 dt = 0;
	F64 start_counter = OS_SecondsSinceAppStart();
	B32 running = true;

	while (running)
	{
		TempMemoryArena scratch = GetScratch(0, 0);

		OS_EventList *event_list = OS_GatherEventsFromWindow(scratch.arena);

		for (OS_EventNode *node = event_list->first;
			 node != 0;
			 node = node->next)
		{
			switch (node->event.type)
			{
				case OS_EventType_Quit:
				{
					running = false;
				} break;

				case OS_EventType_KeyPress:
				{
					if (node->event.key == OS_Key_F11)
					{
						OS_ToggleFullscreen(window);
					}
				} break;
			}
		}

		R_Begin(scratch.arena);
		R_PushText(V2(1700, 50), R_FontKeyFromString(CORE_RESOURCE("font/Inter-Regular.ttf")), 20, Str8Lit("Hello, world!"), V4(1.0f, 1.0f, 1.0f, 1.0f));
		R_PushRect(V2(1200 - 50, 50), V2(1200 + 450, 50 + 65), .color = V4(0.5, 0, 0, 1), .corner_radius = V4(10, 10, 10, 10), .edge_softness = 1);
		R_PushRect(V2(1200 - 50, 50), V2(1200 + 450, 50 + 65), .color = V4(1, 1, 1, 1), .corner_radius = V4(10, 10, 10, 10), .edge_softness = 1, .border_thickness = 1);

		R_PushRect(V2(0, 0), V2(1024 * 4, 1024 * 4), .texture = tile_atlas->texture);

		Vec4F32 corner_radius = V4(30, 50, 20, 10);

		R_PushRect(V2(50, 500), V2(500, 1000), .color = V4(0, 0, 0, 1), .corner_radius = corner_radius, .edge_softness = 1);
		R_PushRect(V2(50, 500), V2(500, 1000), .color = V4(1, 0, 0, 1), .corner_radius = corner_radius, .edge_softness = 1, .border_thickness = 0.5f);

		UI_Begin(UI_DefaultTheme(), event_list, dt);

		UITest();

		UI_NextRelativePos2(1200, 200);
		UI_NextSize2(UI_Em(20), UI_Em(20));
		UI_NextChildLayoutAxis(Axis2_Y);
		UI_NextBackgroundColor(ui_state->theme.window_color);
		UI_Box *window_container = UI_BoxMake(UI_BoxFlag_FixedPos |
											  UI_BoxFlag_DrawBackground |
											  UI_BoxFlag_DrawBorder |
											  UI_BoxFlag_DrawDropShadow,
											  Str8Lit("WindowContainer"));

		UI_Parent(window_container)
		{
			UI_NextSize2(UI_Pct(1), UI_Em(1.0f));
			UI_Row()
			{
				UI_Spacer(UI_Fill());

				UI_Box *title = UI_BoxMake(UI_BoxFlag_DrawText,
										   Str8Lit("WindowTitlebar"));

				UI_EquipBoxWithDisplayString(title, Str8Lit("My window"));

				UI_NextSize(Axis2_X, UI_Fill());
				UI_Row()
				{
					UI_Spacer(UI_Fill());

					UI_NextIcon(R_IconIndex_Cross);
					UI_NextSize2(UI_Em(1.0f), UI_Em(1.0f));
					UI_NextBackgroundColor(V4(0.6f, 0, 0, 1));
					UI_Box *close_button = UI_BoxMake(UI_BoxFlag_DrawText |
													  UI_BoxFlag_DrawBackground |
													  UI_BoxFlag_DrawBorder |
													  UI_BoxFlag_HotAnimation |
													  UI_BoxFlag_ActiveAnimation,
													  Str8Lit("WindowCloseButton"));
					UI_Comm comm = UI_CommFromBox(close_button);
				}
			}

			UI_Spacer(UI_Em(0.5f));
			UI_Row()
			{
				UI_Spacer(UI_Em(0.5f));
				UI_Column()
				{
					UI_NextSize2(UI_Em(5), UI_Em(1));
					UI_Button(Str8Lit("Click me!"));

					UI_Spacer(UI_Em(0.5f));

					UI_Row()
					{
						local_persist F32 test_value;
						UI_SliderF32(&test_value, -10, 10, Str8Lit("TestSlider"));

						UI_Spacer(UI_Em(0.5f));

						UI_Text(Str8Lit("A slider"));
					}
				}
			}
		}

		UI_End();

		//R_DEBUG_DrawFontAtlas(font_atlas);

		R_End();
		D3D11_End(V4(0.3f, 0.3f, 0.3f, 1.0f));

		F64 end_counter = OS_SecondsSinceAppStart();
		dt = end_counter - start_counter;
		start_counter = end_counter;

		ReleaseScratch(scratch);
	}
	return(0);
}