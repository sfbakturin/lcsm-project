#ifndef LCSM_LCSMTESTINGCONFIG_H
#define LCSM_LCSMTESTINGCONFIG_H

#if defined(LCSM_CONFIG_SYSTEM_WIN32) && defined(LCSM_CONFIG_LIBRARY_SHARED)
#ifdef LCSMTesting_EXPORTS
#define LCSMTesting_API __declspec(dllexport)
#else
#define LCSMTesting_API __declspec(dllimport)
#endif
#else
#define LCSMTesting_API
#endif

#endif /* LCSM_LCSMTESTINGCONFIG_H */
