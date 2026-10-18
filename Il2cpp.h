#pragma once

#include <cstdint>
#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif

// Minimal Il2Cpp types required by Il2Cpp.cpp to compile.

typedef uint16_t UTF16;

typedef struct Il2CppString {
    int32_t length;        // number of UTF-16 code units
    UTF16 start_char;      // first char; real Il2CppString stores chars inline after this field

    // Methods are defined/implemented in Il2Cpp.cpp
    const char* CString();
    const wchar_t* WCString();
    static Il2CppString* Create(const char* s);
    static Il2CppString* Create(const wchar_t* s, int len);
} Il2CppString;

// Minimal class/field layout used by Il2CppGetStaticFieldOffset
typedef struct Il2CppClass {
    void* static_fields; // pointer to the class's static field storage
} Il2CppClass;

typedef struct FieldInfo {
    Il2CppClass* parent;
    size_t offset;
} FieldInfo;

#ifdef __cplusplus
}
#endif
