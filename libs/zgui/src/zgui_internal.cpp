#include "./imgui/imgui.h"
#include "./imgui/imgui_internal.h"

#ifndef ZGUI_API
#define ZGUI_API
#endif

extern "C" {

//--------------------------------------------------------------------------------------------------
//
// Window Settings
//
//--------------------------------------------------------------------------------------------------
ZGUI_API void zguiWindowSettings_populate() {
    ImGuiContext& g = *GImGui;
    for (int i = 0; i != g.Windows.Size; i++) {
        ImGuiWindow* window = g.Windows[i];
        if (window->Flags & ImGuiWindowFlags_NoSavedSettings)
            continue;

        ImGuiWindowSettings* settings = ImGui::FindWindowSettingsByWindow(window);
        if (!settings) {
            settings = ImGui::CreateNewWindowSettings(window->Name);
            window->SettingsOffset = g.SettingsWindows.offset_from_ptr(settings);
        }
        IM_ASSERT(settings->ID == window->ID);
        settings->Pos = ImVec2ih(window->Pos);
        settings->Size = ImVec2ih(window->SizeFull);
        settings->Collapsed = window->Collapsed;
        settings->WantDelete = false;
    }
}

ZGUI_API int zguiWindowSettings_size() {
    return GImGui->SettingsWindows.size();
}

ZGUI_API ImGuiWindowSettings* zguiWindowSettings_begin() {
    return GImGui->SettingsWindows.begin();
}

ZGUI_API ImGuiWindowSettings* zguiWindowSettings_next(ImGuiWindowSettings* prev) {
    return GImGui->SettingsWindows.next_chunk(prev);
}

ZGUI_API void zguiWindowSettings_update(const char* name, short pos_x, short pos_y, short size_x, short size_y, bool collapsed) {
    ImGuiID id = ImHashStr(name);
    ImGuiWindowSettings* settings = ImGui::FindWindowSettingsByID(id);
    if (settings)
        *settings = ImGuiWindowSettings(); // Clear existing if recycling previous entry
    else
        settings = ImGui::CreateNewWindowSettings(name);

    settings->ID = id;
    settings->Pos = ImVec2ih(pos_x, pos_y);
    settings->Size = ImVec2ih(size_x, size_y);
    settings->Collapsed = collapsed;
    settings->WantApply = true;
}

ZGUI_API void zguiWindowSettings_setLoaded() {
    GImGui->SettingsLoaded = true;
}

//--------------------------------------------------------------------------------------------------

} /* extern "C" */
