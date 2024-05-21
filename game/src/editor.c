#include "editor.h"
#include "body.h"
#include "render.h"

#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"

bool ncEditorActive = true;
bool ncEditorIntersect = false;
ncEditorData_t ncEditorData;

Rectangle editorRect;

Vector2 anchor01 = { 850, 50 };
Texture2D cursorTexture;

bool EditorBoxActive = true;

void InitEditor()
{
    GuiLoadStyle("raygui/styles/ashes/style_ashes.rgs");

    ncEditorData.MassValue = 10;
    ncEditorData.GravitationValue = 2;
    ncEditorData.GravityValue = -10;
    ncEditorData.BodyTypeEditMode = false;
    ncEditorData.BodyTypeActive = 0;
    ncEditorData.DampingValue = 0.0f;
    ncEditorData.GravityScaleValue = 1.0f;
    ncEditorData.StiffnessValue = 0.0f;
    ncEditorData.FixedTimeStepValue = 0.02f;

    ncEditorData.Simulate = true;
    ncEditorData.Reset = false;

    editorRect = (Rectangle){ anchor01.x + 0, anchor01.y + 0, 304, 616 };
}

void UpdateEditor(Vector2 position)
{
    // toggle show / hide editor box with key press
    if (IsKeyPressed(KEY_TAB)) EditorBoxActive = !EditorBoxActive;

    // check if cursor position is intersecting the editor box
    ncEditorIntersect = EditorBoxActive && CheckCollisionPointRec(position, editorRect);
}

void DrawEditor()
{
    if (ncEditorData.BodyTypeEditMode) GuiLock();

    if (EditorBoxActive)
    {
        EditorBoxActive = !GuiWindowBox((Rectangle) { anchor01.x + 0, anchor01.y + 0, 304, 616 }, "Editor");

        GuiGroupBox((Rectangle) { anchor01.x + 10, anchor01.y + 40, 280, 270 }, "Body");
        GuiSliderBar((Rectangle) { anchor01.x + 100, anchor01.y + 90, 150, 16 }, "Mass", TextFormat("%0.2f", ncEditorData.MassValue), & ncEditorData.MassValue, 0, 100);
        GuiSliderBar((Rectangle) { anchor01.x + 100, anchor01.y + 130, 150, 16 }, "Damping", TextFormat("%0.2f", ncEditorData.DampingValue), & ncEditorData.DampingValue, 0, 100);
        GuiSliderBar((Rectangle) { anchor01.x + 100, anchor01.y + 170, 150, 16 }, "Gravity Scale", TextFormat("%0.2f", ncEditorData.GravityScaleValue), & ncEditorData.GravityScaleValue, 1, 10);
        GuiSliderBar((Rectangle) { anchor01.x + 100, anchor01.y + 210, 150, 16 }, "Stiffness", TextFormat("%0.2f", ncEditorData.StiffnessValue), & ncEditorData.StiffnessValue, 0, 100);

        GuiGroupBox((Rectangle) { anchor01.x + 10, anchor01.y + 320, 280, 270 }, "World");
        GuiSliderBar((Rectangle) { anchor01.x + 100, anchor01.y + 340, 150, 16 }, "Gravity", TextFormat("%0.2f", ncEditorData.GravityValue), & ncEditorData.GravityValue, -100, 100);
        GuiSliderBar((Rectangle) { anchor01.x + 100, anchor01.y + 380, 150, 16 }, "Gravitation", TextFormat("%0.2f", ncEditorData.GravitationValue), & ncEditorData.GravitationValue, 0, 100);
        GuiSliderBar((Rectangle) { anchor01.x + 100, anchor01.y + 410, 150, 16 }, "FPS", TextFormat("%0.002f", ncEditorData.FixedTimeStepValue), & ncEditorData.FixedTimeStepValue, 0.01f, 0.05f);

        GuiToggle((Rectangle) { anchor01.x + 175, anchor01.y + 450, 75, 30 }, "Simulate", &ncEditorData.Simulate);
        GuiToggle((Rectangle) { anchor01.x + 50, anchor01.y + 450, 75, 30 }, "Reset", &ncEditorData.Reset);

        if (GuiDropdownBox((Rectangle) { anchor01.x + 100, anchor01.y + 50, 150, 20 }, "DYNAMIC;KINEMATIC;STATIC", & ncEditorData.BodyTypeActive, ncEditorData.BodyTypeEditMode)) ncEditorData.BodyTypeEditMode = !ncEditorData.BodyTypeEditMode;
    }

    GuiUnlock();
}

ncBody* GetBodyIntersect(ncBody* bodies, Vector2 position)
{
    for (ncBody* body = bodies; body; body = body->next)
    {
        Vector2 screen = ConvertWorldToScreen(body->position);
        if (CheckCollisionPointCircle(position, screen, ConvertWorldToPixel(body->mass)))
        {
            return body;
        }
    }

    return NULL;
}

void DrawLineBodyToPosition(ncBody* body, Vector2 position)
{
    Vector2 screen = ConvertWorldToScreen(body->position);
    DrawLine((int)screen.x, (int)screen.y, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, YELLOW);
}
