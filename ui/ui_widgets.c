internal void
UI_DefaultSize(UI_Size x, UI_Size y)
{
	UI_LayoutStyle *layout = UI_TopLayout();

	if (!layout->pref_size[Axis2_X].kind)
	{
		UI_NextSize(Axis2_X, x);
	}

	if (!layout->pref_size[Axis2_Y].kind)
	{
		UI_NextSize(Axis2_Y, y);
	}
}

internal void
UI_Spacer(UI_Size size)
{
	Axis2 axis = UI_TopParent()->child_layout_axis;
	UI_NextSize(axis, size);
	UI_NextSize(Flip(axis), UI_Pixels(0));
	UI_BoxMake(0,
	           Str8Lit(""));
}

internal UI_Box *
UI_Text(String8 string)
{
	UI_Box *box = UI_BoxMake(UI_BoxFlag_DrawText,
	                         Str8Lit(""));
	UI_EquipBoxWithDisplayString(box, string);
	return(box);
}

internal void
UI_TextF(char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	String8 string = PushStr8FV(UI_FrameArena(), fmt, args);
	UI_Text(string);
	va_end(args);
}

internal UI_Comm
UI_Button(String8 string)
{
	UI_LayoutStyle *layout = UI_TopLayout();

	UI_NextHoverCursor(OS_Cursor_Hand);
	UI_Box *box = UI_BoxMake(UI_BoxFlag_DrawBackground |
	                         UI_BoxFlag_DrawText |
	                         UI_BoxFlag_DrawBorder |
	                         UI_BoxFlag_HotAnimation |
	                         UI_BoxFlag_ActiveAnimation |
	                         UI_BoxFlag_Clickable,
	                         string);
	UI_EquipBoxWithDisplayString(box, string);

	UI_Comm comm = UI_CommFromBox(box);

	return(comm);
}

internal UI_Comm
UI_ButtonF(char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	String8 string = PushStr8FV(UI_FrameArena(), fmt, args);

	UI_Comm comm = UI_Button(string);

	va_end(args);

	return(comm);
}

internal UI_Comm
UI_Check(String8 string, B32 *val)
{
	B32 save_state = UI_TopLayout()->flags & UI_BoxFlag_SaveState;
	UI_TopLayout()->flags &= ~UI_BoxFlag_SaveState;

	UI_PushString(string);
	UI_NextSize2(UI_SumOfChildren(), UI_SumOfChildren());
	UI_NextChildLayoutAxis(Axis2_X);
	UI_Box *container = UI_BoxMake(0, Str8Lit("CheckContainer"));
	UI_Comm comm = {0};
	UI_Parent(container)
	{
		UI_NextSize2(UI_Em(1.0f), UI_Em(1.0f));
		UI_NextHoverCursor(OS_Cursor_Hand);
		UI_Box *check_box = UI_BoxMake(UI_BoxFlag_DrawBackground |
		                               UI_BoxFlag_Clickable |
		                               UI_BoxFlag_DrawBorder |
		                               UI_BoxFlag_HotAnimation |
		                               UI_BoxFlag_ActiveAnimation,
		                               Str8Lit("Check"));

		comm = UI_CommFromBox(check_box);

		if (comm.pressed)
		{
			*val = !(*val);
		}

		if (*val)
		{
			UI_Parent(check_box)
			{
				UI_NextIcon(R_IconIndex_Check);
				UI_NextSize2(UI_Pct(1), UI_Pct(1));
				UI_Box *check_mark = UI_BoxMake(UI_BoxFlag_DrawText,
				                                Str8Lit("CheckMark"));
			}
		}
	}

	UI_PopString();

	return(comm);
}

internal UI_Box *
UI_BeginNamedColumn(String8 string)
{
	UI_DefaultSize(UI_SumOfChildren(), UI_SumOfChildren());

	UI_NextChildLayoutAxis(Axis2_Y);
	UI_Box *box = UI_BoxMake(0, string);
	UI_PushParent(box);
	return(box);
}

internal void
UI_EndNamedColumn()
{
	UI_PopParent();
}

internal UI_Box *
UI_BeginColumn()
{
	UI_Box *box = UI_BeginNamedColumn(Str8Lit(""));
	return(box);
}

internal void
UI_EndColumn()
{
	UI_EndNamedColumn();
}

internal UI_Box *
UI_BeginNamedRow(String8 string)
{
	UI_DefaultSize(UI_SumOfChildren(), UI_SumOfChildren());
	UI_NextChildLayoutAxis(Axis2_X);
	UI_Box *box = UI_BoxMake(0, string);
	UI_PushParent(box);
	return(box);
}

internal void
UI_EndNamedRow()
{
	UI_PopParent();
}

internal UI_Box *
UI_BeginRow()
{
	UI_Box *box = UI_BeginNamedRow(Str8Lit(""));
	return(box);
}

internal void
UI_EndRow()
{
	UI_EndNamedRow();
}

internal void
UI_EndTree()
{
	UI_EndColumn();
	UI_EndRow();

	UI_PopParent();

	UI_Box *parent = UI_TopParent();

	UI_PopString();
}

internal B32
UI_BeginTree(String8 string)
{
	UI_PushStringF(string);
	UI_NextChildLayoutAxis(Axis2_Y);
	UI_NextSize2(UI_SumOfChildren(), UI_SumOfChildren());
	UI_NextBoxFlags(UI_BoxFlag_SaveState);
	UI_Box *container = UI_BoxMake(UI_BoxFlag_AnimateWidth |
	                               UI_BoxFlag_AnimateHeight,
	                               Str8Lit("TreeContainer"));

	UI_PushParent(container);

	UI_NextSize2(UI_SumOfChildren(), UI_Em(1.0f));
	UI_NextBackgroundColor(V4(0, 0, 0, 0));
	UI_NextHotColor(V4(0, 0, 0, 0));
	UI_NextActiveColor(V4(0, 0, 0, 0));
	UI_NextHoverCursor(OS_Cursor_Hand);
	UI_NextBoxFlags(UI_BoxFlag_HotAnimation |
	                UI_BoxFlag_Clickable |
	                UI_BoxFlag_DrawBackground |
	                UI_BoxFlag_ActiveAnimation);
	UI_Box *label = UI_BeginNamedRow(Str8Lit("Label"));
	UI_Comm comm = UI_CommFromBox(label);

	B32 clicked_on = false;
	B32 clicked_off = false;
	B32 was_on = container->show_expanded_tree;

	if (comm.pressed)
	{
		container->show_expanded_tree = !(container->show_expanded_tree);
		if (container->show_expanded_tree)
		{
			clicked_on = true;
		}
		else
		{
			clicked_off = true;
		}
	}

	if (container->show_expanded_tree)
	{
		UI_NextIcon(R_IconIndex_Angle_Down);
	}
	else
	{
		UI_NextIcon(R_IconIndex_Angle_Right);
	}

	UI_NextSize2(UI_Em(1.0f), UI_Em(1.0f));
	UI_Box *collapser = UI_BoxMake(UI_BoxFlag_DrawText,
	                               Str8Lit("TreeCollapser"));

	UI_Spacer(UI_Em(0.5f));

	UI_Text(string);
	UI_EndRow();

	UI_NextSize2(UI_SumOfChildren(), UI_SumOfChildren());
	UI_Box *box = UI_BeginNamedRow(Str8Lit("TreeRow"));

	B32 should_show = container->show_expanded_tree;

	UI_Spacer(UI_Em(0.5f));

	if (should_show)
	{
		UI_NextSize(Axis2_Y, UI_Fill());
		UI_NamedColumn(Str8Lit("Indent guide column"))
		{
			UI_NextSize2(UI_Pixels(1), UI_Fill());
			UI_NextBackgroundColor(V4(0.9f, 0.9f, 0.9f, 1.0f));
			UI_BoxMake(UI_BoxFlag_DrawBackground |
			           UI_BoxFlag_AnimateHeight |
			           UI_BoxFlag_AnimateStart |
			           UI_BoxFlag_SaveState,
			           Str8Lit("Indent guide"));
		}
	}

	UI_Size indentation = UI_Em(1.5f);
	UI_Spacer(indentation);

	UI_NextSize2(UI_SumOfChildren(), UI_SumOfChildren());
	UI_NextBoxFlags(UI_BoxFlag_AnimateHeight | UI_BoxFlag_AnimateWidth);
	UI_BeginNamedColumn(Str8Lit("TreeColumn"));

	if (!(should_show))
	{
		UI_EndTree();
	}

	return(should_show);
}

internal B32
UI_BeginTreeF(char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	String8 string = PushStr8FV(UI_FrameArena(), fmt, args);
	B32 b = UI_BeginTree(string);
	va_end(args);
	return(b);
}

typedef struct UI_DragData
{
	F32 val;
	F32 min;
	F32 max;
} UI_DragData;

UI_CUSTOM_DRAW_FUNCTION(UI_SliderF32CustomDraw)
{
	UI_RectStyle *rect_style = &root->rect_style;
	UI_TextStyle *text_style = &root->text_style;

	R_Font *font = R_GetFontFromKey(ui_state->font_key, text_style->font_size);

	Vec4F32 corner_radius = rect_style->corner_radius;
	corner_radius = V4MulF32(corner_radius, (F32)font->height);

	if (root->flags & UI_BoxFlag_DrawDropShadow)
	{
		R_PushRect(V2SubV2(root->calc_rect.min, V2(10, 10)),
		           V2AddV2(root->calc_rect.max, V2(15, 15)),
		           .color = V4(0, 0, 0, 0.5f),
		           .edge_softness = 10.0f,
		           .corner_radius = corner_radius);
	}

	if (root->flags & UI_BoxFlag_DrawBackground)
	{
		Vec4F32 color0 = rect_style->background_color;
		Vec4F32 color1 = rect_style->background_color;

		if ((root->flags & UI_BoxFlag_ActiveAnimation) &&
		    UI_IsActive(root))
		{
			color0 = rect_style->active_color;
		}
		else if (root->flags & UI_BoxFlag_HotAnimation &&
		         UI_IsHot(root))
		{
			color0 = rect_style->hot_color;
		}

		UI_DragData *drag_data = root->user;

		Vec2F32 min = root->calc_rect.min;
		Vec2F32 max = root->calc_rect.max;

		// TODO(hampus): Look more into this!
		if (UI_BoxHasFlag(root, UI_BoxFlag_DrawBorder))
		{
			min.y += 1;
			min.x += 1;

			max.x -= 1;
			max.y -= 1;
		}

		F32 width = max.x - min.x;

		max.x = min.x + (F32)(drag_data->val - drag_data->min) / (F32)(drag_data->max - drag_data->min) * width;
		R_PushRectGradient(min, max,
		                   color0, color1,
		                   .corner_radius = corner_radius,
		                   .edge_softness = rect_style->edge_softness);
	}

	if (root->flags & UI_BoxFlag_DrawBorder)
	{
		if (UI_IsFocused(root) && 
		    !UI_KeyIsNull(ui_state->focus_key) && 
		    UI_BoxHasFlag(root, UI_BoxFlag_FocusAnimation))
		{
			// TODO(hampus): Fix this. 
			F32 t = 0;
			R_PushRect(root->calc_rect.min, root->calc_rect.max,
			           .corner_radius = corner_radius,
			           .border_thickness = rect_style->border_thickness,
			           .color = V4(0.8f + 0.2f * t, 0.8f + 0.2f * t, 0.0f, 1.0f),
			           .edge_softness = 1.0f);
		}
		else
		{
			R_PushRect(root->calc_rect.min, root->calc_rect.max,
			           .corner_radius = corner_radius,
			           .border_thickness = rect_style->border_thickness,
			           .color = rect_style->border_color,
			           .edge_softness = 1.0f);
		}
	}
}

internal UI_Comm
UI_SliderF32(F32 *val, F32 min, F32 max, String8 string)
{
	UI_PushString(string);

	UI_DefaultSize(UI_Em(6), UI_Em(1.0f));

	UI_Box *slider_back = UI_BoxMake(UI_BoxFlag_DrawBackground,
	                                 Str8Lit("SliderBack"));
	UI_Comm comm = {0};
	UI_Parent(slider_back)
	{
		UI_NextSize2(UI_Pct(1), UI_Pct(1));
		UI_NextHoverCursor(OS_Cursor_ResizeX);
		UI_NextBackgroundColor(V4(0.0f, 0.3f, 0.4f, 1.0f));
		UI_NextHotColor(V4(0.0f, 0.5f, 0.6f, 1.0f));
		UI_NextActiveColor(V4(0.0f, 0.6f, 0.7f, 1.0f));
		UI_Box *dragger = UI_BoxMake(UI_BoxFlag_DrawBackground |
		                             UI_BoxFlag_HotAnimation |
		                             UI_BoxFlag_DrawBorder |
		                             UI_BoxFlag_ActiveAnimation |
		                             UI_BoxFlag_Clickable,
		                             Str8Lit("SliderDragger"));
		comm = UI_CommFromBox(dragger);
		if (comm.dragging)
		{
			F32 val_pct = (comm.drag_delta.x) / slider_back->calc_size[Axis2_X];

			*val += (F32)(val_pct * (max - min));
		}

		*val = Clamp(min, *val, max);

		UI_DragData *drag_data = PushStruct(UI_FrameArena(), UI_DragData);
		drag_data->val = *val;
		drag_data->min = min;
		drag_data->max = max;

		UI_EquipBoxWithCustomDrawFunction(dragger, UI_SliderF32CustomDraw, drag_data);

		UI_NextSize2(UI_Pct(1), UI_Pct(1));
		UI_Box *value_display = UI_BoxMake(UI_BoxFlag_DrawText |
		                                   UI_BoxFlag_FixedPos,
		                                   Str8Lit(""));

		UI_EquipBoxWithDisplayString(value_display, PushStr8F(UI_FrameArena(), "%.02f", *val));

	}

	UI_PopString();

	return(comm);
}

internal UI_Comm
UI_SliderS32(S32 *val, S32 min, S32 max, String8 string)
{
	UI_PushString(string);

	UI_DefaultSize(UI_Em(6), UI_Em(1.0f));

	UI_Box *slider_back = UI_BoxMake(UI_BoxFlag_DrawBackground |
	                                 UI_BoxFlag_DrawBorder,
	                                 Str8Lit("SliderBack"));
	UI_Comm comm = {0};
	UI_Parent(slider_back)
	{
		UI_NextSize2(UI_Pct(1), UI_Pct(1));
		UI_NextHoverCursor(OS_Cursor_ResizeX);
		UI_NextBackgroundColor(V4(0.0f, 0.3f, 0.4f, 1.0f));
		UI_NextHotColor(V4(0.0f, 0.5f, 0.6f, 1.0f));
		UI_NextActiveColor(V4(0.0f, 0.6f, 0.7f, 1.0f));
		UI_Box *dragger = UI_BoxMake(UI_BoxFlag_DrawBackground |
		                             UI_BoxFlag_DrawBorder |
		                             UI_BoxFlag_HotAnimation |
		                             UI_BoxFlag_ActiveAnimation |
		                             UI_BoxFlag_Clickable,
		                             Str8Lit("SliderDragger"));
		comm = UI_CommFromBox(dragger);
		if (comm.dragging)
		{
			F32 val_pct = (comm.drag_delta.x) / dragger->calc_size[Axis2_X];

			*val += (S32)(val_pct * (max - min));
		}

		*val = Clamp(min, *val, max);

		UI_DragData *drag_data = PushStruct(UI_FrameArena(), UI_DragData);
		drag_data->val = (F32)*val;
		drag_data->min = (F32)min;
		drag_data->max = (F32)max;

		UI_EquipBoxWithCustomDrawFunction(dragger, UI_SliderF32CustomDraw, drag_data);

		UI_NextRelativePos2(0, 0);
		UI_NextSize2(UI_Pct(1), UI_Pct(1));
		UI_Box *value_display = UI_BoxMake(UI_BoxFlag_DrawText |
		                                   UI_BoxFlag_FixedX |
		                                   UI_BoxFlag_FixedY,
		                                   Str8Lit(""));

		UI_EquipBoxWithDisplayString(value_display, PushStr8F(UI_FrameArena(), "%d", *val));

	}

	UI_PopString();

	return(comm);
}

internal void
UI_Divider()
{
	UI_NextBackgroundColor(ui_state->theme.text_color);
	UI_NextSize(UI_TopParent()->child_layout_axis, UI_Pixels(1));
	UI_NextSize(Flip(UI_TopParent()->child_layout_axis), UI_Fill());
	UI_NextCornerRadius4(0);
	UI_NextTextEdgeSoftness(0);
	UI_BoxMake(UI_BoxFlag_DrawBackground, Str8Lit(""));
}

internal void
UI_ColorPicker(Vec4F32 *color, String8 string)
{
	UI_PushString(string);

	UI_DefaultSize(UI_Em(16), UI_Em(5));
	UI_Box *container = UI_BoxMake(UI_BoxFlag_DrawBorder, Str8Lit("ColorWheelContainer"));

	UI_Parent(container)
	{
		UI_Spacer(UI_Em(0.5f));
		UI_NextSize2(UI_Fill(), UI_Pct(1));
		UI_Column()
		{
			UI_Spacer(UI_Em(0.5f));
			UI_NextSize2(UI_Pct(1), UI_Fill());
			UI_Row()
			{
				UI_NextSize2(UI_TextContent(), UI_Em(1.0f));
				UI_Text(string);

				UI_Spacer(UI_Em(0.5f));

				UI_NextBackgroundColor(*color);
				UI_NextSize2(UI_Em(1.0f), UI_Em(1.0f));
				UI_NextHoverCursor(OS_Cursor_Hand);
				UI_Box *color_rect = UI_BoxMake(UI_BoxFlag_DrawBackground |
				                                UI_BoxFlag_DrawBorder |
				                                UI_BoxFlag_HotAnimation |
				                                UI_BoxFlag_ActiveAnimation |
				                                UI_BoxFlag_Clickable,
				                                Str8Lit("ColorRect"));
				UI_Comm comm = UI_CommFromBox(color_rect);
				if (comm.pressed)
				{
					container->show_color_wheel = !container->show_color_wheel;
				}

				UI_NextRelativePos2(color_rect->calc_pos[Axis2_X], color_rect->calc_pos[Axis2_Y] - 400);
				UI_NextSize2(UI_Em(15), UI_Em(15));
				UI_NextBackgroundColor(ui_state->theme.window_color);
				UI_NextBoxFlags(UI_BoxFlag_DrawBorder | UI_BoxFlag_DrawDropShadow | UI_BoxFlag_Clip | UI_BoxFlag_DrawBackground | UI_BoxFlag_FixedX | UI_BoxFlag_FixedY | UI_BoxFlag_AnimateScale);
				UI_Popup(&container->show_color_wheel)
				{
				}
			}

			UI_Spacer(UI_Em(0.5f));
			UI_NextSize2(UI_Pct(1), UI_Fill());
			UI_Row()
			{
				UI_SliderF32(&color->r, 0, 1.0f, Str8Lit("R"));

				UI_Spacer(UI_Em(0.5f));

				UI_SliderF32(&color->g, 0, 1.0f, Str8Lit("G"));
			}

			UI_Spacer(UI_Em(0.5f));
			UI_NextSize2(UI_Pct(1), UI_Fill());
			UI_Row()
			{
				UI_SliderF32(&color->b, 0, 1.0f, Str8Lit("B"));

				UI_Spacer(UI_Em(0.5f));

				UI_SliderF32(&color->a, 0, 1.0f, Str8Lit("A"));
			}
			UI_Spacer(UI_Em(0.5f));
		}
		UI_Spacer(UI_Em(0.5f));
	}

	UI_PopString();
}

internal UI_Comm
UI_TextInput(char *buffer, size_t buffer_size, String8 string)
{
	UI_PushString(string);
	UI_DefaultSize(UI_SumOfChildren(), UI_Em(1));
	UI_Box *container = UI_BoxMake(0, Str8Lit(""));
	UI_Comm input_box_comm = {0};
	UI_Parent(container)
	{
		UI_NextSize2(UI_Fill(), UI_Pct(1));
		UI_Box *input_box = UI_BoxMake(UI_BoxFlag_DrawBackground |
		                               UI_BoxFlag_DrawText |
		                               UI_BoxFlag_DrawBorder |
		                               UI_BoxFlag_HotAnimation |
		                               UI_BoxFlag_ActiveAnimation |
		                               UI_BoxFlag_FocusAnimation |
		                               UI_BoxFlag_Clickable, Str8Lit("InputBox"));
		input_box_comm = UI_CommFromBox(input_box);

		R_PushClipRect(input_box->calc_rect);

		U64 string_length = CStringLength(buffer);

		F32 overflow_x = 0;

		B32 focused = UI_KeyMatch(input_box->key, ui_state->focus_key);

		if (focused)
		{
			char ch = OS_GetLastChar();

			if (ch == 8)
			{
				if (string_length)
				buffer[string_length - 1] = 0;
			}
			else if (ch >= 3 && ch <= 125)
			{
				if (string_length + 1 < buffer_size)
				buffer[string_length] = ch;
			}
		}

		String8 text = Str8C(buffer);

		R_Font *font = R_GetFontFromKey(ui_state->font_key, UI_TopTextStyle()->font_size);

		Vec2F32 text_dim = R_GetTextDim(font, text);
		overflow_x = (text_dim.x + UI_Em(0.5f).value) - input_box->calc_size[Axis2_X];
		if (overflow_x < 0)
		{
			overflow_x = 0;
		}

		UI_Parent(input_box)
		{
			UI_Spacer(UI_Pixels(-overflow_x));
			UI_Spacer(UI_Pixels(5));
			UI_NextSize(Axis2_Y, UI_Pixels(input_box->calc_size[Axis2_Y]));
			UI_Text(text);
			if (focused)
			{
				UI_Spacer(UI_Pixels(2));
				UI_NextSize2(UI_Pixels(2), UI_Fill());
				UI_NextBackgroundColor(V4(1.0f, 1.0f, 1.0f, 1.0f));
				UI_BoxMake(UI_BoxFlag_DrawBackground |
				           UI_BoxFlag_AnimateX |
				           UI_BoxFlag_AnimateY |
				           UI_BoxFlag_AnimateWidth, Str8Lit("Cursor"));
			}
		}

		R_PopClipRect();
	}

	UI_PopString();

	return(input_box_comm);
}

typedef struct UI_RadioData
{
	B32 *val;
	String8 string;
} UI_RadioData;

internal void
UI_Radio(UI_RadioData *data, U32 data_count, String8 string)
{
	UI_PushString(string);

	UI_Text(string);
	UI_Spacer(UI_Em(0.5f));

	F32 corner_radius = 0.6f;
	F32 inner_box_pct_size = 0.6f;

	UI_PushCornerRadius4(corner_radius);
	for (U32 i = 0; i < data_count; ++i)
	{
		UI_Row()
		{
			UI_NextSize2(UI_Em(1), UI_Em(1));
			UI_Box *box = UI_BoxMake(UI_BoxFlag_DrawBackground |
			                         UI_BoxFlag_DrawBorder |
			                         UI_BoxFlag_Clickable |
			                         UI_BoxFlag_HotAnimation |
			                         UI_BoxFlag_ActiveAnimation,
			                         data[i].string);
			UI_Comm comm = UI_CommFromBox(box);
			if (comm.pressed)
			{
				*data[i].val = true;
				for (U32 j = 0; j < data_count; ++j)
				{
					if (i != j)
					{
						*data[j].val = false;
					}
				}
			}
			if (*(data[i].val))
			{
				UI_Parent(box)
				{
					UI_Spacer(UI_Fill());
					UI_NextSize2(UI_Em(0.6f), UI_Fill());
					UI_Column()
					{
						UI_Spacer(UI_Fill());

						UI_NextBackgroundColor(V4(0.0f, 0.3f, 0.4f, 1.0f));
						UI_NextSize2(UI_Em(inner_box_pct_size), UI_Em(inner_box_pct_size));
						UI_NextCornerRadius4(inner_box_pct_size * corner_radius);

						UI_Box *inner_box = UI_BoxMake(UI_BoxFlag_AnimateScale |
						                               UI_BoxFlag_AnimateStart |
						                               UI_BoxFlag_DrawBackground ,
						                               PushStr8F(UI_FrameArena(), "%d", i));

						UI_Spacer(UI_Fill());
					}
					UI_Spacer(UI_Fill());
				}
			}
			UI_Spacer(UI_Em(0.5f));

			UI_Text(data[i].string);
		}

		UI_Spacer(UI_Em(0.5f));
	}
	UI_PopCornerRadius4();


	UI_PopString();
}

internal void
UI_PopScrollableContainer()
{
	UI_PopString();

	UI_PopParent();
	UI_PopParent();

	UI_EndColumn();

	UI_PopParent();
	R_PopClipRect();
}

internal UI_Box *
UI_PushScrollableContainer(String8 string)
{
	UI_PushString(string);

	UI_Box *view_box = 0;

	UI_NextChildLayoutAxis(Axis2_Y);
	UI_Box *container = UI_BoxMake(0, Str8Lit("Container"));
	UI_PushParent(container);

	F32 scrollbar_size = UI_Em(0.7f).value;
	F32 container_width = container->calc_size[Axis2_X];
	F32 container_height = container->calc_size[Axis2_Y];

	UI_NextChildLayoutAxis(Axis2_X);
	UI_NextSize2(UI_Pixels(container_width), UI_Pixels(container_height - scrollbar_size));
	UI_Box *container_x = UI_BoxMake(UI_BoxFlag_DrawBackground, Str8Lit("Containerx"));

	UI_Box *content_box = UI_GetBox(Str8Lit("ScrollCalcBox"));

	F32 content_dim[Axis2_COUNT] = {0};

	if (content_box)
	{
		content_dim[Axis2_X] = content_box->calc_size[Axis2_X];
		content_dim[Axis2_Y] = content_box->calc_size[Axis2_Y];
	}

	F32 view_dim[Axis2_COUNT] = {0};

	F32 overflow_x = 0;
	F32 overflow_y = 0;

	F32 reverse_scroll_y = true;

	F32 scroll_y = 0;

	B32 dragging_scroll_bar = false;
	B32 scrolling_with_mousewheel = false;
	UI_Parent(container_x)
	{
		UI_NextSize2(UI_Pixels(container_width - scrollbar_size), UI_Pixels(container_height - scrollbar_size));
		UI_NextChildLayoutAxis(Axis2_Y);
		UI_NextBackgroundColor(ui_state->theme.window_color);
		UI_NextChildLayoutCorner(UI_Corner_BottomLeft);
		view_box = UI_BoxMake(UI_BoxFlag_DrawBackground |
		                      UI_BoxFlag_DrawBorder, 
		                      Str8Lit("ContentBox"));
		UI_Comm comm = UI_CommFromBox(view_box);

		view_dim[Axis2_X] = floorf(view_box->calc_size[Axis2_X]);
		view_dim[Axis2_Y] = floorf(view_box->calc_size[Axis2_Y]);

		if (comm.scroll)
		{
			F32 scroll_speed = 100;
			if (reverse_scroll_y)
			container->scroll.y -= comm.scroll * scroll_speed;
			else
			container->scroll.y += comm.scroll * scroll_speed;

			scrolling_with_mousewheel = true;
		}
		if (comm.page_up)
		{
			F32 scroll_speed = -view_dim[Axis2_Y];
			if (reverse_scroll_y)
			container->scroll.y -= scroll_speed;
			else
			container->scroll.y += scroll_speed;
			scrolling_with_mousewheel = true;
		}

		if (comm.page_down)
		{
			F32 scroll_speed = view_dim[Axis2_Y];
			if (reverse_scroll_y)
			container->scroll.y -= scroll_speed;
			else
			container->scroll.y += scroll_speed;
			scrolling_with_mousewheel = true;
		}

		overflow_x = content_dim[Axis2_X] - view_dim[Axis2_X];
		overflow_y = content_dim[Axis2_Y] - view_dim[Axis2_Y];

		B32 need_scrollbar_y = overflow_y > 0;
		B32 need_scrollbar_x = overflow_x > 0;

		if (!need_scrollbar_y)
		{
			content_dim[Axis2_Y] = view_dim[Axis2_Y];
		}
		if (!need_scrollbar_x)
		{
			content_dim[Axis2_X] = view_dim[Axis2_X];
		}

		UI_NextSize2(UI_Pixels(scrollbar_size), UI_Fill());
		UI_NextChildLayoutAxis(Axis2_Y);
		UI_Box *right_scrollbar = UI_BoxMake(UI_BoxFlag_Clickable |
		                                     UI_BoxFlag_DrawBorder |
		                                     UI_BoxFlag_DrawBackground |
		                                     UI_BoxFlag_HotAnimation |
		                                     UI_BoxFlag_ActiveAnimation,
		                                     Str8Lit("Right border"));
		UI_Comm right_scrollbar_comm = UI_CommFromBox(right_scrollbar);

		if (right_scrollbar_comm.dragging)
		{
			dragging_scroll_bar = true;
			if (reverse_scroll_y)
			{
				container->scroll.y -= ((right_scrollbar_comm.drag_delta.y) / right_scrollbar->calc_size[Axis2_Y]) * content_dim[Axis2_Y];
			}
			else
			{
				container->scroll.y += ((right_scrollbar_comm.drag_delta.y) / right_scrollbar->calc_size[Axis2_Y]) * content_dim[Axis2_Y];
			}

			content_box->calc_pos[Axis2_Y] = content_box->target_pos[Axis2_Y];
		}

		container->scroll.y = Clamp(0, container->scroll.y, overflow_y);

		if (reverse_scroll_y)
		{
			scroll_y = overflow_y - container->scroll.y;
		}
		else
		{
			scroll_y = container->scroll.y;
		}


		UI_Parent(right_scrollbar)
		{
			if (content_dim[Axis2_Y] != 0)
			{
				if (overflow_y != 0)
				{
					F32 spacer_size = roundf(((scroll_y) / overflow_y) * (right_scrollbar->calc_size[Axis2_Y] - ((view_dim[Axis2_Y] / content_dim[Axis2_Y]) * right_scrollbar->calc_size[Axis2_Y])));
					UI_Spacer(UI_Pixels(spacer_size));
				}

				UI_NextBackgroundColor(V4(0.0f, 0.3f, 0.4f, 1.0f));
				UI_NextSize2(UI_Pct(1), UI_Pct(view_dim[Axis2_Y] / content_dim[Axis2_Y]));
				if (scrolling_with_mousewheel)
				{
					UI_NextBoxFlags(UI_BoxFlag_AnimateX | UI_BoxFlag_AnimateY);
				}
				UI_Box *right_scrollbar_button = UI_BoxMake(UI_BoxFlag_DrawBorder |
				                                            UI_BoxFlag_DrawBackground,
				                                            Str8Lit("Right scrollbar button"));

				if (right_scrollbar_button->calc_pos[Axis2_Y] != right_scrollbar_button->target_pos[Axis2_Y])
				{
					right_scrollbar_button->flags |= UI_BoxFlag_AnimateX | UI_BoxFlag_AnimateY;
				}
			}
		}
	}

	UI_Row()
	{
		UI_NextSize2(UI_Pixels(container_width - scrollbar_size), UI_Pixels(scrollbar_size));
		UI_Box *down_scrollbar = UI_BoxMake(UI_BoxFlag_DrawBorder |
		                                    UI_BoxFlag_DrawBackground |
		                                    UI_BoxFlag_HotAnimation |
		                                    UI_BoxFlag_Clickable |
		                                    UI_BoxFlag_ActiveAnimation,
		                                    Str8Lit("Down border"));
		UI_Comm down_scrollbar_comm = UI_CommFromBox(down_scrollbar);

		if (down_scrollbar_comm.dragging)
		{
			container->scroll.x += ((down_scrollbar_comm.drag_delta.x) / (down_scrollbar->calc_size[Axis2_X])) * content_dim[Axis2_X];
		}

		container->scroll.x = Clamp(0, container->scroll.x, overflow_x);
		UI_Parent(down_scrollbar)
		{
			if (content_dim[Axis2_X] != 0)
			{
				if (overflow_x != 0)
				{
					UI_Spacer(UI_Pixels((container->scroll.x / overflow_x) * (down_scrollbar->calc_size[Axis2_X] - ((view_dim[Axis2_X] / content_dim[Axis2_X]) * down_scrollbar->calc_size[Axis2_X]))));
				}
				UI_NextBackgroundColor(V4(0.0f, 0.3f, 0.4f, 1.0f));
				UI_NextSize2(UI_Pct(view_dim[Axis2_X] / content_dim[Axis2_X]), UI_Pct(1));
				UI_Box *down_scrollbar_button = UI_BoxMake(UI_BoxFlag_DrawBorder |
				                                           UI_BoxFlag_DrawBackground, Str8Lit("Down scrollbar button"));
			}
		}

		UI_NextSize2(UI_Pixels(scrollbar_size), UI_Pixels(scrollbar_size));
		UI_BoxMake(UI_BoxFlag_DrawBorder | UI_BoxFlag_DrawBackground, Str8Lit(""));
	}

	UI_PushParent(view_box);

	UI_Spacer(UI_Pixels(-container->scroll.y));

	UI_BeginRow();

	UI_Spacer(UI_Pixels(-container->scroll.x));

	UI_NextSize2(UI_SumOfChildren(), UI_SumOfChildren());
	UI_NextBackgroundColor(V4(1.0f, 0.0f, 1.0f, 1.0f));

	if (scrolling_with_mousewheel)
	{
		UI_NextBoxFlags(UI_BoxFlag_AnimateY);
	}
	UI_NextChildLayoutAxis(Axis2_Y);
	UI_NextChildLayoutCorner(UI_Corner_BottomLeft);
	content_box = UI_BoxMake(0,
	                         Str8Lit("ScrollCalcBox"));

	if (content_box->calc_pos[Axis2_Y] != content_box->target_pos[Axis2_Y])
	{
		content_box->flags |= UI_BoxFlag_AnimateX | UI_BoxFlag_AnimateY;
	}

	UI_PushParent(content_box);
	view_box->calc_rect = R_IntersectRectF32(view_box->calc_rect, content_box->calc_rect);
	R_PushClipRect(view_box->calc_rect);
	content_box->scroll.y = container->scroll.y;

	return(content_box);
}