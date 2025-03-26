#ifndef LCSM_LCSMCONFIG_H
#define LCSM_LCSMCONFIG_H

#ifdef LCSM_CONFIG_COMPILER_MSVC
#pragma warning(disable : 4251)
#endif

#if defined(LCSM_CONFIG_SYSTEM_WIN32) && defined(LCSM_CONFIG_LIBRARY_SHARED)
#ifdef LCSM_EXPORTS
#define LCSM_API __declspec(dllexport)
#else
#define LCSM_API __declspec(dllimport)
#endif
#else
#define LCSM_API
#endif

#endif /* LCSM_LCSMCONFIG_H */
