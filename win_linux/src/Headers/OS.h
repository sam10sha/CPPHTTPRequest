#ifndef OS_H
#define OS_H

#if (defined(__GNUC__) || defined(GNUG)) && !(defined(__clang__) || defined(__ICC) || defined(__INTEL_COMPILER))
#define OS_LINUX
#elif (defined(_MSC_VER))
#define OS_WIN
#endif // OS_VERSION

#endif // OS_H