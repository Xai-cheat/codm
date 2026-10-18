#pragma once

#include <cstdint>
#include <cstddef>

// Fallbacks for Android-IL2CPP offset constants when the project does not include
// the generated offset header or the symbols are unavailable in this build target.
#ifndef IL2CPP_DOMAIN_GET
#define IL2CPP_DOMAIN_GET 0
#endif

#ifndef IL2CPP_DOMAIN_GET_ASSEMBLIES
#define IL2CPP_DOMAIN_GET_ASSEMBLIES 0
#endif

#ifndef IL2CPP_ASSEMBLY_GET_IMAGE
#define IL2CPP_ASSEMBLY_GET_IMAGE 0
#endif

#ifndef IL2CPP_DOMAIN_ASSEMBLY_OPEN
#define IL2CPP_DOMAIN_ASSEMBLY_OPEN 0
#endif

#ifndef IL2CPP_CLASS_FROM_NAME
#define IL2CPP_CLASS_FROM_NAME 0
#endif

#ifndef IL2CPP_CLASS_GET_FIELD_FROM_NAME
#define IL2CPP_CLASS_GET_FIELD_FROM_NAME 0
#endif

#ifndef IL2CPP_CLASS_GET_METHOD_FROM_NAME
#define IL2CPP_CLASS_GET_METHOD_FROM_NAME 0
#endif

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
