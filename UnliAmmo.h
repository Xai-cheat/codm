#pragma once

bool UnlimitedAmmo = false;
std::chrono::steady_clock::time_point lastAmmoRefillTime;

void UnlimitedAmmoSystem() {
    if (!UnlimitedAmmo) return;
    
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastAmmoRefillTime).count();
    
    if (elapsed < 50) return;
    
    lastAmmoRefillTime = now;
    
    BaseGame *get_MatchGame = GamePlay::get_MatchGame();
    if (!Tools::IsPtrValid(get_MatchGame)) return;
    
    Pawn *get_LocalPawn = GamePlay::get_LocalPawn();
    if (!Tools::IsPtrValid(get_LocalPawn)) return;
    
    Weapon *currentWeapon = get_LocalPawn->get_CurrentWeapon();
    if (!Tools::IsPtrValid(currentWeapon)) return;
    
    uintptr_t weaponPtr = (uintptr_t)currentWeapon;
    
    uintptr_t fireComponentsArray = *(uintptr_t*)(weaponPtr + Class_Weapon_m_FireComponents);
    if (!Tools::IsPtrValid((void*)fireComponentsArray)) return;
    
    uintptr_t fireComponent = *(uintptr_t*)(fireComponentsArray + Class_FireComponentArray_m_CurrentComponent);
    if (!Tools::IsPtrValid((void*)fireComponent)) return;
    
    int* clipAmmoPtr = (int*)(fireComponent + Class_WeaponFireComponent_m_ClipAmmoCount);
    int* carriedAmmoPtr = (int*)(fireComponent + Class_WeaponFireComponent_m_CarriedAmmoCount);
    int* baseClipAmmoPtr = (int*)(fireComponent + Class_WeaponFireComponent_m_BaseClipAmmoCount);
    
    int maxClipAmmo = *baseClipAmmoPtr;
    if (maxClipAmmo <= 0 || maxClipAmmo > 999) maxClipAmmo = 999;
    
    if (*clipAmmoPtr < maxClipAmmo) {
        *clipAmmoPtr = maxClipAmmo;
    }
    
    if (*carriedAmmoPtr < 999) {
        *carriedAmmoPtr = 999;
    }
}

void DrawUnlimitedAmmoUI(ImDrawList *draw, int sWidth, int sHeight) {
    if (UnlimitedAmmo) {
        float fontSize = 22.0f;
        const char* statusText = "[ UNLIMITED AMMO ]";
        
        ImVec2 textSize = ImGui::CalcTextSize(statusText);
        textSize.x *= (fontSize / ImGui::GetFontSize());
        textSize.y *= (fontSize / ImGui::GetFontSize());
        
        float textPosX = (sWidth - textSize.x) / 2;
        float textPosY = sHeight * 0.85f;
        
        draw->AddText(NULL, fontSize, ImVec2(textPosX - 1, textPosY - 1), IM_COL32(0, 0, 0, 255), statusText);
        draw->AddText(NULL, fontSize, ImVec2(textPosX + 1, textPosY - 1), IM_COL32(0, 0, 0, 255), statusText);
        draw->AddText(NULL, fontSize, ImVec2(textPosX - 1, textPosY + 1), IM_COL32(0, 0, 0, 255), statusText);
        draw->AddText(NULL, fontSize, ImVec2(textPosX + 1, textPosY + 1), IM_COL32(0, 0, 0, 255), statusText);
        draw->AddText(NULL, fontSize, ImVec2(textPosX, textPosY), IM_COL32(255, 215, 0, 255), statusText);
    }
}
