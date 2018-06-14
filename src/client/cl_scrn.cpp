/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.
Copyright (C) 2000-2013 Darklegion Development
Copyright (C) 2015-2018 GrangerHub

This file is part of Tremulous.

Tremulous is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Tremulous is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Tremulous; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
// cl_scrn.c -- master for refresh, status bar, console, chat, notify, etc

#include "imgui.h"

#include "client.h"

bool scr_initialized;  // ready to draw

cvar_t *cl_timegraph;
cvar_t *cl_debuggraph;
cvar_t *cl_graphheight;
cvar_t *cl_graphscale;
cvar_t *cl_graphshift;

ImGuiIO& io = ImGui::GetIO();
void IMGUI_Init()
{
    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
}

void IMGUI_Exit()
{
    ImGui::DestroyContext();
}

void IMGUI_Run()
{
    ImGui::StyleColorsDark();

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    SDL_Event event;
    while ( SDL_PollEvent(&event) )
    {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if ( event.type == SDL_QUIT )
                    done = true;

            if ( event.type == SDL_WINDOWEVENT
              && event.window.event == SDL_WINDOWEVENT_CLOSE
              && event.window.windowID == SDL_GetWindowID(window) )
                    done = true;
    }
    
    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();
    
    // 1. Show a simple window.
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets
    // automatically appears in a window called "Debug".
    {
        static float f = 0.0f;
        static int counter = 0;
        ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
        
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
        ImGui::Checkbox("Another Window", &show_another_window);
        
        if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
                counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);
        
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f
                        / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate); 
    }
    
    // 2. Show another simple window. In most cases you will use an explicit
    // Begin/End pair to name your windows.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
                show_another_window = false;
        ImGui::End();
    }
    
    // 3. Show the ImGui demo window. Most of the sample code is in
    // ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
    if (show_demo_window)
    {
        // Normally user code doesn't need/want to call this because
        // positions are saved in .ini file anyway. Here we just want to
        // make the demo initial state a bit more friendly!
        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
        ImGui::ShowDemoWindow(&show_demo_window);
    }
    
    // Rendering
    ImGui::Render();

    SDL_GL_MakeCurrent(window, gl_context);
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/*
================
SCR_DrawNamedPic

Coordinates are 640*480 virtual values
=================
*/
void SCR_DrawNamedPic(float x, float y, float width, float height, const char *picname)
{
    qhandle_t hShader;

    assert(width != 0);

    hShader = re.RegisterShader(picname);
    SCR_AdjustFrom640(&x, &y, &width, &height);
    re.DrawStretchPic(x, y, width, height, 0, 0, 1, 1, hShader);
}

/*
================
SCR_AdjustFrom640

Adjusted for resolution and screen aspect ratio
================
*/
void SCR_AdjustFrom640(float *x, float *y, float *w, float *h)
{
    float xscale;
    float yscale;

#if 0
		// adjust for wide screens
		if ( cls.glconfig.vidWidth * 480 > cls.glconfig.vidHeight * 640 ) {
			*x += 0.5 * ( cls.glconfig.vidWidth - ( cls.glconfig.vidHeight * 640 / 480 ) );
		}
#endif

    // scale for screen sizes
    xscale = cls.glconfig.vidWidth / 640.0;
    yscale = cls.glconfig.vidHeight / 480.0;
    if (x)
    {
        *x *= xscale;
    }
    if (y)
    {
        *y *= yscale;
    }
    if (w)
    {
        *w *= xscale;
    }
    if (h)
    {
        *h *= yscale;
    }
}

/*
================
SCR_FillRect

Coordinates are 640*480 virtual values
=================
*/
void SCR_FillRect(float x, float y, float width, float height, const float *color)
{
    re.SetColor(color);

    SCR_AdjustFrom640(&x, &y, &width, &height);
    re.DrawStretchPic(x, y, width, height, 0, 0, 0, 0, cls.whiteShader);

    re.SetColor(NULL);
}

/*
================
SCR_DrawPic

Coordinates are 640*480 virtual values
=================
*/
void SCR_DrawPic(float x, float y, float width, float height, qhandle_t hShader)
{
    SCR_AdjustFrom640(&x, &y, &width, &height);
    re.DrawStretchPic(x, y, width, height, 0, 0, 1, 1, hShader);
}

/*
** SCR_DrawChar
** chars are drawn at 640*480 virtual screen size
*/
static void SCR_DrawChar(int x, int y, float size, int ch)
{
    int row, col;
    float frow, fcol;
    float ax, ay, aw, ah;

    ch &= 255;

    if (ch == ' ')
    {
        return;
    }

    if (y < -size)
    {
        return;
    }

    ax = x;
    ay = y;
    aw = size;
    ah = size;
    SCR_AdjustFrom640(&ax, &ay, &aw, &ah);

    row = ch >> 4;
    col = ch & 15;

    frow = row * 0.0625;
    fcol = col * 0.0625;
    size = 0.0625;

    re.DrawStretchPic(ax, ay, aw, ah, fcol, frow, fcol + size, frow + size, cls.charSetShader);
}

/*
** SCR_DrawSmallChar
** small chars are drawn at native screen resolution
*/
void SCR_DrawSmallChar(int x, int y, int ch)
{
    int row, col;
    float frow, fcol;
    float size;

    ch &= 255;

    if (ch == ' ')
    {
        return;
    }

    if (y < -SMALLCHAR_HEIGHT)
    {
        return;
    }

    row = ch >> 4;
    col = ch & 15;

    frow = row * 0.0625;
    fcol = col * 0.0625;
    size = 0.0625;

    re.DrawStretchPic(x, y, SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT, fcol, frow, fcol + size, frow + size, cls.charSetShader);
}

/*
==================
SCR_DrawBigString[Color]

Draws a multi-colored string with a drop shadow, optionally forcing
to a fixed color.

Coordinates are at 640 by 480 virtual resolution
==================
*/
static void SCR_DrawStringExt(
    int x, int y, float size, const char *string, float *setColor, bool forceColor, bool noColorEscape)
{
    vec4_t color;
    const char *s;
    int xx;

    // draw the drop shadow
    color[0] = color[1] = color[2] = 0;
    color[3] = setColor[3];
    re.SetColor(color);
    s = string;
    xx = x;
    while (*s)
    {
        if (!noColorEscape && Q_IsColorString(s))
        {
            s += 2;
            continue;
        }
        SCR_DrawChar(xx + 2, y + 2, size, *s);
        xx += size;
        s++;
    }

    // draw the colored text
    s = string;
    xx = x;
    re.SetColor(setColor);
    while (*s)
    {
        if (Q_IsColorString(s))
        {
            if (!forceColor)
            {
                ::memcpy(color, g_color_table[ColorIndex(*(s + 1))], sizeof(color));
                color[3] = setColor[3];
                re.SetColor(color);
            }
            if (!noColorEscape)
            {
                s += 2;
                continue;
            }
        }
        SCR_DrawChar(xx, y, size, *s);
        xx += size;
        s++;
    }
    re.SetColor(NULL);
}

void SCR_DrawBigString(int x, int y, const char *s, float alpha, bool noColorEscape)
{
    float color[4];

    color[0] = color[1] = color[2] = 1.0;
    color[3] = alpha;
    SCR_DrawStringExt(x, y, BIGCHAR_WIDTH, s, color, false, noColorEscape);
}

void SCR_DrawBigStringColor(int x, int y, const char *s, vec4_t color, bool noColorEscape)
{
    SCR_DrawStringExt(x, y, BIGCHAR_WIDTH, s, color, true, noColorEscape);
}

/*
==================
SCR_DrawSmallString[Color]

Draws a multi-colored string with a drop shadow, optionally forcing
to a fixed color.
==================
*/
void SCR_DrawSmallStringExt(int x, int y, const char *string, float *setColor, bool forceColor, bool noColorEscape)
{
    vec4_t color;
    const char *s;
    int xx;

    // draw the colored text
    s = string;
    xx = x;
    re.SetColor(setColor);
    while (*s)
    {
        if (Q_IsColorString(s))
        {
            if (!forceColor)
            {
                ::memcpy(color, g_color_table[ColorIndex(*(s + 1))], sizeof(color));
                color[3] = setColor[3];
                re.SetColor(color);
            }
            if (!noColorEscape)
            {
                s += 2;
                continue;
            }
        }
        SCR_DrawSmallChar(xx, y, *s);
        xx += SMALLCHAR_WIDTH;
        s++;
    }
    re.SetColor(NULL);
}

/*
** SCR_Strlen -- skips color escape codes
*/
static int SCR_Strlen(const char *str)
{
    const char *s = str;
    int count = 0;

    while (*s)
    {
        if (Q_IsColorString(s))
        {
            s += 2;
        }
        else
        {
            count++;
            s++;
        }
    }

    return count;
}

/*
** SCR_GetBigStringWidth
*/
int SCR_GetBigStringWidth(const char *str) { return SCR_Strlen(str) * BIGCHAR_WIDTH; }
//===============================================================================

#ifdef USE_VOIP
/*
=================
SCR_DrawVoipMeter

FIXME: inherited from ioq3, move to cgame/ui
=================
*/
void SCR_DrawVoipMeter(void)
{
    char buffer[16];
    char string[256];
    int limit, i;

    if (!cl_voipShowMeter->integer)
        return;  // player doesn't want to show meter at all.
    else if (!cl_voipSend->integer)
        return;  // not recording at the moment.
    else if (clc.state != CA_ACTIVE)
        return;  // not connected to a server.
    else if (!clc.voipEnabled)
        return;  // server doesn't support VoIP.
    else if (clc.demoplaying)
        return;  // playing back a demo.
    else if (!cl_voip->integer)
        return;  // client has VoIP support disabled.

    limit = (int)(clc.voipPower * 10.0f);
    if (limit > 10) limit = 10;

    for (i = 0; i < limit; i++) buffer[i] = '*';
    while (i < 10) buffer[i++] = ' ';
    buffer[i] = '\0';

    sprintf(string, "VoIP: [%s]", buffer);
    SCR_DrawStringExt(320 - strlen(string) * 4, 10, 8, string, g_color_table[7], true, false);
}
#endif

/*
===============================================================================

DEBUG GRAPH

===============================================================================
*/

static int current;
static float values[1024];

/*
==============
SCR_DebugGraph
==============
*/
void SCR_DebugGraph(float value)
{
    values[current] = value;
    current = (current + 1) % ARRAY_LEN(values);
}

/*
==============
SCR_DrawDebugGraph
==============
*/
void SCR_DrawDebugGraph(void)
{
    int a, x, y, w, i, h;
    float v;

    //
    // draw the graph
    //
    w = cls.glconfig.vidWidth;
    x = 0;
    y = cls.glconfig.vidHeight;
    re.SetColor(g_color_table[0]);
    re.DrawStretchPic(x, y - cl_graphheight->integer, w, cl_graphheight->integer, 0, 0, 0, 0, cls.whiteShader);
    re.SetColor(NULL);

    for (a = 0; a < w; a++)
    {
        i = (ARRAY_LEN(values) + current - 1 - (a % ARRAY_LEN(values))) % ARRAY_LEN(values);
        v = values[i];
        v = v * cl_graphscale->integer + cl_graphshift->integer;

        if (v < 0) v += cl_graphheight->integer * (1 + (int)(-v / cl_graphheight->integer));
        h = (int)v % cl_graphheight->integer;
        re.DrawStretchPic(x + w - 1 - a, y - h, 1, h, 0, 0, 0, 0, cls.whiteShader);
    }
}

//=============================================================================

/*
==================
SCR_Init
==================
*/
void SCR_Init(void)
{
    cl_timegraph = Cvar_Get("timegraph", "0", CVAR_CHEAT);
    cl_debuggraph = Cvar_Get("debuggraph", "0", CVAR_CHEAT);
    cl_graphheight = Cvar_Get("graphheight", "32", CVAR_CHEAT);
    cl_graphscale = Cvar_Get("graphscale", "1", CVAR_CHEAT);
    cl_graphshift = Cvar_Get("graphshift", "0", CVAR_CHEAT);

    scr_initialized = true;
}

//=======================================================

/*
==================
SCR_DrawScreenField

This will be called twice if rendering in stereo mode
==================
*/
void SCR_DrawScreenField(stereoFrame_t stereoFrame)
{
    bool uiFullscreen;

    re.BeginFrame(stereoFrame);

    uiFullscreen = (cls.ui && VM_Call(cls.ui, UI_IS_FULLSCREEN - (cls.uiInterface == 2 ? 2 : 0)));

    // wide aspect ratio screens need to have the sides cleared
    // unless they are displaying game renderings
    if (uiFullscreen || clc.state < CA_LOADING)
    {
        if (cls.glconfig.vidWidth * 480 > cls.glconfig.vidHeight * 640)
        {
            re.SetColor(g_color_table[0]);
            re.DrawStretchPic(0, 0, cls.glconfig.vidWidth, cls.glconfig.vidHeight, 0, 0, 0, 0, cls.whiteShader);
            re.SetColor(NULL);
        }
    }

    // if the menu is going to cover the entire screen, we
    // don't need to render anything under it
    if (cls.ui && !uiFullscreen)
    {
        switch (clc.state)
        {
            default:
                Com_Error(ERR_FATAL, "SCR_DrawScreenField: bad clc.state");
                break;
            case CA_CINEMATIC:
                SCR_DrawCinematic();
                break;
            case CA_DISCONNECTED:
                // force menu up
                S_StopAllSounds();
                VM_Call(cls.ui, UI_SET_ACTIVE_MENU - (cls.uiInterface == 2 ? 2 : 0), UIMENU_MAIN);
                break;
            case CA_CONNECTING:
            case CA_CHALLENGING:
            case CA_CONNECTED:
                // connecting clients will only show the connection dialog
                // refresh to update the time
                VM_Call(cls.ui, UI_REFRESH - (cls.uiInterface == 2 ? 2 : 0), cls.realtime);
                VM_Call(cls.ui, UI_DRAW_CONNECT_SCREEN - (cls.uiInterface == 2 ? 2 : 0), false);
                break;
            case CA_LOADING:
            case CA_PRIMED:
                // draw the game information screen and loading progress
                CL_CGameRendering(stereoFrame);
                break;
            case CA_ACTIVE:
                // always supply STEREO_CENTER as vieworg offset is now done by the engine.
                CL_CGameRendering(stereoFrame);
#ifdef USE_VOIP
                SCR_DrawVoipMeter();
#endif
                break;
        }
    }

    // the menu draws next
    if (Key_GetCatcher() & KEYCATCH_UI && cls.ui)
    {
        VM_Call(cls.ui, UI_REFRESH - (cls.uiInterface == 2 ? 2 : 0), cls.realtime);
    }

    // console draws next
    Con_DrawConsole();

    // debug graph can be drawn on top of anything
    if (cl_debuggraph->integer || cl_timegraph->integer || cl_debugMove->integer)
    {
        SCR_DrawDebugGraph();
    }
}

/*
==================
SCR_UpdateScreen

This is called every frame, and can also be called explicitly to flush
text to the screen.
==================
*/
void SCR_UpdateScreen(void)
{
    static int recursive;

    if (!scr_initialized)
    {
        return;  // not initialized yet
    }

    if (++recursive > 2)
    {
        Com_Error(ERR_FATAL, "SCR_UpdateScreen: recursively called");
    }
    recursive = 1;

    // If there is no VM, there are also no rendering commands issued. Stop the renderer in
    // that case.
    if (cls.ui || com_dedicated->integer)
    {
        // XXX
        int in_anaglyphMode = Cvar_VariableIntegerValue("r_anaglyphMode");
        // if running in stereo, we need to draw the frame twice
        if (cls.glconfig.stereoEnabled || in_anaglyphMode)
        {
            SCR_DrawScreenField(STEREO_LEFT);
            SCR_DrawScreenField(STEREO_RIGHT);
        }
        else
        {
            SCR_DrawScreenField(STEREO_CENTER);
        }

	// wtfbbqhax - IMGUI HERE?
        if (com_speeds->integer)
        {
            re.EndFrame(&time_frontend, &time_backend);
        }
        else
        {
            re.EndFrame(NULL, NULL);
        }
    }

    recursive = 0;
}
