#include "Includes/Logger.h"
#include "Includes/Macros.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.h"
#include "ImGui/Call_ImGui.h"
#include "IL2CppSDKGenerator/BasicStructs/Call_BasicStructs.h"
#include "IL2CppSDKGenerator/IL2Cpp/Call_IL2Cpp.h"
#include "Hacks/Hacks.h"
#include "IL2CppSDKGenerator/KittyMemory/MemoryPatch.h"
#include "foxcheats/include/ScanEngine.hpp"

#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <functional>
#include <cstring>
#include <cfloat>
#include <jni.h>
#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "oxorany/source/oxorany.h"
#include "oxorany/source/oxorany.cpp"
#include "oxorany/source/oxorany_include.h"

#include "MainFeatureIncludes.h"

class _BYTE;
class _BOOL4;
class _BOOL8;
class _WORD;
class _DWORD;
class _QWORD;

#define CREATE_COLOR(r, g, b, a) new float[4] {(float)(r) / 255.0f, (float)(g) / 255.0f, (float)(b) / 255.0f, (float)(a) / 255.0f}
bool ClearDisplay = false;
bool ShowFPS;
bool SnowB = false;
float SnowBsize = 0.0f;
bool isSpeedHackEnabled = false;
float speedHackMultiplier = 1.0f;
bool RedWallhackShow = false;
bool isJumpAdjustmentEnabled = false;
float jumpHeightMultiplier = 1.0f;
char logintext[4096];
float menu[4] = {188.0f / 255.0f, 110.0f / 255.0f, 1.0f, 1.0f};

float g_LastLogoOpacity = 1.0f;
float g_LastLogoSize = 1.0f;
int g_LogoHideDelayFrames = 0;
int g_LogoHideDelay = 40;

#define _BYTE uint8_t
#define _WORD  uint8_t
#define _DWORD uint64_t
#define _QWORD uint64_t
#define _BOOL4 uint8_t

#include <fstream>
using namespace std;

#include <Substrate/SubstrateHook.h>
#include <Substrate/CydiaSubstrate.h>

ImFont* F50 = nullptr;
ImFont* F107 = nullptr;
ImFont* SOCIAL = nullptr;
ImFont* Bold = nullptr;
JavaVM* jvm = nullptr;
JavaVM* VM = nullptr;

namespace font {
    ImFont* icomoon_logo = nullptr;
    ImFont* inter_semibold = nullptr;
    ImFont* icomoon_page = nullptr;
}


static int g_GlWidth, g_GlHeight;
static bool g_App = false;

struct My_Patches
{
    MemoryPatch A1, grap, NoCrouch, SpeedhackX, SpeedhackX1, NoWingsuit, fpss, frame;
} Patches;

float AVIWA = 119.167f;

bool wallh;
bool active = false;
float AimSmooth = 1.0f;

bool showKeyboard = false;
static bool g_RuntimeClearDisplayInit = false;

struct ClearDisplayDefaultInit {
    ClearDisplayDefaultInit()
    {
        Config.ExtraMenu.ClearDisplay = true;       
    }
} g_ClearDisplayDefaultInit;

struct sRegion
{
    uintptr_t start, end;
};

std::chrono::steady_clock::time_point appStartTime = std::chrono::steady_clock::now();

static float veh_min = 0.1f;
static float veh_max = 5.0f;
static bool windowCollapsed = false;
static double collapseBarLastActiveTime = 0.0;
static float collapseBarOpacityAnim = 1.0f;
static float collapseBarPressAnim = 0.0f;
static float collapseBarEnterAnim = 1.0f;
static float collapseBarRestoreAnim = 1.0f;
static bool collapseBarRestoreActive = false;
static bool collapseBarWasCollapsed = false;
static float uncollapseOpenAnim = 1.0f;
static bool dark = true;
static float tabAlpha = 0.0f;
static float tabAdd = 0.0f;
static int page = 1;
static int activeTab = 1;
bool g_LogoPreviewMode = false;
static bool isLogin = true;
static std::string err;
static std::string storedKey = "";
static char s[256];
static bool g_LoginTextLoaded = false;

std::vector<sRegion> trapRegions;
uintptr_t address = 0;
std::string md5(std::string s);
uintptr_t g_il2cpp;
static bool isMenuVisible = true;
static float tabExpandOld[6] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
static float tabGlowAlpha[6] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }; 
static float tabExpandT[6] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };     
int TABG = 1;

EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
{
    eglQuerySurface(dpy, surface, EGL_WIDTH, &g_GlWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &g_GlHeight);

    if (!g_App)
    {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = NULL;
        io.LogFilename = NULL;
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
        io.MouseDoubleClickTime = 0.3f;
        io.MouseDragThreshold = 2.f;
        ImGui_ImplOpenGL3_Init("#version 300 es");

        ImFontConfig icomoon_logo_config;
        icomoon_logo_config.MergeMode = false;
        icomoon_logo_config.PixelSnapH = true;
        icomoon_logo_config.FontDataOwnedByAtlas = false;
        font::icomoon_logo = io.Fonts->AddFontFromMemoryTTF(
            (void*)icomoon_page,
            sizeof(icomoon_page),
            20.f,
            &icomoon_logo_config
        );

        ImFontConfig inter_config;
        inter_config.MergeMode = false;
        inter_config.PixelSnapH = true;
        inter_config.FontDataOwnedByAtlas = false;
        font::inter_semibold = io.Fonts->AddFontFromMemoryTTF(
            (void*)inter_semibold,
            sizeof(inter_semibold),
            16.f,
            &inter_config
        );

        ImFontConfig page_config;
        page_config.MergeMode = false;
        page_config.PixelSnapH = true;
        page_config.FontDataOwnedByAtlas = false;
        font::icomoon_page = io.Fonts->AddFontFromMemoryTTF(
            (void*)icomoon_page,
            sizeof(icomoon_page),
            18.f,
            &page_config
        );

        static const ImWchar icons_ranges[] = { 0xe000, 0xf8ff, 0 };
        ImFontConfig iconsConfig;
        iconsConfig.MergeMode = true;
        iconsConfig.PixelSnapH = true;
        iconsConfig.OversampleH = 2.5f;
        iconsConfig.OversampleV = 2.5f;
        iconsConfig.FontDataOwnedByAtlas = false;
        F107 = io.Fonts->AddFontFromMemoryCompressedTTF(
            (void*)font_awesome_data1,
            (int)font_awesome_size1,
            25.0f,
            &iconsConfig,
            icons_ranges
        );
        F50 = io.Fonts->AddFontFromMemoryTTF((void *)F50_data, F50_size, 30.0f, NULL, io.Fonts->GetGlyphRangesDefault());
        if (!F107) {
            F107 = font::inter_semibold;
        }
        if (font::inter_semibold) {
            io.FontDefault = font::inter_semibold;
        }
        io.Fonts->Build();
        
        ImGui_ImplOpenGL3_CreateFontsTexture();

        memset(&Config, 0, sizeof(sConfig));

        Config.sColorsESPPLAYER.LinePLAYER = CREATE_COLOR(255, 0, 0, 255);
        Config.sColorsESPPLAYER.BoxPLAYER = CREATE_COLOR(255, 0, 0, 255);
        Config.sColorsESPPLAYER.NamePLAYER = CREATE_COLOR(255, 0, 0, 255);
        Config.sColorsESPPLAYER.DistancePLAYER = CREATE_COLOR(255, 0, 0, 255);
        Config.sColorsESPPLAYER.HealthPLAYER = CREATE_COLOR(255, 0, 0, 255);
        Config.sColorsESPPLAYER.SkeletonPLAYER = CREATE_COLOR(255, 0, 0, 255);
        Config.sColorsESPBOT.LineBOT = CREATE_COLOR(0, 255, 0, 180);
        Config.sColorsESPBOT.BoxBOT = CREATE_COLOR(0, 255, 0, 180);
        Config.sColorsESPBOT.NameBOT = CREATE_COLOR(0, 255, 0, 180);
        Config.sColorsESPBOT.HealthBOT = CREATE_COLOR(0, 255, 0, 180);
        Config.sColorsESPBOT.DistanceBOT = CREATE_COLOR(0, 255, 0, 180);
        Config.sColorsESPBOT.SkeletonBOT = CREATE_COLOR(0, 255, 0, 180);
        Config.sColorsESPOTHERS.PovOTHERS = CREATE_COLOR(225, 0, 0, 180);

        Config.ExtraMenu.NoGravityScale = 1.0f;
        Config.Aim.AimAssistSize = 0.0f;
        Config.Aim.Cross = 45.0f;
        Config.Aim.Target = EAimTarget::Heads;
        Config.Aim.Trigger = EAimTrigger::None;
        Config.Aim.By = EAim::Distance;

        Config.Bline = 2.0f;
        Config.Pline = 2.0f;

        g_App = true;
    }

    ImGuiIO *io = &ImGui::GetIO();
    screenWidth = (float)g_GlWidth;
    screenHeight = (float)g_GlHeight;
    io->DisplaySize = ImVec2((float)g_GlWidth, (float)g_GlHeight);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    ImDrawList *draw = ImGui::GetBackgroundDrawList();

    DrawESP(ImGui::GetBackgroundDrawList(), screenWidth, screenHeight, get_dpi());
    floating_info::Render(draw, screenWidth, screenHeight);
      
        
    if (windowCollapsed)
    {
        if (!collapseBarWasCollapsed) {
            collapseBarEnterAnim = 0.0f;
            collapseBarRestoreAnim = 1.0f;
            collapseBarRestoreActive = false;
            collapseBarOpacityAnim = 1.0f;
            collapseBarLastActiveTime = ImGui::GetTime();
        }
        collapseBarWasCollapsed = true;
        const float dt = ImGui::GetIO().DeltaTime;
        collapseBarEnterAnim = ImClamp(collapseBarEnterAnim + dt * 6.0f, 0.0f, 1.0f);
        if (collapseBarRestoreActive)
            collapseBarRestoreAnim = ImClamp(collapseBarRestoreAnim + dt * 8.0f, 0.0f, 1.0f);
        const float enterEase = collapseBarEnterAnim * collapseBarEnterAnim * (3.0f - 2.0f * collapseBarEnterAnim);
        const float restoreEase = collapseBarRestoreAnim * collapseBarRestoreAnim * (3.0f - 2.0f * collapseBarRestoreAnim);
        const float collapsedAlphaSetting = ImClamp(GetLogoOpacity(), 0.0f, 1.0f);
        const float collapsedScaleSetting = ImClamp(GetLogoSizeMultiplier(), 0.1f, 2.0f);
        const ImVec2 display = ImGui::GetIO().DisplaySize;
        const float baseLineW = 120.0f * c::scale * collapsedScaleSetting;
        const float line_w = collapseBarRestoreActive ? ImLerp(baseLineW * 1.28f, baseLineW, restoreEase) : ImLerp(baseLineW * 1.28f, baseLineW, enterEase);
        const float line_h = collapseBarRestoreActive ? ImLerp(10.0f, 6.0f, restoreEase) : ImLerp(6.0f, 10.0f, enterEase);
        const float lineH = line_h * c::scale * collapsedScaleSetting;
        const float click_w = line_w + 72.0f * c::scale;
        const float click_h = ImMax(28.0f * c::scale, lineH + 20.0f * c::scale);
        const float startY = display.y - 10.0f * c::scale - lineH;
        const float endY = display.y - 10.0f * c::scale - lineH;
        const float handleY = collapseBarRestoreActive ? ImLerp(startY, endY, restoreEase) : ImLerp(endY, startY, enterEase);
        const ImVec2 handlePos((display.x - line_w) * 0.5f, handleY);
        const ImVec2 clickPos(handlePos.x - (click_w - line_w) * 0.5f, handlePos.y - (click_h - lineH) * 0.5f);

        ImGui::SetNextWindowPos(clickPos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(click_w, click_h), ImGuiCond_Always);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

        if (ImGui::Begin("##indicator", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoSavedSettings))
        {
            ImVec2 windowPos = ImGui::GetWindowPos();
            ImGui::SetCursorPos(ImVec2(0, 0));
            ImGui::InvisibleButton("##restoreclick", ImVec2(click_w, click_h));

            bool barHeld = ImGui::IsItemActive();
            bool barHovered = ImGui::IsItemHovered();
            if (barHovered || barHeld)
                collapseBarLastActiveTime = ImGui::GetTime();
            const bool idle = (ImGui::GetTime() - collapseBarLastActiveTime) > 5.0;
            const float targetOpacity = idle ? 0.20f : 1.0f;
            collapseBarOpacityAnim = ImLerp(collapseBarOpacityAnim, targetOpacity, ImGui::GetIO().DeltaTime * 9.0f);
            collapseBarPressAnim = ImLerp(collapseBarPressAnim, barHeld ? 1.0f : 0.0f, ImGui::GetIO().DeltaTime * 22.0f);

            if (ImGui::IsItemClicked())
            {
                collapseBarRestoreActive = true;
                collapseBarRestoreAnim = 0.0f;
                collapseBarLastActiveTime = ImGui::GetTime();
            }

            float drawAlpha = ImClamp(collapseBarOpacityAnim * collapsedAlphaSetting, 0.0f, 1.0f);
            ImDrawList* indicatorDraw = ImGui::GetWindowDrawList();
            const ImVec2 lineMin(windowPos.x + (click_w - line_w) * 0.5f, windowPos.y + (click_h - lineH) * 0.5f);
            const ImVec2 lineMax(lineMin.x + line_w, lineMin.y + lineH);
            const float press = collapseBarPressAnim;
            indicatorDraw->AddRectFilled(
                lineMin,
                lineMax,
                IM_COL32(255, 255, 255, (int)((225.0f + 30.0f * press) * drawAlpha)),
                lineH * 0.5f
            );

            if (collapseBarRestoreActive && collapseBarRestoreAnim >= 1.0f)
            {
                windowCollapsed = false;
                isMenuVisible = true;
                uncollapseOpenAnim = 0.0f;
                collapseBarRestoreActive = false;
                collapseBarEnterAnim = 1.0f;
            }
        }
        ImGui::End();
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(3);
    }
    else
    {
        collapseBarWasCollapsed = false;
        collapseBarEnterAnim = 1.0f;
    }

    if (isMenuVisible && !windowCollapsed)
    {
        if (!g_RuntimeClearDisplayInit) {
            Config.ExtraMenu.ClearDisplay = true;
            g_RuntimeClearDisplayInit = true;
        }

        if (!g_LoginTextLoaded && VM != nullptr)
        {
            if (LoadTextFromFile() && logintext[0] != '\0')
            {
                strncpy(s, logintext, sizeof(s) - 1);
                s[sizeof(s) - 1] = '\0';
                g_LoginTextLoaded = true;
            }
        }

        runtime_preview_menu::EnsureTexturesLoaded();
        main_runtime_theme::ApplyThemeState();

        ImVec2 viewportCenter = ImGui::GetMainViewport()->GetCenter();
        ImVec2 displaySize = ImGui::GetIO().DisplaySize;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

if (!isLogin && !ui_loading::IsActive())
{
    ImGui::SetNextWindowPos(viewportCenter, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(500, 530), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.0f);
    
    if (ImGui::Begin(OBFUSCATE("Login Menu"), nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
    {
        const ImVec2 pos = ImGui::GetWindowPos();
        ImDrawList* draw = ImGui::GetWindowDrawList();

        const ImVec2 login_size = ImVec2(500, 530);
        const float outerRounding = 18.0f;
        const float innerInset = 13.0f;
        const float innerRounding = 13.0f;
        
        const ImVec2 innerMin = pos + ImVec2(innerInset, innerInset);
        const ImVec2 innerMax = pos + login_size - ImVec2(innerInset, innerInset);

        
        const ImU32 goldMain      = IM_COL32(255, 223, 90, 255);     
        const ImU32 goldDeep      = IM_COL32(212, 175, 55, 255);     
        const ImU32 goldLight     = IM_COL32(255, 245, 180, 255);    
        const ImU32 goldAccent    = IM_COL32(255, 215, 0, 255);
        const ImU32 bgDark        = IM_COL32(12, 10, 7, 255);        
        const ImU32 innerBg       = IM_COL32(20, 17, 12, 248);       
        const ImU32 softGlow      = IM_COL32(255, 215, 0, 28);

        
        draw->AddRectFilled(pos, pos + login_size, bgDark, outerRounding);
        
        
        draw->AddRectFilledMultiColor(pos, ImVec2(pos.x + login_size.x, pos.y + 160.0f),
            IM_COL32(255, 215, 0, 48), IM_COL32(255, 215, 0, 48),
            IM_COL32(255, 215, 0, 0), IM_COL32(255, 215, 0, 0));

        
        draw->AddRect(pos, pos + login_size, IM_COL32(255, 215, 0, 35), outerRounding, 0, 3.0f);

        
        draw->AddRectFilled(innerMin, innerMax, innerBg, innerRounding);
        
        
        draw->AddRectFilledMultiColor(
            innerMin,
            ImVec2(innerMax.x, innerMin.y + 4.0f),
            goldAccent, goldLight, goldLight, goldAccent
        );

        
        draw->AddRect(innerMin, innerMax, IM_COL32(255, 215, 0, 75), innerRounding, 0, 1.8f);

        
        {
            const char* part1 = "KAIRO";
            const char* part2 = "MOD";
            const float titleSize = 49.0f;
            ImFont* tf = F50 ? F50 : ImGui::GetFont();

            const ImVec2 s1 = tf->CalcTextSizeA(titleSize, FLT_MAX, 0.0f, part1);
            const ImVec2 s2 = tf->CalcTextSizeA(titleSize, FLT_MAX, 0.0f, part2);
            const float totalW = s1.x + s2.x;
            const float startX = pos.x + (login_size.x - totalW) * 0.5f;
            const float titleY = pos.y + 55.0f;

            
            draw->AddText(tf, titleSize, ImVec2(startX, titleY), IM_COL32(255, 255, 255, 255), part1);

            
            const float accentX = startX + s1.x;
            
            
            const ImU32 glowLayer1 = IM_COL32(255, 223, 90, 55);
            const ImU32 glowLayer2 = IM_COL32(255, 215, 0, 28);
            const ImU32 glowLayer3 = IM_COL32(255, 215, 0, 12);

            
            for (float i = -3.5f; i <= 3.5f; i += 1.0f) {
                draw->AddText(tf, titleSize, ImVec2(accentX + i, titleY), glowLayer3, part2);
                draw->AddText(tf, titleSize, ImVec2(accentX, titleY + i), glowLayer3, part2);
            }

            draw->AddText(tf, titleSize, ImVec2(accentX - 2.5f, titleY), glowLayer1, part2);
            draw->AddText(tf, titleSize, ImVec2(accentX + 2.5f, titleY), glowLayer1, part2);
            draw->AddText(tf, titleSize, ImVec2(accentX, titleY - 2.5f), glowLayer1, part2);
            draw->AddText(tf, titleSize, ImVec2(accentX, titleY + 2.5f), glowLayer1, part2);

            
            draw->AddText(tf, titleSize, ImVec2(accentX, titleY), goldMain, part2);
        }

        
        draw->AddLine(
            ImVec2(pos.x + 85, pos.y + 118),
            ImVec2(pos.x + login_size.x - 85, pos.y + 118),
            goldDeep, 1.5f
        );

        
        {
            const char* helperLine1 = "Enter your authentication key to continue";
            const char* helperLine2 = "Paste your key or type it manually";
            
            ImVec2 line1Size = ImGui::CalcTextSize(helperLine1);
            ImVec2 line2Size = ImGui::CalcTextSize(helperLine2);
            
            const ImU32 subTextColor = IM_COL32(200, 180, 140, 235);

            draw->AddText(ImVec2(pos.x + (login_size.x - line1Size.x) * 0.5f, pos.y + 135.0f), subTextColor, helperLine1);
            draw->AddText(ImVec2(pos.x + (login_size.x - line2Size.x) * 0.5f, pos.y + 155.0f), subTextColor, helperLine2);
        }

        
        const float inputWidth = 430.0f;
        const float inputHeight = 54.0f;
        const float inputX = (login_size.x - inputWidth) * 0.5f;
        ImGui::SetCursorPos(ImVec2(inputX, 195.0f));
        
        ImGui::AstralInput("##key_login", s, sizeof(s), ImVec2(inputWidth, inputHeight));

        bool loginInputClicked = ImGui::IsItemClicked();
        bool loginInputActive  = ImGui::IsItemActive();
        bool loginInputHovered = ImGui::IsItemHovered();

        if (loginInputClicked || loginInputActive)
            showKeyboard = true;

        if (showKeyboard && !loginInputActive && !loginInputHovered && ImGui::IsMouseClicked(0))
        {
            ImGuiIO& io = ImGui::GetIO();
            float screenHeight = io.DisplaySize.y;
            float keyboardHeight = screenHeight * 0.60f;
            if (ImGui::GetMousePos().y < screenHeight - keyboardHeight)
                showKeyboard = false;
        }

        
        const float buttonWidth = 430.0f;
        const float buttonX = (login_size.x - buttonWidth) * 0.5f;

        
        ImGui::SetCursorPos(ImVec2(buttonX, 268.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(35, 28, 15, 255));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(60, 48, 25, 255));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(28, 22, 12, 255));
        ImGui::PushStyleColor(ImGuiCol_Border, goldAccent);
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(245, 235, 210, 255));
        
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.3f);
        
        if (F50) ImGui::PushFont(F50);
        if (ImGui::Button("PASTE KEY", ImVec2(buttonWidth, 54.0f)))
        {
            auto key = getClipboard();
            strncpy(s, key.c_str(), sizeof(s) - 1);
            s[sizeof(s) - 1] = '\0';
        }
        if (F50) ImGui::PopFont();
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(5);

        
        const float loginBtnWidth = 325.0f;
        const float loginBtnX = (login_size.x - loginBtnWidth) * 0.5f;
        
        ImGui::SetCursorPos(ImVec2(loginBtnX, 342.0f));
        
        ImGui::PushStyleColor(ImGuiCol_Button, goldDeep);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, goldMain);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(170, 130, 30, 255));
        ImGui::PushStyleColor(ImGuiCol_Border, goldLight);
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(15, 15, 15, 255)); 
        
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 13.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.8f);
        
        if (F50) ImGui::PushFont(F50);
        if (ImGui::Button("LOGIN", ImVec2(loginBtnWidth, 56.0f)))
        {
            err = Login(s);
            if (err == "OK")
            {
                showKeyboard = false;
                strncpy(logintext, s, sizeof(logintext) - 1);
                logintext[sizeof(logintext) - 1] = '\0';
                SaveLoginTextToFile(s);
                g_LoginTextLoaded = true;
                err.clear();
                ui_loading::Start(7.0f);
            }
        }
        if (F50) ImGui::PopFont();
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(5);

        
        if (!err.empty() && err != "OK")
        {
            ImGui::SetCursorPos(ImVec2(55, 418.0f));
            ImGui::TextColored(ImColor(255, 85, 85, 255), "Error: %s", err.c_str());
        }

        if (showKeyboard)
            RenderVirtualKeyboard("##VirtualKeyboardLogin", s, sizeof(s), &showKeyboard);
    }
            ImGui::End();
        }
        else if (!isLogin && ui_loading::IsActive())
        {
            if (ui_loading::RenderWindow((ImTextureID)(intptr_t)runtime_preview_menu::g_menuBackground.id))
            {
                isLogin = true;
            }
        }
        else
        {
            uncollapseOpenAnim = ImClamp(uncollapseOpenAnim + ImGui::GetIO().DeltaTime * 5.0f, 0.0f, 1.0f);
            float openEase = uncollapseOpenAnim * uncollapseOpenAnim * (3.0f - 2.0f * uncollapseOpenAnim);
            float openAlpha = 0.2f + 0.8f * openEase;
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, openAlpha);

            ImVec2 mainWindowSize = ImVec2(780.f, 600.f);
            mainWindowSize.x = ImMin(mainWindowSize.x, displaySize.x);
            mainWindowSize.y = ImMin(mainWindowSize.y, displaySize.y);
            ImGui::SetNextWindowPos(viewportCenter, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            ImGui::SetNextWindowSize(mainWindowSize, ImGuiCond_Always);
            ImGui::SetNextWindowBgAlpha(0.0f);

            

            ImGui::Begin(
                 "@OnlyKairo",
                  nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoBackground
        );
   {
    runtime_preview_menu::StateRefs runtimeState{
        dark,
        tabAlpha,
        tabAdd,
        page,
        activeTab,
        windowCollapsed,
        isMenuVisible,
        collapseBarLastActiveTime,
        collapseBarOpacityAnim,
        collapseBarPressAnim
    };
    {
        using namespace runtime_preview_menu;
        main_runtime_theme::ApplyAccentFromHue();

        ImGuiStyle *runtimeStyle = &ImGui::GetStyle();
        runtimeStyle->ScrollbarSize = 12.0f;
        c::ApplyMainWindowStyle(*runtimeStyle);
        c::UpdateTheme(runtimeState.dark, menu, ImGui::GetIO().DeltaTime);
        main_runtime_theme::ApplyThemeState();

        const ImVec2 runtimeWindowSize = ImGui::GetWindowSize();
        const ImVec2 runtimeWindowPos = ImGui::GetWindowPos();
        ImDrawList *runtimeDrawList = ImGui::GetWindowDrawList();

       
        static bool g_perfMode = false;
        static bool pushedContentFont = false;


        const float outerPad = 16.0f;
        const float layoutGap = 8.0f;
        const float contentPadding = 12.0f;
        const float columnGap = 12.0f;
        const float sideTabWidth = 130.0f;
        const float bottomBarHeight = 60.0f;


        const float panelLeft = runtimeWindowPos.x + outerPad;
        const float panelRight = runtimeWindowPos.x + runtimeWindowSize.x - outerPad;
        const float panelTop = runtimeWindowPos.y + outerPad;
        const float panelWidth = panelRight - panelLeft; 
        const float panelBottom = runtimeWindowPos.y + runtimeWindowSize.y - outerPad; 
        const float contentBottom = panelBottom - bottomBarHeight - layoutGap; 


        
        const float headerHeight = 60.0f;
        const ImVec2 headerMin(panelLeft, panelTop);
        const ImVec2 headerMax(panelRight, panelTop + headerHeight);

        const main_runtime_theme::ThemePalette& tp = main_runtime_theme::g_themes[main_runtime_theme::g_activeTheme];
        const ImVec4& ac = c::accent;
        const ImU32 acU32 = IM_COL32((int)(ac.x*255),(int)(ac.y*255),(int)(ac.z*255),255);
        const ImU32 acSoft = IM_COL32((int)(ac.x*255),(int)(ac.y*255),(int)(ac.z*255),80);
        const ImU32 acGlow = IM_COL32((int)(ac.x*255),(int)(ac.y*255),(int)(ac.z*255),45);

        
        runtimeDrawList->AddRectFilled(headerMin, headerMax, tp.bgCap, 12.0f);
        runtimeDrawList->AddRect(headerMin, headerMax, acSoft, 12.0f, 0, 1.5f);
        runtimeDrawList->AddLine(
            ImVec2(headerMin.x + 20.0f, headerMax.y - 1.0f),
            ImVec2(headerMax.x - 20.0f, headerMax.y - 1.0f),
            acU32, 2.0f
        );

        
        {
            ImFont* titleFont = F50 ? F50 : ImGui::GetFont();
            const float titleSize = (titleFont == F50) ? 28.0f : titleFont->FontSize * 1.5f;
            const char* menuTitle = "KAIROMOD";
            const ImVec2 titleSz = titleFont->CalcTextSizeA(titleSize, FLT_MAX, 0.0f, menuTitle);
            const float titleX = headerMin.x + (panelWidth - titleSz.x) * 0.5f;
            const float titleY = headerMin.y + (headerHeight - titleSz.y) * 0.5f;
            runtimeDrawList->AddText(titleFont, titleSize + 1.2f, ImVec2(titleX + 1, titleY + 1), acGlow, menuTitle);
            runtimeDrawList->AddText(titleFont, titleSize, ImVec2(titleX, titleY), acU32, menuTitle);
        }

        
        {
            const float btnSize = 34.0f;
            const float btnX = headerMax.x - btnSize - 10.0f;
            const float btnY = headerMin.y + (headerHeight - btnSize) * 0.5f;
            ImGui::SetCursorScreenPos(ImVec2(btnX, btnY));
            ImGui::InvisibleButton("##powerBtn", ImVec2(btnSize, btnSize));
            const bool hov = ImGui::IsItemHovered();
            const bool clk = ImGui::IsItemClicked();
            if (clk) runtime_preview_menu::CollapseMenu(runtimeState);
            if (hov) runtimeDrawList->AddCircleFilled(ImVec2(btnX+btnSize*0.5f, btnY+btnSize*0.5f), btnSize*0.5f, IM_COL32(255,75,75,40), 24);
            ImFont* iconFont = custom::shell::GetIconFont();
            if (iconFont) {
                const float iconSz = hov ? 18.0f : 16.0f;
                const ImVec2 iSz = iconFont->CalcTextSizeA(iconSz, FLT_MAX, 0, ICON_FA_POWER_OFF);
                runtimeDrawList->AddText(iconFont, iconSz, ImVec2(btnX+(btnSize-iSz.x)*0.5f, btnY+(btnSize-iSz.y)*0.5f), hov ? IM_COL32(255,90,90,255) : IM_COL32(170,155,125,220), ICON_FA_POWER_OFF);
            }
        }

        
        const float tabBarLeft = panelRight - sideTabWidth;
        const float tabBarTop = headerMax.y + layoutGap;
        const float tabBarBottom = contentBottom;
        const float tabHeight = 58.0f;
        const float tabGap = 8.0f;

        runtimeDrawList->AddRectFilled(ImVec2(tabBarLeft, tabBarTop), ImVec2(panelRight, tabBarBottom), IM_COL32(15, 19, 25, 255), 12.0f);
        runtimeDrawList->AddRect(ImVec2(tabBarLeft, tabBarTop), ImVec2(panelRight, tabBarBottom), acSoft, 12.0f, 0, 1.0f);

        
        static const char* tabLabels[] = {"ESP", "AIM", "MEMORY", "SKINS", "MISC", "SETTINGS"};
        static const char* tabIcons[] = {ICON_FA_EYE, ICON_FA_CROSSHAIRS, ICON_FA_DATABASE, ICON_FA_TSHIRT, ICON_FA_SLIDERS_H, ICON_FA_COG};
        const int tabFeatureCounts[6] = {
            (int)Config.ESPMenu.isPlayerLine + (int)Config.ESPMenu.Box + (int)Config.ESPMenu.Skeleton + (int)Config.ESPMenu.Health +
            (int)Config.ESPMenu.Name + (int)Config.ESPMenu.Distance + (int)Config.ESPMenu.Count + (int)Config.ESPMenu.Alert,
            (int)Config.Aim.Aimbot360 + (int)Config.Aim.AimSilent,
            (int)Config.ExtraMenu.Recoil + (int)Config.ExtraMenu.Spread + (int)Config.ExtraMenu.Rpd + (int)Config.ExtraMenu.Fire,
            0,
            (int)Config.ExtraMenu.WallHack + (int)Config.ExtraMenu.RedWallhack + (int)Config.ExtraMenu.Parachute + (int)Config.ExtraMenu.Flash,
            0
        };
        const int tabCount = IM_ARRAYSIZE(tabLabels);
        const float tabInnerPad = 8.0f;
        const float dt = ImGui::GetIO().DeltaTime;

        runtimeState.page = ImClamp(runtimeState.page, 1, tabCount);
        runtimeState.activeTab = ImClamp(runtimeState.activeTab, 1, tabCount);

        static float tabAnim[6] = {1,0,0,0,0,0};
        for(int i=0; i<tabCount; i++){
            bool active = runtimeState.page == i+1;
            if(g_perfMode) tabAnim[i] = active ? 1.0f : 0.0f;
            else tabAnim[i] = ImClamp(tabAnim[i] + dt*(active?6.0f:-6.0f), 0.0f, 1.0f);
        }

        ImFont* iconFont = custom::shell::GetIconFont();
        ImFont* labelFont = F50 ? F50 : ImGui::GetFont();
        const float iconSize = 18.0f;
        const float labelSize = (labelFont == F50) ? 14.0f : labelFont->FontSize * 0.95f;

        for(int i=0; i<tabCount; i++){
            const ImVec2 tabMin(tabBarLeft + tabInnerPad, tabBarTop + tabInnerPad + (tabHeight + tabGap) * i);
            const ImVec2 tabMax(panelRight - tabInnerPad, tabMin.y + tabHeight);

            ImGui::SetCursorScreenPos(tabMin);
            ImGui::InvisibleButton(("##vtab_"+std::to_string(i)).c_str(), ImVec2(tabMax.x - tabMin.x, tabMax.y - tabMin.y));
            const bool hov = ImGui::IsItemHovered();
            const bool clk = ImGui::IsItemClicked();
            if(clk) runtimeState.page = i+1;
            const bool active = runtimeState.page == i+1;

            ImU32 tabBg;
            if(active) tabBg = IM_COL32((int)(ac.x*255+18), (int)(ac.y*255+18), (int)(ac.z*255+18), 55);
            else if(hov) tabBg = IM_COL32(50, 62, 75, 110);
            else tabBg = IM_COL32(0,0,0,0);
            runtimeDrawList->AddRectFilled(tabMin, tabMax, tabBg, 10.0f);
            if(active){
                runtimeDrawList->AddRect(tabMin, tabMax, IM_COL32(ac.x*255, ac.y*255, ac.z*255, 130), 10.0f, 0, 1.2f);
                runtimeDrawList->AddRectFilled(ImVec2(tabMin.x+2, tabMin.y+12), ImVec2(tabMin.x+5, tabMax.y-12), acU32, 2.0f);
            }

            ImU32 textCol = active ? IM_COL32(255,255,255,255) : hov ? IM_COL32(235,240,250,235) : IM_COL32(150,165,180,210);

            if(iconFont){
                const ImVec2 iSz = iconFont->CalcTextSizeA(iconSize, FLT_MAX, 0, tabIcons[i]);
                const ImVec2 lSz = labelFont->CalcTextSizeA(labelSize, FLT_MAX, 0, tabLabels[i]);
                const float totalW = iSz.x + 6.0f + lSz.x;
                const float startX = tabMin.x + ((tabMax.x - tabMin.x) - totalW) * 0.5f;
                const float centerY = tabMin.y + ((tabMax.y - tabMin.y) - iSz.y) * 0.5f;
                runtimeDrawList->AddText(iconFont, iconSize, ImVec2(startX, centerY), textCol, tabIcons[i]);
                runtimeDrawList->AddText(labelFont, labelSize, ImVec2(startX + iSz.x + 6.0f, centerY + (iSz.y - lSz.y)*0.5f), textCol, tabLabels[i]);
            }else{
                const ImVec2 lSz = labelFont->CalcTextSizeA(labelSize, FLT_MAX, 0, tabLabels[i]);
                runtimeDrawList->AddText(labelFont, labelSize, ImVec2(tabMin.x + ((tabMax.x - tabMin.x) - lSz.x)*0.5f, tabMin.y + ((tabMax.y - tabMin.y) - lSz.y)*0.5f), textCol, tabLabels[i]);
            }

            if(tabFeatureCounts[i] > 0){
                char buf[8];
                snprintf(buf, sizeof(buf), "%d", tabFeatureCounts[i]);
                ImFont* badgeFont = F50 ? F50 : ImGui::GetFont();
                const float badgeSz = badgeFont->FontSize * 0.7f;
                const ImVec2 bSz = badgeFont->CalcTextSizeA(badgeSz, FLT_MAX, 0, buf);
                const float br = 7.0f;
                const float bx = tabMax.x - br - 4.0f;
                const float by = tabMin.y + br + 4.0f;
                runtimeDrawList->AddCircleFilled(ImVec2(bx, by), br, IM_COL32(ac.x*255, ac.y*255, ac.z*255, 245), 16);
                runtimeDrawList->AddText(badgeFont, badgeSz, ImVec2(bx - bSz.x*0.5f, by - bSz.y*0.5f), IM_COL32(0,0,0,255), buf);
            }
        }

        
        const ImVec2 contentMin(panelLeft, headerMax.y + layoutGap);
        const ImVec2 contentMax(tabBarLeft - columnGap, contentBottom);
        runtimeDrawList->AddRectFilled(contentMin, contentMax, tp.bgContent, 12.0f);
        runtimeDrawList->AddRect(contentMin, contentMax, acSoft, 12.0f, 0, 1.5f);
        const ImVec2 contentInnerMin(contentMin.x + contentPadding, contentMin.y + contentPadding);
        const ImVec2 contentInnerSize(contentMax.x - contentMin.x - contentPadding*2, contentMax.y - contentMin.y - contentPadding*2);

        
        const ImVec2 bottomBarMin(panelLeft, panelBottom - bottomBarHeight);
        const ImVec2 bottomBarMax(panelRight, panelBottom);
        runtimeDrawList->AddRectFilled(bottomBarMin, bottomBarMax, IM_COL32(12, 16, 22, 255), 10.0f);
        runtimeDrawList->AddRect(bottomBarMin, bottomBarMax, acSoft, 10.0f, 0, 1.0f);
        runtimeDrawList->AddLine(ImVec2(bottomBarMin.x+16, bottomBarMin.y), ImVec2(bottomBarMax.x-16, bottomBarMin.y), acU32, 1.5f);

        ImFont* bottomFont = F50 ? F50 : ImGui::GetFont();
        const float bottomTextSize = (bottomFont == F50) ? 28.0f : bottomFont->FontSize * 1.5f;
        const char* bottomText = "PREMIUM EDITION";
        const ImVec2 bTextSz = bottomFont->CalcTextSizeA(bottomTextSize, FLT_MAX, 0.0f, bottomText);
        const float textCenterX = bottomBarMin.x + (bottomBarMax.x - bottomBarMin.x - bTextSz.x) * 0.5f;
        const float textCenterY = bottomBarMin.y + (bottomBarHeight - bTextSz.y) * 0.5f;
        runtimeDrawList->AddText(bottomFont, bottomTextSize + 1.5f,
            ImVec2(textCenterX + 1, textCenterY + 1),
            acGlow,
            bottomText
        );

        runtimeDrawList->AddText(bottomFont, bottomTextSize,
            ImVec2(textCenterX, textCenterY),
            acU32,
            bottomText
        );



        
        if(!g_perfMode){
            runtimeState.tabAlpha = ImClamp(runtimeState.tabAlpha + (4.2f*dt*(runtimeState.page==runtimeState.activeTab?1.0f:-1.0f)), 0.0f, 1.0f);
            if(runtimeState.tabAlpha <= 0.01f && runtimeState.tabAdd == 0.0f) runtimeState.activeTab = runtimeState.page;
        }

        
        ImGui::SetCursorScreenPos(contentInnerMin);
        ImGui::BeginChild("##KairoContent", contentInnerSize, false, ImGuiWindowFlags_NoBackground);
        {
            pushedContentFont = false; 
            if(font::inter_semibold){
                ImGui::PushFont(font::inter_semibold);
                pushedContentFont = true;
            }
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, runtimeState.tabAlpha * runtimeStyle->Alpha);
            const ImVec2 contentRegion = ImGui::GetContentRegionAvail();
            const float childHeight = ImMax(0.0f, contentRegion.y);
            const float childWidth = ImMax(0.0f, (contentRegion.x - columnGap) * 0.5f);
            
                        if (runtimeState.activeTab == 1)
                        {
                            custom::BeginGroup();
                            {
                                const ChildFrame left = BeginContentChild("ESP##RUNTIME_ESP", ImVec2(childWidth, childHeight));
                                custom::Checkbox("ESP Line", &Config.ESPMenu.isPlayerLine);
                                custom::Checkbox("ESP Box", &Config.ESPMenu.Box);
                                custom::Checkbox("ESP Skeleton", &Config.ESPMenu.Skeleton);
                                custom::Checkbox("ESP Health", &Config.ESPMenu.Health);
                                custom::Checkbox("ESP Armor", &Config.ESPMenu.Armor);
                                custom::Checkbox("ESP Name", &Config.ESPMenu.Name);
                                custom::Checkbox("ESP Distance", &Config.ESPMenu.Distance);
                                custom::Checkbox("ESP Count", &Config.ESPMenu.Count);
                                custom::Checkbox("ESP BR Class", &Config.ESPMenu.BRClass);
                                custom::Checkbox("ESP Vehicle", &Config.ESPMenu.Vehicle);
                                custom::Checkbox("ESP Vehicle Health", &Config.ESPMenu.VehicleHealth);                               
                                custom::Checkbox("360 Alert", &Config.ESPMenu.Alert);
                                EndContentChild(left);
                            }
                            custom::EndGroup();

                            ImGui::SameLine(0.0f, 10.0f);

                            custom::BeginGroup();
                            {
                                const ChildFrame right = BeginContentChild("ESP OPTIONS", ImVec2(childWidth, childHeight));
                                static const char *boxTypes[] = {"Fill", "Outline", "Corner", "3D"};
                                custom::Combo("Box Type", (int *)&Config.ESPMenu.BoxType, boxTypes, IM_ARRAYSIZE(boxTypes), -1);
                                static const char *linePositions[] = {"Top", "Mid", "Bottom"};
                                custom::Combo("Line Position", (int *)&Config.ESPMenu.Target, linePositions, IM_ARRAYSIZE(linePositions), -1);
                                static const char *healthPositions[] = {"Top", "Side"};
                                custom::Combo("Health Position", (int *)&Config.ESPMenu.HealthPosition, healthPositions, IM_ARRAYSIZE(healthPositions), -1);
                                static const char *espStyles[] = {"None", "3D Sphere", "Player Signal"};                         
                                custom::Combo("ESP Style", (int *)&Config.ESPMenu.EspStyle, espStyles, IM_ARRAYSIZE(espStyles), -1);

                                runtime_preview_menu::DrawRuntimeEspColorRow("Player ESP Color", Config.sColorsESPPLAYER.LinePLAYER);
                                runtime_preview_menu::CopyLinkedEspColors(
                                    Config.sColorsESPPLAYER.LinePLAYER,
                                    Config.sColorsESPPLAYER.BoxPLAYER,
                                    Config.sColorsESPPLAYER.NamePLAYER,
                                    Config.sColorsESPPLAYER.HealthPLAYER,
                                    Config.sColorsESPPLAYER.DistancePLAYER,
                                    Config.sColorsESPPLAYER.SkeletonPLAYER
                                );
                                runtime_preview_menu::DrawRuntimeEspColorRow("Bot ESP Color", Config.sColorsESPBOT.LineBOT);
                                runtime_preview_menu::CopyLinkedEspColors(
                                    Config.sColorsESPBOT.LineBOT,
                                    Config.sColorsESPBOT.BoxBOT,
                                    Config.sColorsESPBOT.NameBOT,
                                    Config.sColorsESPBOT.HealthBOT,
                                    Config.sColorsESPBOT.DistanceBOT,
                                    Config.sColorsESPBOT.SkeletonBOT
                                );
                                EndContentChild(right);
                            }
                            custom::EndGroup();
                        }

                        if (runtimeState.activeTab == 2)
                        {
                            custom::BeginGroup();
                            {
                                const ChildFrame left = BeginContentChild("AIMBOT", ImVec2(childWidth, childHeight));
                                custom::Checkbox("Aimbot 360", &Config.Aim.Aimbot360);
                                custom::Checkbox("Bullet Track", &Config.Aim.AimSilent);
                                
                                custom::SliderFloat("Aim Assist Size", &Config.Aim.AimAssistSize, 0.0f, 100.0f, "%.0f");
                                EndContentChild(left);
                            }
                            custom::EndGroup();

                            ImGui::SameLine(0.0f, 10.0f);

                            custom::BeginGroup();
                            {
                                const ChildFrame right = BeginContentChild("COMBAT OPTIONS", ImVec2(childWidth, childHeight));
                                static const char *targets[] = {"Head", "Chest", "Body"};
                                custom::Combo("Location", (int *)&Config.Aim.Target, targets, IM_ARRAYSIZE(targets), -1);
                                static const char *triggers[] = {"None", "Shooting", "Scoping"};
                                custom::Combo("Trigger", (int *)&Config.Aim.Trigger, triggers, IM_ARRAYSIZE(triggers), -1);
                                static const char *targetBy[] = {"Distance", "FOV"};                               
                                custom::Combo("Target By", (int *)&Config.Aim.By, targetBy, IM_ARRAYSIZE(targetBy), -1);
                                
                                custom::Checkbox("Terrain Clarity", &Config.ExtraMenu.ClearTerrain);                              
                                custom::Checkbox("Show AimLine", &Config.ESPMenu.Aimline);
                                custom::Checkbox("Show FOV", &Config.ESPMenu.ShowFov);                              
                                custom::SliderFloat("FOV Size", &Config.Aim.Cross, 0.0f, 500.0f, "%.0f");
                                custom::Checkbox("Camera Pov", &Config.ExtraMenu.CameraPov);
                                if (Config.ExtraMenu.CameraPov) 
                                {
                                    custom::SliderFloat("Camera Pov Speed", &Config.ExtraMenu.CameraPovSize, 0.0f, 150.0f, "%.0f");
                                }
                                EndContentChild(right);
                            }
                            custom::EndGroup();
                        }

                        if (runtimeState.activeTab == 3)
                        {
                            custom::BeginGroup();
                            {
                                const ChildFrame left = BeginContentChild("MEMORY HACKS", ImVec2(childWidth, childHeight));
                                custom::Checkbox("Smart Reload", &Config.ExtraMenu.SmartReload);
								custom::Checkbox("Throwable Alert", &Config.ESPMenu.GrenadeWarn);
                                custom::Checkbox("Hitbox", &Config.ExtraMenu.Hit);
                                custom::Checkbox("No Recoil", &Config.ExtraMenu.Recoil);
                                custom::Checkbox("No Spread", &Config.ExtraMenu.Spread);
                                custom::Checkbox("No Shake", &Config.ExtraMenu.Shake);
                                custom::Checkbox("No Overheat", &Config.ExtraMenu.Rpd);
                                custom::Checkbox("No Parachute", &Config.ExtraMenu.Parachute);
                                custom::Checkbox("No Wingsuit", &Config.ExtraMenu.NoWingsuit);
                                custom::Checkbox("No Flashbang", &Config.ExtraMenu.Flash);
                                custom::Checkbox("No Crouch", &Config.ExtraMenu.NoCrouch);
                                custom::Checkbox("No Smoke", &Config.ExtraMenu.NoSmoke);
                                custom::Checkbox("Firerate", &Config.ExtraMenu.Fire);
                                custom::Checkbox("Fast Dive", &Config.ExtraMenu.Diving);
                                custom::Checkbox("Walk Under Water", &Config.ExtraMenu.WalkUnderWater);
                                custom::Checkbox("Fast Reload", &Config.ExtraMenu.Reload);
                                custom::Checkbox("Fast Scope", &Config.ExtraMenu.Scope);
                                custom::Checkbox("Quick Switch", &Config.ExtraMenu.Switch);
                                custom::Checkbox("Weapon Kinetic", &Config.ExtraMenu.Kinetic);
								
                                EndContentChild(left);
                            }
                            custom::EndGroup();

                            ImGui::SameLine(0.0f, 10.0f);

                            custom::BeginGroup();
                            {
                                const ChildFrame right = BeginContentChild("MISC FEATURES", ImVec2(childWidth, childHeight));                                
                                custom::SliderFloat("Snowboard Speed", &SnowBsize, 0.0f, 100.0f, "%.1f");
                                custom::SliderFloat("Slide Distance", &SlideRange, 0.0f, 30.0f, "%.1f");
                                custom::SliderFloat("SpeedHack", &speedHackMultiplier, 0.5f, 2.0f, "%.1fx");
                                custom::SliderFloat("High Jump", &jumpHeightMultiplier, 0.5f, 5.0f, "%.2fx");
                                custom::SliderFloat("Gravity", &Config.ExtraMenu.NoGravityScale, 0.0f, 1.0f, "%.2f");
                                custom::Checkbox("No Gravity", &Config.ExtraMenu.NoGravity);
                                custom::Checkbox("Unlock Blueprints", &Config.ExtraMenu.Blueprints);
                                custom::Checkbox("Unlock Attachment", &Config.ExtraMenu.Attachment);
                                custom::Checkbox("Long Execute", &isExecute);
                                custom::Checkbox("Teleport kill", &Config.TeleportEnemy);
                                custom::Checkbox("Yellow Wallhack", &Config.ExtraMenu.WallHack);
                                custom::Checkbox("Red Wallhack", &Config.ExtraMenu.RedWallhack);
                                custom::Checkbox("Spectate No Delay", &Config.ExtraMenu.Spectatex);
                                static const char *camoModes[] = {
                                    "Off",
                                    "Golden Emerald",
                                    "Blistering Magma",
                                    "Polychromatic",
                                    "Glacial Ripple",
                                    "Red Sprite",
                                    "Aether Crystal",
                                    "Platinum",
                                    "Diamond",
                                    "Damascus",
                                    "Gold",
                                };
                                if (custom::Combo("Select Your Desired Camo", &Config.ExtraMenu.CamoTestMode, camoModes, IM_ARRAYSIZE(camoModes), -1)) {
                                    Config.ExtraMenu.CamoTest = Config.ExtraMenu.CamoTestMode > 0;
                                }                                
                                EndContentChild(right);
                            }
                            custom::EndGroup();
                        }                       

                        if (runtimeState.activeTab == 4)
                        {
                            const ChildFrame skinChild = BeginContentChild("SKINS", ImVec2(contentRegion.x, childHeight));
                            RenderSkinCategoryContent(skinSubTab, true);
                            EndContentChild(skinChild);
                        }

                        if (runtimeState.activeTab == 5)
                        {
                            const ChildFrame misc = BeginContentChild("MISC", ImVec2(contentRegion.x, childHeight));
                            const misc_tab::LayoutMetrics layout = misc_tab::CalculateLayout(contentRegion.x, runtime_preview_menu::g_activeChangelogTab);

                            {
                                const ChildFrame changelog = BeginContentChild("CHANGELOG", ImVec2(contentRegion.x, layout.changelogHeight), ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

                                const float changelogTabGap = 5.0f;
                                const float changelogTabH = 40.0f;
                                const float changelogSafeRight = ImGui::GetStyle().ScrollbarSize + 12.0f;
                                const float changelogAvailW = ImMax(1.0f, ImGui::GetContentRegionAvail().x - changelogSafeRight);
                                const float changelogTabW = ImMax(1.0f, ImFloor((changelogAvailW - changelogTabGap * 2.0f) / 3.0f));
                                for (int i = 0; i < IM_ARRAYSIZE(misc_tab::kChangelogTabs); ++i) {
                                    char buttonId[48] = {};
                                    std::snprintf(buttonId, sizeof(buttonId), "##changelog_%d", i);
                                    if (i > 0) {
                                        ImGui::SameLine(0.0f, changelogTabGap);
                                    }
                                    if (misc_tab::DrawChangelogTab(buttonId, misc_tab::kChangelogIcons[i], misc_tab::kChangelogTabs[i], runtime_preview_menu::g_activeChangelogTab == i, ImVec2(changelogTabW, changelogTabH))) {
                                        runtime_preview_menu::g_activeChangelogTab = i;
                                    }
                                }

                                misc_tab::ContentGap(12.0f);

                                const misc_tab::ChangelogSelection activeSection = misc_tab::GetActiveChangelogSelection(runtime_preview_menu::g_activeChangelogTab);

                                ImFont *activeIconFont = F107 ? F107 : ImGui::GetFont();
                                ImGui::PushFont(activeIconFont);
                                ImGui::TextColored(activeSection.color, "%s", activeSection.icon);
                                ImGui::PopFont();
                                ImGui::SameLine(0.0f, 8.0f);
                                ImGui::TextColored(activeSection.color, "%s", activeSection.title);
                                ImGui::SameLine(0.0f, 10.0f);
                                ImGui::TextColored(c::text::text, "May 17, 2026");
                                misc_tab::ContentGap(8.0f);
                                misc_tab::DrawSectionItems(activeSection.items, activeSection.count);
                                EndContentChild(changelog);
                            }

                            misc_tab::ContentGap(layout.rowGap);

                            {
                                const ChildFrame info = BeginContentChild("INFO", ImVec2(contentRegion.x, layout.infoHeight), ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
                                misc_tab::DrawInfoSummary();
                                EndContentChild(info);
                            }

                            misc_tab::ContentGap(layout.rowGap);

                            {
                                const ChildFrame price = BeginContentChild("PRICELIST", ImVec2(contentRegion.x, layout.priceHeight), ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
                                misc_tab::DrawPriceSummary("##runtime_main_misc_prices");
                                EndContentChild(price);
                            }

                            EndContentChild(misc);
                        }

                        if (runtimeState.activeTab == 6)
                        {
                            ImGuiStyle &style = ImGui::GetStyle();
                            const float settingsColumnGap = ImClamp(contentRegion.x * 0.022f, 12.0f, 18.0f);
                            const float settingsRowGap = 6.0f;
                            const float leftChildWidth = ImMax(0.0f, ImFloor((contentRegion.x - settingsColumnGap) * 0.5f));
                            const float rightChildWidth = ImMax(0.0f, contentRegion.x - settingsColumnGap - leftChildWidth);
                            const float topChildHeight = ImMax(0.0f, ImFloor((childHeight - settingsRowGap) * 0.5f));
                            const float bottomChildHeight = ImMax(0.0f, childHeight - settingsRowGap - topChildHeight);
                            const float perfCardHeight = topChildHeight + 50.0f;
                            const float enhCardHeight  = ImMax(0.0f, bottomChildHeight - 50.0f);
                            const float startX = ImGui::GetCursorPosX();

                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, settingsRowGap));

                            ImGui::SetCursorPosX(startX);
                            custom::BeginGroup();
                            {
                                const ChildFrame licenseChild = BeginContentChild("LICENSE INFO", ImVec2(leftChildWidth, topChildHeight));
                                settings_tab::RenderLicenseCard();
                                EndContentChild(licenseChild);

                                const ChildFrame logoChild = BeginContentChild("LOGO SETTINGS", ImVec2(leftChildWidth, bottomChildHeight));
                                settings_tab::RenderLogoCard();
                                EndContentChild(logoChild);
                            }
                            custom::EndGroup();

                            ImGui::SameLine(0.0f, settingsColumnGap);

                            custom::BeginGroup();
                            {
                                const ChildFrame configChild = BeginContentChild("PERFORMANCE", ImVec2(rightChildWidth, perfCardHeight));
                                {
                                    const float rowH  = 44.0f;
                                    const float padX  = 12.0f;
                                    const float avail = ImGui::GetContentRegionAvail().x;
                                    const float dt_p  = ImGui::GetIO().DeltaTime;

                                    ImDrawList* pdl    = ImGui::GetWindowDrawList();
                                    const ImVec2 rMin  = ImGui::GetCursorScreenPos();
                                    const ImVec2 rMax(rMin.x + avail, rMin.y + rowH);

                                    
                                    if (g_perfMode)
                                        pdl->AddRectFilled(rMin, rMax,
                                            IM_COL32((int)(c::accent.x*48),(int)(c::accent.y*48),(int)(c::accent.z*48),200), 8.0f);

                                    ImGui::InvisibleButton("##perfrow", ImVec2(avail, rowH));
                                    if (ImGui::IsItemClicked()) {
                                        g_perfMode = !g_perfMode;
                                   
                                        for (int _i=0;_i<6;++_i) { tabGlowAlpha[_i]=0.f; tabExpandT[_i]=0.f; }
                                        tabGlowAlpha[runtimeState.page-1]=1.f;
                                        tabExpandT[runtimeState.page-1]=1.f;
                                    }
                                    const bool prHov = ImGui::IsItemHovered();
                                    if (prHov && !g_perfMode)
                                        pdl->AddRectFilled(rMin, rMax, IM_COL32(38,50,60,80), 8.0f);

                            
                                    if (g_perfMode)
                                        pdl->AddRectFilled(
                                            ImVec2(rMin.x, rMin.y+6),
                                            ImVec2(rMin.x+3, rMax.y-6),
                                            IM_COL32((int)(c::accent.x*255),(int)(c::accent.y*255),(int)(c::accent.z*255),255), 2.f);

                                    const float iconCY = rMin.y + rowH * 0.5f;
                                    ImFont* iF = custom::shell::GetIconFont();
                                    if (iF) {
                                        const float ifs2 = 13.0f;
                                        const ImVec2 iSz = iF->CalcTextSizeA(ifs2,FLT_MAX,0.f,ICON_FA_BOLT);
                                        pdl->AddText(iF, ifs2,
                                            ImVec2(rMin.x+padX, iconCY-iSz.y*0.5f),
                                            g_perfMode
                                                ? IM_COL32((int)(c::accent.x*255),(int)(c::accent.y*255),(int)(c::accent.z*255),255)
                                                : IM_COL32(110,100,82,200),
                                            ICON_FA_BOLT);
                                    }

                                    
                                    ImFont* lbF = F50?F50:ImGui::GetFont();
                                    const float lbFs  = (lbF==F50)?11.5f:lbF->FontSize*0.82f;
                                    const float sbFs2 = (lbF==F50)? 9.0f:lbF->FontSize*0.64f;
                                    const float txtX  = rMin.x + padX + (iF ? 22.0f : 0.0f);
                                    const ImVec2 lbSz = lbF->CalcTextSizeA(lbFs,FLT_MAX,0.f,"Performance Mode");
                                    const ImVec2 sbSz2= lbF->CalcTextSizeA(sbFs2,FLT_MAX,0.f,"No animations, glow, or lerp");
                                    const float  tbkH = lbSz.y + 2.f + sbSz2.y;
                                    const float  tStartY = iconCY - tbkH*0.5f;
                                    pdl->AddText(lbF, lbFs, ImVec2(txtX, tStartY),
                                        g_perfMode
                                            ? IM_COL32((int)(c::accent.x*255),(int)(c::accent.y*255),(int)(c::accent.z*255),255)
                                            : prHov ? IM_COL32(215,205,185,255) : IM_COL32(185,170,140,220),
                                        "Performance Mode");
                                    pdl->AddText(lbF, sbFs2, ImVec2(txtX, tStartY+lbSz.y+2.f),
                                        IM_COL32(105,96,76,180), "No animations, glow, or lerp");

                                    
                                    static float perfT = 0.0f;
                                    perfT = ImLerp(perfT, g_perfMode?1.f:0.f, dt_p * 14.0f);
                                    const float pillW=38.f, pillH2=20.f;
                                    const float pillX=rMax.x-pillW-padX;
                                    const float pillY=iconCY-pillH2*0.5f;
                                    const ImVec4& ac5=c::accent;
                                 
                                    const ImVec4 bgON5 =ImVec4(ac5.x,ac5.y,ac5.z,0.90f);
                                    const ImVec4 bgOFF5=ImVec4(0.16f,0.20f,0.24f,0.78f);
                                    const ImVec4 pillC =ImLerp(bgOFF5,bgON5,perfT);
                                    pdl->AddRectFilled(
                                        ImVec2(pillX,pillY),ImVec2(pillX+pillW,pillY+pillH2),
                                        ImGui::ColorConvertFloat4ToU32(pillC), pillH2*0.5f);
                                
                                    const float tR2  = pillH2*0.5f-2.5f;
                                    const float tXOn = pillX+pillW-tR2-3.f;
                                    const float tXOff= pillX+tR2+3.f;
                                    const float tXCur= ImLerp(tXOff,tXOn,perfT);
                                    pdl->AddCircleFilled(
                                        ImVec2(tXCur,pillY+pillH2*0.5f), tR2,
                                        g_perfMode?IM_COL32(15,20,24,255):IM_COL32(200,188,165,230), 16);

                                    
                                    const ImVec2 sep0(rMin.x + 8.0f, rMin.y + rowH + 1.0f);
                                    const ImVec2 sep1(rMax.x - 8.0f, rMin.y + rowH + 1.0f);
                                    pdl->AddLine(sep0, sep1, IM_COL32(60,55,45,55), 1.0f);

                                    
                                    ImDrawList* hpdl    = ImGui::GetWindowDrawList();
                                    const ImVec2 hrMin  = ImGui::GetCursorScreenPos();
                                    const ImVec2 hrMax(hrMin.x + avail, hrMin.y + rowH);
                                    bool& hfps = Config.ExtraMenu.HighFps;

                                    if (hfps)
                                        hpdl->AddRectFilled(hrMin, hrMax,
                                            IM_COL32((int)(c::accent.x*48),(int)(c::accent.y*48),(int)(c::accent.z*48),200), 8.0f);

                                    ImGui::InvisibleButton("##highfpsrow", ImVec2(avail, rowH));
                                    if (ImGui::IsItemClicked())
                                        hfps = !hfps;
                                    const bool hHov = ImGui::IsItemHovered();
                                    if (hHov && !hfps)
                                        hpdl->AddRectFilled(hrMin, hrMax, IM_COL32(38,50,60,80), 8.0f);

                                    if (hfps)
                                        hpdl->AddRectFilled(
                                            ImVec2(hrMin.x, hrMin.y+6),
                                            ImVec2(hrMin.x+3, hrMax.y-6),
                                            IM_COL32((int)(c::accent.x*255),(int)(c::accent.y*255),(int)(c::accent.z*255),255), 2.f);

                                    const float hIconCY = hrMin.y + rowH * 0.5f;
                                    ImFont* hIF = custom::shell::GetIconFont();
                                    if (hIF) {
                                        const float hIfs = 13.0f;
                                        const ImVec2 hISz = hIF->CalcTextSizeA(hIfs,FLT_MAX,0.f,ICON_FA_DIGITAL_TACHOGRAPH);
                                        hpdl->AddText(hIF, hIfs,
                                            ImVec2(hrMin.x+padX, hIconCY-hISz.y*0.5f),
                                            hfps
                                                ? IM_COL32((int)(c::accent.x*255),(int)(c::accent.y*255),(int)(c::accent.z*255),255)
                                                : IM_COL32(110,100,82,200),
                                            ICON_FA_DIGITAL_TACHOGRAPH);
                                    }

                                    ImFont* hlbF = F50?F50:ImGui::GetFont();
                                    const float hlbFs  = (hlbF==F50)?11.5f:hlbF->FontSize*0.82f;
                                    const float hsbFs  = (hlbF==F50)? 9.0f:hlbF->FontSize*0.64f;
                                    const float htxtX  = hrMin.x + padX + (hIF ? 22.0f : 0.0f);
                                    const ImVec2 hlbSz = hlbF->CalcTextSizeA(hlbFs,FLT_MAX,0.f,"High FPS");
                                    const float  htbkH = hlbSz.y + 2.f + hlbF->CalcTextSizeA(hsbFs,FLT_MAX,0.f,"Unlock 120fps frame rate").y;
                                    const float  htStartY = hIconCY - htbkH*0.5f;
                                    hpdl->AddText(hlbF, hlbFs, ImVec2(htxtX, htStartY),
                                        hfps
                                            ? IM_COL32((int)(c::accent.x*255),(int)(c::accent.y*255),(int)(c::accent.z*255),255)
                                            : hHov ? IM_COL32(215,205,185,255) : IM_COL32(185,170,140,220),
                                        "High FPS");
                                    hpdl->AddText(hlbF, hsbFs, ImVec2(htxtX, htStartY+hlbSz.y+2.f),
                                        IM_COL32(105,96,76,180), "Unlock 120fps frame rate");

                                    static float highFpsT = 0.0f;
                                    highFpsT = ImLerp(highFpsT, hfps?1.f:0.f, dt_p * 14.0f);
                                    const float hpillX=hrMax.x-pillW-padX;
                                    const float hpillY=hIconCY-pillH2*0.5f;
                                    const ImVec4 hpillC=ImLerp(bgOFF5,bgON5,highFpsT);
                                    hpdl->AddRectFilled(
                                        ImVec2(hpillX,hpillY),ImVec2(hpillX+pillW,hpillY+pillH2),
                                        ImGui::ColorConvertFloat4ToU32(hpillC), pillH2*0.5f);
                                    const float htXOn = hpillX+pillW-tR2-3.f;
                                    const float htXOff= hpillX+tR2+3.f;
                                    const float htXCur= ImLerp(htXOff,htXOn,highFpsT);
                                    hpdl->AddCircleFilled(
                                        ImVec2(htXCur,hpillY+pillH2*0.5f), tR2,
                                        hfps?IM_COL32(15,20,24,255):IM_COL32(200,188,165,230), 16);
                                }
                                EndContentChild(configChild);

                                const ChildFrame enhancementChild = BeginContentChild("ENHANCEMENT", ImVec2(rightChildWidth, enhCardHeight));
                                settings_tab::RenderEnhancementCard();
                                EndContentChild(enhancementChild);
                            }
                            custom::EndGroup();

                            ImGui::PopStyleVar();
                        }

                        ImGui::PopStyleVar();
                        if (pushedContentFont) {
                            ImGui::PopFont();
                        }
                    }
                    ImGui::EndChild();

                    {
                        const float statusBarH = 26.0f; 
                        const float sbTop = contentBottom + layoutGap;
                        const ImVec2 sbMin(panelLeft, sbTop);
                        const ImVec2 sbMax(panelRight, sbTop + statusBarH);
                        runtimeDrawList->AddRectFilled(sbMin, sbMax, tp.bgStatusBar, 8.0f);
                        runtimeDrawList->AddRect(sbMin, sbMax, IM_COL32(42, 52, 60, 160), 8.0f, 0, 1.0f);
                         
                        struct SbItem { const char* label; bool on; };
                        SbItem items[] = {
                            { "ESP", tabFeatureCounts[0] > 0 },
                            { "AIM", tabFeatureCounts[1] > 0 },
                            { "MEM", tabFeatureCounts[2] > 0 },
                            { "SKN", tabFeatureCounts[3] > 0 },
                            { "STG", tabFeatureCounts[5] > 0 },
                        };
                        const int itemCount = IM_ARRAYSIZE(items);
                        ImFont* sf  = F50 ? F50 : ImGui::GetFont();
                        const float sfs  = (sf == F50) ? 9.5f : sf->FontSize * 0.68f;
                        const float dotR = 3.2f;
                        const float itemW = panelWidth / (float)itemCount;
                        static float sbGlow[5] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
                        const float sbDt = ImGui::GetIO().DeltaTime;
                        for (int i = 0; i < itemCount; ++i) {
                            if (g_perfMode)
                            sbGlow[i] = items[i].on ? 1.0f : 0.0f; 
                            else
                            sbGlow[i] = ImClamp(sbGlow[i] + sbDt * (items[i].on ? 7.0f : -7.0f), 0.0f, 1.0f);
                        }
                        for (int i = 0; i < itemCount; ++i) {
                            const float cx   = panelLeft + itemW * i + itemW * 0.5f;
                            const float cy   = sbTop + statusBarH * 0.5f; 
                            const float g    = sbGlow[i];
                            const ImVec2 tSz = sf->CalcTextSizeA(sfs, FLT_MAX, 0.0f, items[i].label);
                            const float tw   = dotR * 2.0f + 5.0f + tSz.x;
                            const float sx   = cx - tw * 0.5f;
                            const float dotX = sx + dotR;
                            const float txtX = sx + dotR * 2.0f + 5.0f;
                            const float txtY = cy - tSz.y * 0.5f;

                            if (g > 0.01f) {
                                if (!g_perfMode) {
                                    
                                    runtimeDrawList->AddCircleFilled(ImVec2(dotX,cy), dotR+6.f, IM_COL32(70,210,90,(int)(28*g)), 20);
                                    runtimeDrawList->AddCircleFilled(ImVec2(dotX,cy), dotR+3.5f, IM_COL32(80,225,100,(int)(62*g)), 20);
                                }
                                runtimeDrawList->AddCircleFilled(ImVec2(dotX,cy), dotR, IM_COL32(110,240,130,255), 20);
                                if (!g_perfMode)
                                    runtimeDrawList->AddCircleFilled(ImVec2(dotX-0.7f,cy-0.7f), dotR*0.38f, IM_COL32(210,255,220,150), 12);
                                const ImU32 lblC = IM_COL32(
                                    (int)ImLerp(90.f,215.f,g),(int)ImLerp(82.f,245.f,g),(int)ImLerp(68.f,225.f,g),255);
                                if (!g_perfMode)
                                    runtimeDrawList->AddText(sf, sfs, ImVec2(txtX+0.5f,txtY+0.5f), IM_COL32(70,210,90,(int)(55*g)), items[i].label);
                                runtimeDrawList->AddText(sf, sfs, ImVec2(txtX,txtY), lblC, items[i].label);
                            } else {
                                runtimeDrawList->AddCircleFilled(ImVec2(dotX,cy), dotR, IM_COL32(50,60,68,200), 20);
                                runtimeDrawList->AddText(sf, sfs, ImVec2(txtX,txtY), IM_COL32(85,78,64,185), items[i].label);
                            }
                        }
                    }

     
                    {
                        const float scW   = 72.0f;
                        const float scGap = layoutGap;   
                        const float scH   = runtimeWindowSize.y; 
                        const ImVec2 scPos(runtimeWindowPos.x - scGap - scW,
                                           runtimeWindowPos.y);  

                        ImGui::SetNextWindowPos(scPos, ImGuiCond_Always);
                        ImGui::SetNextWindowSize(ImVec2(scW, scH), ImGuiCond_Always);
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 14.0f);
                        ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 0));
                        ImGui::PushStyleColor(ImGuiCol_Border,   IM_COL32(0, 0, 0, 0));

                        ImGui::Begin("##SideCard", nullptr,
                            ImGuiWindowFlags_NoTitleBar  | ImGuiWindowFlags_NoResize     |
                            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse   |
                            ImGuiWindowFlags_NoMove      | ImGuiWindowFlags_NoNav        |
                            ImGuiWindowFlags_NoBringToFrontOnFocus |
                            ImGuiWindowFlags_NoSavedSettings);
                        {
                            ImDrawList* sdl  = ImGui::GetWindowDrawList();
                            const ImVec2 sp  = ImGui::GetWindowPos();
                            const ImVec2 ssz = ImGui::GetWindowSize();
                            const ImVec4& ac = c::accent;
                            const float   cx = sp.x + scW * 0.5f;

                            
                            const float halfY = sp.y + ssz.y * 0.5f;

               
                            sdl->AddRectFilled(
                                ImVec2(sp.x + 4, sp.y + 4),
                                ImVec2(sp.x + ssz.x + 4, sp.y + ssz.y + 4),
                                IM_COL32(0, 0, 0, 55), 14.0f);

         
                            {
                                
                                const ImVec4 tbg = ImGui::ColorConvertU32ToFloat4(
                                    main_runtime_theme::g_themes[main_runtime_theme::g_activeTheme].bgWindow);
                                
                                const ImU32 cardBg = IM_COL32(
                                    (int)ImClamp(tbg.x * 255.0f * 0.75f, 0.f, 255.f),
                                    (int)ImClamp(tbg.y * 255.0f * 0.75f, 0.f, 255.f),
                                    (int)ImClamp(tbg.z * 255.0f * 0.75f, 0.f, 255.f),
                                    245);
                                sdl->AddRectFilled(sp, sp + ssz, cardBg, 14.0f);
                            }

                            
                            sdl->AddRect(sp, sp + ssz,
                                IM_COL32((int)(ac.x*255),(int)(ac.y*255),(int)(ac.z*255), 75),
                                14.0f, 0, 1.2f);

                            
                            sdl->AddLine(
                                ImVec2(sp.x + 8.0f,       halfY),
                                ImVec2(sp.x + scW - 8.0f, halfY),
                                IM_COL32((int)(ac.x*255),(int)(ac.y*255),(int)(ac.z*255), 55), 1.0f);

                         
                            const float topH   = ssz.y * 0.5f;

                            
                            const float logoR  = 18.0f;
                            const float logoCY = sp.y + logoR + 10.0f;

                        
                            sdl->AddCircleFilled(ImVec2(cx, logoCY), logoR,
                                IM_COL32((int)(ac.x*50),(int)(ac.y*50),(int)(ac.z*50), 230), 48);
                       
                            sdl->AddCircle(ImVec2(cx, logoCY), logoR,
                                IM_COL32((int)(ac.x*255),(int)(ac.y*255),(int)(ac.z*255), 210),
                                48, 1.6f);
                           
                            sdl->AddCircle(ImVec2(cx - 2.5f, logoCY - 2.5f), logoR * 0.72f,
                                IM_COL32(255, 255, 255, 22), 32, 1.0f);
                           
                            ImFont* lf  = F50 ? F50 : ImGui::GetFont();
                            const float lfs = (lf == F50) ? 13.0f : lf->FontSize * 0.85f;
                            const ImVec2 zxSz = lf->CalcTextSizeA(lfs, FLT_MAX, 0.f, "YX"); 
                            sdl->AddText(lf, lfs,
                                ImVec2(cx - zxSz.x*0.5f + 1.0f, logoCY - zxSz.y*0.5f + 1.0f),
                                IM_COL32(0, 0, 0, 120), "YX"); 
                            sdl->AddText(lf, lfs,
                                ImVec2(cx - zxSz.x*0.5f, logoCY - zxSz.y*0.5f),
                                IM_COL32(240, 230, 210, 255), "YX"); 

                
                            const float logoDivY = logoCY + logoR + 8.0f;
                            sdl->AddLine(
                                ImVec2(sp.x + 10.0f, logoDivY),
                                ImVec2(sp.x + scW - 10.0f, logoDivY),
                                IM_COL32((int)(ac.x*255),(int)(ac.y*255),(int)(ac.z*255), 40), 1.0f);

                            
                            static float savFlash = 0.f, lodFlash = 0.f, rstFlash = 0.f;
                            savFlash = ImMax(0.f, savFlash - ImGui::GetIO().DeltaTime * 2.5f);
                            lodFlash = ImMax(0.f, lodFlash - ImGui::GetIO().DeltaTime * 2.5f);
                            rstFlash = ImMax(0.f, rstFlash - ImGui::GetIO().DeltaTime * 2.5f);

                            struct CfgBtn { const char* label; const char* sub; float* flash; };
                            CfgBtn cfgBtns[3] = {
                                { "SAV", "cfg", &savFlash },
                                { "LOD", "cfg", &lodFlash },
                                { "RST", "def", &rstFlash },
                            };

                           
                            const float btnAreaTop = logoDivY + 6.0f;
                            const float btnAreaBot = halfY    - 6.0f;
                            const float btnAreaH   = btnAreaBot - btnAreaTop;
                            const float cbW = scW - 14.0f;
                            const float cbH = (btnAreaH - 2.0f * 5.0f) / 3.0f; 
                            const float cbX = sp.x + 7.0f;

                            ImFont* bf   = F50 ? F50 : ImGui::GetFont();
                            const float bfs  = (bf == F50) ? 9.0f  : bf->FontSize * 0.65f;
                            const float bfs2 = (bf == F50) ? 7.0f  : bf->FontSize * 0.50f;

                            for (int bi = 0; bi < 3; ++bi) {
                                const CfgBtn& cb = cfgBtns[bi];
                                const float   ff = *cb.flash;
                                const float   cbY = btnAreaTop + bi * (cbH + 5.0f);

                                ImGui::SetCursorScreenPos(ImVec2(cbX, cbY));
                                const std::string btnId = std::string("##scfg") + cb.label;
                                ImGui::InvisibleButton(btnId.c_str(), ImVec2(cbW, cbH));
                                const bool bhov = ImGui::IsItemHovered();
                                const bool bclk = ImGui::IsItemClicked();

                                if (bclk) {
                                    *cb.flash = 1.0f;
                                    if (bi == 0) {
                                        
                                        SaveConfiguration("astral_config");
                                        SaveConfig();
                                    } else if (bi == 1) {
                                        
                                        if (!LoadConfiguration("astral_config")) {
                                            LoadConfig();
                                        }
                                    } else {
                                        memset(&Config, 0, sizeof(sConfig));
                                        Config.sColorsESPPLAYER.LinePLAYER     = CREATE_COLOR(255,0,0,255);
                                        Config.sColorsESPPLAYER.BoxPLAYER      = CREATE_COLOR(255,0,0,255);
                                        Config.sColorsESPPLAYER.NamePLAYER     = CREATE_COLOR(255,0,0,255);
                                        Config.sColorsESPPLAYER.DistancePLAYER = CREATE_COLOR(255,0,0,255);
                                        Config.sColorsESPPLAYER.HealthPLAYER   = CREATE_COLOR(255,0,0,255);
                                        Config.sColorsESPPLAYER.SkeletonPLAYER = CREATE_COLOR(255,0,0,255);
                                        Config.sColorsESPBOT.LineBOT           = CREATE_COLOR(0,255,0,180);
                                        Config.sColorsESPBOT.BoxBOT            = CREATE_COLOR(0,255,0,180);
                                        Config.sColorsESPBOT.NameBOT           = CREATE_COLOR(0,255,0,180);
                                        Config.sColorsESPBOT.HealthBOT         = CREATE_COLOR(0,255,0,180);
                                        Config.sColorsESPBOT.DistanceBOT       = CREATE_COLOR(0,255,0,180);
                                        Config.sColorsESPBOT.SkeletonBOT       = CREATE_COLOR(0,255,0,180);
                                        Config.ExtraMenu.NoGravityScale = 1.0f;
                                        Config.Aim.AimAssistSize               = 0.0f;
                                        Config.Aim.Cross                       = 45.0f;
                                        Config.Aim.Target                      = EAimTarget::Heads;
                                        Config.Aim.Trigger                     = EAimTrigger::None;
                                        Config.Aim.By                          = EAim::Distance;
                                        Config.Bline                           = 2.0f;
                                    }
                                }

                                
                                ImU32 bgCol = (ff > 0.05f)
                                    ? IM_COL32(
                                        (int)ImLerp((float)(bi==2?80:22),(float)(ac.x*255),ff),
                                        (int)ImLerp((float)(bi==2?22:ac.y*255*0.7f),(float)(ac.y*255),ff),
                                        (int)ImLerp(22.f,(float)(ac.z*255),ff),
                                        (int)ImLerp(200.f,255.f,ff))
                                    : bhov ? IM_COL32(38,50,60,210) : IM_COL32(20,28,35,200);
                                sdl->AddRectFilled(ImVec2(cbX,cbY), ImVec2(cbX+cbW,cbY+cbH), bgCol, 6.0f);
                                sdl->AddRect(ImVec2(cbX,cbY), ImVec2(cbX+cbW,cbY+cbH),
                                    (ff>0.05f||bhov)
                                        ? IM_COL32((int)(ac.x*255),(int)(ac.y*255),(int)(ac.z*255),(int)(110+ff*120))
                                        : IM_COL32(42,54,64,140),
                                    6.0f, 0, 1.0f);

                               
                                const ImU32 lblCol = (ff>0.05f) ? IM_COL32(255,255,255,255)
                                    : bhov ? IM_COL32((int)(ac.x*255),(int)(ac.y*255),(int)(ac.z*255),255)
                                           : IM_COL32(195,182,155,230);
                                const float midY = cbY + cbH * 0.5f;
                                const ImVec2 lSz = bf->CalcTextSizeA(bfs,  FLT_MAX, 0.f, cb.label);
                                const ImVec2 sSz = bf->CalcTextSizeA(bfs2, FLT_MAX, 0.f, cb.sub);
                                const float  txtBlockH = lSz.y + 1.0f + sSz.y;
                                sdl->AddText(bf, bfs,
                                    ImVec2(cbX+(cbW-lSz.x)*0.5f, midY - txtBlockH*0.5f),
                                    lblCol, cb.label);
                                sdl->AddText(bf, bfs2,
                                    ImVec2(cbX+(cbW-sSz.x)*0.5f, midY - txtBlockH*0.5f + lSz.y + 1.0f),
                                    IM_COL32(100,92,74,180), cb.sub);
                            }

                                
                            ImFont* sf  = F50 ? F50 : ImGui::GetFont();
                            const float sfs = (sf == F50) ? 8.5f : sf->FontSize * 0.62f;
                            const float dt2 = ImGui::GetIO().DeltaTime;

                            
                            const bool riskHigh = (tabFeatureCounts[1] > 0);
                            const bool riskMed  = !riskHigh && (tabFeatureCounts[2] > 0);
                            const bool riskLow  = !riskHigh && !riskMed &&
                                                  (tabFeatureCounts[0] > 0 || tabFeatureCounts[3] > 0);

                            
                            const float riskTarget = riskHigh ? 1.0f
                                                   : riskMed  ? 0.60f
                                                   : riskLow  ? 0.20f
                                                   : 0.0f;

                            
                            static float displayRisk = 0.0f;
                            if (g_perfMode)
                                displayRisk = riskTarget;
                            else
                                displayRisk = ImLerp(displayRisk, riskTarget, dt2 * 3.5f);
                            const float dr = displayRisk;

             
                            static float pulseT = 0.0f;
                            if (!g_perfMode && riskHigh) pulseT += dt2 * 3.2f;
                            else                          pulseT  = 0.0f;
                            const float pulse = (!g_perfMode && riskHigh)
                                ? (0.5f + 0.5f * sinf(pulseT)) : 0.0f;

                            
                            static int  lastRiskLevel = 0;
                            static float flashT = 0.0f;
                            const int curLevel = riskHigh?3 : riskMed?2 : riskLow?1 : 0;
                            if (!g_perfMode && curLevel != lastRiskLevel) { flashT = 1.0f; }
                            lastRiskLevel = curLevel;
                            flashT = ImMax(0.0f, flashT - dt2 * 4.0f);

                           
                            const ImU32 colGreen  = IM_COL32( 60, 210,  90, 220);
                            const ImU32 colOrange = IM_COL32(235, 145,  30, 220);
                            const ImU32 colRed    = IM_COL32(235,  45,  45, 220);
                            const ImU32 colGreenD = IM_COL32( 28,  80,  40, 120);
                            const ImU32 colOrangeD= IM_COL32( 90,  55,  12, 120);
                            const ImU32 colRedD   = IM_COL32( 90,  18,  18, 120);

                            
                            auto SegCol = [&](float r, int a) -> ImU32 {
                                if (r < 0.34f) {
                                    
                                    return IM_COL32(60, (int)ImLerp(180.f,225.f,r/0.34f), 70, a);
                                } else if (r < 0.67f) {
                                    
                                    const float t = (r - 0.34f) / 0.33f;
                                    return IM_COL32(
                                        (int)ImLerp(200.f, 240.f, t),
                                        (int)ImLerp(200.f, 120.f, t),
                                        30, a);
                                } else {
                                    
                                    const float t = (r - 0.67f) / 0.33f;
                                    return IM_COL32(
                                        (int)ImLerp(240.f, 255.f, t),
                                        (int)ImLerp(80.f,  32.f,  t),
                                        (int)ImLerp(28.f,  28.f,  t),
                                        a + (int)(pulse * 35.0f));
                                }
                            };

                            
                            const float riskLabelY = halfY + 10.0f;
                            const ImVec2 rlSz = sf->CalcTextSizeA(sfs, FLT_MAX, 0.f, "RISK");
                            sdl->AddText(sf, sfs,
                                ImVec2(cx - rlSz.x*0.5f, riskLabelY),
                                IM_COL32(140,124,96,200), "RISK");

                           
                            const float barW      = 14.0f;
                            const float barX      = cx - barW * 0.5f;
                            const float barTop    = riskLabelY + rlSz.y + 8.0f;
                            const float barBottom = sp.y + ssz.y - 28.0f;
                            const float barH      = barBottom - barTop;

                    
                            const float seg33Y = barBottom - barH * 0.33f;  
                            const float seg66Y = barBottom - barH * 0.66f;  

     
                            
                            sdl->AddRectFilled(ImVec2(barX, seg33Y), ImVec2(barX+barW, barBottom),
                                colGreenD, 0.0f);
                            
                            sdl->AddRectFilled(ImVec2(barX, seg66Y), ImVec2(barX+barW, seg33Y),
                                colOrangeD, 0.0f);
                            
                            sdl->AddRectFilled(ImVec2(barX, barTop), ImVec2(barX+barW, seg66Y),
                                colRedD, 0.0f);

                  
                            sdl->AddRect(ImVec2(barX, barTop), ImVec2(barX+barW, barBottom),
                                IM_COL32(42,54,64,160), 7.0f, 0, 1.0f);

                      
                            if (dr > 0.005f) {
                                const float fillTop = barBottom - barH * dr;

              
                                if (dr > 0.005f) {
                                    const float segBot = barBottom;
                                    const float segTop = ImMax(fillTop, seg33Y);
                                    sdl->AddRectFilled(ImVec2(barX, segTop),
                                        ImVec2(barX+barW, segBot), colGreen, 7.0f);
                                }
                     
                                if (dr > 0.33f) {
                                    const float segBot = seg33Y;
                                    const float segTop = ImMax(fillTop, seg66Y);
                                    sdl->AddRectFilled(ImVec2(barX, segTop),
                                        ImVec2(barX+barW, segBot), colOrange, 0.0f);
                                }
                   
                                if (dr > 0.66f) {
                                    const float segBot = seg66Y;
                                    const float segTop = ImMax(fillTop, barTop);
                                    sdl->AddRectFilled(ImVec2(barX, segTop),
                                        ImVec2(barX+barW, segBot), colRed, 0.0f);

                               
                                    if (!g_perfMode && pulse > 0.01f) {
                                        sdl->AddRectFilled(
                                            ImVec2(barX - 3.0f, barTop - 2.0f),
                                            ImVec2(barX + barW + 3.0f, seg66Y + 2.0f),
                                            IM_COL32(255, 50, 50, (int)(38.0f * pulse)),
                                            9.0f);
                                        sdl->AddRectFilled(
                                            ImVec2(barX - 1.5f, barTop - 1.0f),
                                            ImVec2(barX + barW + 1.5f, seg66Y + 1.0f),
                                            IM_COL32(255, 80, 80, (int)(65.0f * pulse)),
                                            8.0f);
                                    }
                                }

                    
                                if (!g_perfMode && dr > 0.02f) {
                                    const float tipH  = 4.0f;
                                    const float tipBot = barBottom - barH * dr;
                                    sdl->AddRectFilled(
                                        ImVec2(barX + 2.0f, tipBot),
                                        ImVec2(barX + barW - 2.0f, tipBot + tipH),
                                        SegCol(dr, 200), 2.0f);
                                    sdl->AddRectFilled(
                                        ImVec2(barX + 3.0f, tipBot - 1.0f),
                                        ImVec2(barX + barW - 3.0f, tipBot + 2.0f),
                                        SegCol(dr, 120), 2.0f);
                                }
                            }

                            
                            if (!g_perfMode && flashT > 0.01f) {
                                sdl->AddRect(
                                    ImVec2(barX - 1.5f, barTop - 1.5f),
                                    ImVec2(barX + barW + 1.5f, barBottom + 1.5f),
                                    IM_COL32(255, 255, 255, (int)(180.0f * flashT)),
                                    8.0f, 0, 2.0f);
                            }

            
                            sdl->AddLine(ImVec2(barX-3.f, seg33Y), ImVec2(barX+barW+3.f, seg33Y),
                                IM_COL32(255,200,100, 80), 1.0f);
                            sdl->AddLine(ImVec2(barX-3.f, seg66Y), ImVec2(barX+barW+3.f, seg66Y),
                                IM_COL32(255, 80, 80, 80), 1.0f);

                            
                            const char* riskLabel = riskHigh ? "HIGH"
                                                  : riskMed  ? "MED"
                                                  : riskLow  ? "LOW"
                                                  : "NONE";
                            const float statusY = barBottom + 6.0f;
                            const ImVec2 rlbSz  = sf->CalcTextSizeA(sfs, FLT_MAX, 0.f, riskLabel);
                            sdl->AddText(sf, sfs,
                                ImVec2(cx - rlbSz.x*0.5f, statusY),
                                SegCol(dr, 230), riskLabel);

                            
                            char pctBuf[8];
                            snprintf(pctBuf, sizeof(pctBuf), "%d%%", (int)(dr * 100.0f));
                            const ImVec2 pSz = sf->CalcTextSizeA(sfs, FLT_MAX, 0.f, pctBuf);
                            sdl->AddText(sf, sfs,
                                ImVec2(cx - pSz.x*0.5f, statusY + rlbSz.y + 2.0f),
                                IM_COL32(130,118,98,180), pctBuf);
                        }
                        ImGui::End();
                        ImGui::PopStyleColor(2);
                        ImGui::PopStyleVar(2);
                    }

                    runtime_preview_menu::ResetPopupFocusWindow();
                    runtime_preview_menu::DrawPopupBackdropFocusLayer(ImGui::GetForegroundDrawList());

                }

                if (Config.TeleportEnemy) {
	                TeleportToEnemy();
                }
                if (Config.ExtraMenu.Fps) {
                    Patches.fpss.Modify();
                }
                  else
                {
                    Patches.fpss.Restore();
                }
                if (Config.ExtraMenu.NoCrouch) {
                    Patches.NoCrouch.Modify();                
                }
                else
                {
                    Patches.NoCrouch.Restore();                
                }
                if (Config.ExtraMenu.NoWingsuit) {
                    Patches.NoWingsuit.Modify();                
                }
                else
                {
                    Patches.NoWingsuit.Restore();                
                }
                if (Config.ExtraMenu.Frame) {
                    Patches.frame.Modify();
                }
                  else
                {
                    Patches.frame.Restore();
                }
                if (Config.ExtraMenu.Grapss) {
                    Patches.grap.Modify();
                }
                  else
                {
                    Patches.grap.Restore();
                }
                if (Config.ExtraMenu.SpeedhackX) {
                    Patches.SpeedhackX.Modify();
                    Patches.SpeedhackX1.Modify();
                }
                else
                {
                    Patches.SpeedhackX.Restore();
                    Patches.SpeedhackX1.Restore();
                }
                if (Config.ExtraMenu.WallHack) {
                    Patches.A1.Modify();
                } else {
                    Patches.A1.Restore();
                }
            }
            ImGui::End();
            ImGui::PopStyleVar();
        }

        ImGui::PopStyleVar();
    }

    auto Input_get_touchCount = (int (*)())(Class_Input_get_touchCount);
    if (Input_get_touchCount() > 0)
    {
        auto Input_GetTouch = (Touch(*)(uintptr_t, int))(Class_Input_GetTouch);
        auto Input_get_mousePosition = (Vector3(*)(uintptr_t))(Class_Input_get_mousePosition);
        switch (Input_GetTouch(Config.ImGuiMenu.thiz, 0).m_Phase)
        {
            case TouchPhase::Began:
            case TouchPhase::Stationary:
                io->MouseDown[0] = true;
                io->MousePos = ImVec2(Input_get_mousePosition(Config.ImGuiMenu.thiz).x,
                                     get_height() - Input_get_mousePosition(Config.ImGuiMenu.thiz).y);
                break;
            case TouchPhase::Ended:
            case TouchPhase::Canceled:
                io->MouseDown[0] = false;
                break;
            case TouchPhase::Moved:
                io->MousePos = ImVec2(Input_get_mousePosition(Config.ImGuiMenu.thiz).x,
                                     get_height() - Input_get_mousePosition(Config.ImGuiMenu.thiz).y);
                break;
            default:
                break;
        }
    }
    else
    {
        io->MouseDown[0] = false;
        io->MouseWheel = 0.0f;
        io->MouseWheelH = 0.0f;
    } 
    
    UpdateCamoOverride();
    ApplyWorldVisualsRuntime();
    InstallBRClassEspConfigHook();
        
   
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    

    return old_eglSwapBuffers(dpy, surface);
}

size_t hook_strlen(const char *thread)
{
    if (strstr(thread, "eglSwapBuffers"))
    {
    }
    return strlen(thread);
}

void Init_Thread2()
{
    InitializeProtection();
}

void Init_SwapHook()
{
}

void Init_Thread()
{
    while (!m_unity)
    {
        m_unity = Tools::GetBaseAddress("libunity.so");
        sleep(1);
    }
    LOGI("libunity.so: %p", m_unity);
    UpdateAllOffset();    

    MemoryPatch::createWithHex("libunity.so", 0x5755800, "00 00 80 D2 C0 03 5F D6").Modify();
    MemoryPatch::createWithHex("libunity.so", 0x9FEC8AC, "00 00 80 D2 C0 03 5F D6").Modify();

    Patches.A1 = MemoryPatch::createWithHex("libunity.so", 0x8D781DC, "1F 20 03 D5 E0 03 13 AA");
    Patches.NoCrouch = MemoryPatch::createWithHex("libunity.so",0x511F50C,"00 00 80 D2 C0 03 5F D6");
    Patches.NoWingsuit = MemoryPatch::createWithHex("libunity.so",0x54A12F4,"00 00 80 D2 C0 03 5F D6");
    Patches.SpeedhackX = MemoryPatch::createWithHex("libunity.so",0x63561AC,"40 00 00 1C C0 03 5F D6");
    Patches.SpeedhackX1 = MemoryPatch::createWithHex("libunity.so",0x63561AC+8,"00 00 00 40");

    
    Patches.grap = MemoryPatch::createWithHex("libunity.so",0x9FF1E64,"20 00 80 D2 C0 03 5F D6"); // get_EnableHDR
    Patches.grap = MemoryPatch::createWithHex("libunity.so",0xA057FE8,"20 00 80 D2 C0 03 5F D6"); // set_EnableHDR
    Patches.grap = MemoryPatch::createWithHex("libunity.so",0x9FE3C80,"20 00 80 D2 C0 03 5F D6"); // get_IsHighEndDevice

    DobbyHook((void *)getAbsoluteAddress("libunity.so", 0xC9B6F90), (void *)&WeaponFireComponent_Instant_CreateBulletLine, (void **)&oWeaponFireComponent_Instant_CreateBulletLine);
    DobbyHook((void *)getAbsoluteAddress("libunity.so", 0xC9C33A4), (void *)&WeaponFireComponent_Instant_CreateBulletProjectile, (void **)&oWeaponFireComponent_Instant_CreateBulletProjectile);
    InstallBRClassEspConfigHook();

    InitializeAllHooks();

    auto swapBuffers = ((uintptr_t)DobbySymbolResolver(OBFUSCATE("libunity.so"), OBFUSCATE("eglSwapBuffers")));
    KittyMemory::ProtectAddr((void *)swapBuffers, sizeof(swapBuffers), PROT_READ | PROT_WRITE | PROT_EXEC);
    xhook_enable_debug(0);
    xhook_register(OBFUSCATE(".*libunity\\.so$"), OBFUSCATE("eglSwapBuffers"), (void*)hook_eglSwapBuffers, (void**)&old_eglSwapBuffers);
    if (xhook_refresh(0) == 0) {
        xhook_clear();
    }
}

__attribute__((constructor))
void native_Init(JNIEnv *env, jclass clazz, jobject mContext) {
}


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    jvm = vm;
    VM = vm;
    std::thread(Init_Thread).detach();
    std::thread(Init_Thread2).detach();
    std::thread(Skins_Thread).detach();
    return JNI_VERSION_1_6;
}

