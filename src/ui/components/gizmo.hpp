#pragma once

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#ifdef USE_IMGUI_API
#include "imconfig.h"
#endif
#ifndef IMGUI_API
#define IMGUI_API
#endif

namespace ImGuizmo
{
    // call inside your own window and before Manipulate() in order to draw gizmo to that window.
    // Or pass a specific ImDrawList to draw to (e.g. ImGui::GetForegroundDrawList()).
    IMGUI_API void SetDrawlist(ImDrawList *drawlist = nullptr);

    // call BeginFrame right after ImGui_XXXX_NewFrame();
    IMGUI_API void BeginFrame();

    // this is necessary because when imguizmo is compiled into a dll, and imgui into another
    // globals are not shared between them.
    // More details at https://stackoverflow.com/questions/19373061/what-happens-to-global-and-static-variables-in-a-shared-library-when-it-is-dynam
    // expose method to set imgui context
    IMGUI_API void SetImGuiContext(ImGuiContext *ctx);

    // return true if mouse cursor is over any gizmo control (axis, plan or screen component)
    IMGUI_API bool IsOver();

    // return true if mouse IsOver or if the gizmo is in moving state
    IMGUI_API bool IsUsing();

    // return true if any gizmo is in moving state
    IMGUI_API bool IsUsingAny();

    // enable/disable the gizmo. Stay in the state until next call to Enable.
    // gizmo is rendered with gray half transparent color when disabled
    IMGUI_API void Enable(bool enable);

    // helper functions for manualy editing translation/rotation/scale with an input float
    // translation, rotation and scale float points to 3 floats each
    // Angles are in degrees (more suitable for human editing)
    // example:
    // float matrixTranslation[3], matrixRotation[3], matrixScale[3];
    // ImGuizmo::DecomposeMatrixToComponents(gizmoMatrix.m16, matrixTranslation, matrixRotation, matrixScale);
    // ImGui::InputFloat3("Tr", matrixTranslation, 3);
    // ImGui::InputFloat3("Rt", matrixRotation, 3);
    // ImGui::InputFloat3("Sc", matrixScale, 3);
    // ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, gizmoMatrix.m16);
    //
    // These functions have some numerical stability issues for now. Use with caution.
    IMGUI_API void DecomposeMatrixToComponents(const float *matrix, float *translation, float *rotation, float *scale);
    IMGUI_API void RecomposeMatrixFromComponents(const float *translation, const float *rotation, const float *scale, float *matrix);

    IMGUI_API void SetRect(float x, float y, float width, float height);
    // default is false
    IMGUI_API void SetOrthographic(bool isOrthographic);

    // Render a cube with face color corresponding to face normal. Usefull for debug/tests
    IMGUI_API void DrawCubes(const float *view, const float *projection, const float *matrices, int matrixCount);
    IMGUI_API void DrawGrid(const float *view, const float *projection, const float *matrix, const float gridSize);

    // call it when you want a gizmo
    // Needs view and projection matrices.
    // matrix parameter is the source matrix (where will be gizmo be drawn) and might be transformed by the function. Return deltaMatrix is optional
    // translation is applied in world space
    enum OPERATION
    {
        TRANSLATE_X = (1u << 0),
        TRANSLATE_Y = (1u << 1),
        TRANSLATE_Z = (1u << 2),
        ROTATE_X = (1u << 3),
        ROTATE_Y = (1u << 4),
        ROTATE_Z = (1u << 5),
        ROTATE_SCREEN = (1u << 6),
        SCALE_X = (1u << 7),
        SCALE_Y = (1u << 8),
        SCALE_Z = (1u << 9),
        BOUNDS = (1u << 10),
        SCALE_XU = (1u << 11),
        SCALE_YU = (1u << 12),
        SCALE_ZU = (1u << 13),

        TRANSLATE = TRANSLATE_X | TRANSLATE_Y | TRANSLATE_Z,
        ROTATE = ROTATE_X | ROTATE_Y | ROTATE_Z | ROTATE_SCREEN,
        SCALE = SCALE_X | SCALE_Y | SCALE_Z,
        SCALEU = SCALE_XU | SCALE_YU | SCALE_ZU, // universal
        UNIVERSAL = TRANSLATE | ROTATE | SCALEU
    };

    inline OPERATION operator|(OPERATION lhs, OPERATION rhs)
    {
        return static_cast<OPERATION>(static_cast<int>(lhs) | static_cast<int>(rhs));
    }

    enum MODE
    {
        LOCAL,
        WORLD
    };

    IMGUI_API bool Manipulate(const float *view, const float *projection, OPERATION operation, MODE mode, float *matrix, float *deltaMatrix = NULL, const float *snap = NULL, const float *localBounds = NULL, const float *boundsSnap = NULL);
    //
    // Please note that this cubeview is patented by Autodesk : https://patents.google.com/patent/US7782319B2/en
    // It seems to be a defensive patent in the US. I don't think it will bring troubles using it as
    // other software are using the same mechanics. But just in case, you are now warned!
    //
    IMGUI_API void ViewManipulate(float *view, float length, ImVec2 position, ImVec2 size, ImU32 backgroundColor);

    // use this version if you did not call Manipulate before and you are just using ViewManipulate
    IMGUI_API void ViewManipulate(float *view, const float *projection, OPERATION operation, MODE mode, float *matrix, float length, ImVec2 position, ImVec2 size, ImU32 backgroundColor);

    IMGUI_API void SetID(int id);

    // return true if the cursor is over the operation's gizmo
    IMGUI_API bool IsOver(OPERATION op);
    IMGUI_API void SetGizmoSizeClipSpace(float value);

    // Allow axis to flip
    // When true (default), the guizmo axis flip for better visibility
    // When false, they always stay along the positive world/local axis
    IMGUI_API void AllowAxisFlip(bool value);

    // Configure the limit where axis are hidden
    IMGUI_API void SetAxisLimit(float value);
    // Configure the limit where planes are hiden
    IMGUI_API void SetPlaneLimit(float value);

    enum COLOR
    {
        DIRECTION_X,      // directionColor[0]
        DIRECTION_Y,      // directionColor[1]
        DIRECTION_Z,      // directionColor[2]
        PLANE_X,          // planeColor[0]
        PLANE_Y,          // planeColor[1]
        PLANE_Z,          // planeColor[2]
        SELECTION,        // selectionColor
        INACTIVE,         // inactiveColor
        TRANSLATION_LINE, // translationLineColor
        SCALE_LINE,
        ROTATION_USING_BORDER,
        ROTATION_USING_FILL,
        HATCHED_AXIS_LINES,
        TEXT,
        TEXT_SHADOW,
        COUNT
    };

    struct Style
    {
        IMGUI_API Style();

        float TranslationLineThickness;   // Thickness of lines for translation gizmo
        float TranslationLineArrowSize;   // Size of arrow at the end of lines for translation gizmo
        float RotationLineThickness;      // Thickness of lines for rotation gizmo
        float RotationOuterLineThickness; // Thickness of line surrounding the rotation gizmo
        float ScaleLineThickness;         // Thickness of lines for scale gizmo
        float ScaleLineCircleSize;        // Size of circle at the end of lines for scale gizmo
        float HatchedAxisLineThickness;   // Thickness of hatched axis lines
        float CenterCircleSize;           // Size of circle at the center of the translate/scale gizmo

        ImVec4 Colors[COLOR::COUNT];
    };

    IMGUI_API Style &GetStyle();
}
