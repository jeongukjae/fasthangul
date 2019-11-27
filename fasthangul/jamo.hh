#ifndef __FH_JAMO_H__
#define __FH_JAMO_H__

#include "PythonExtension.hh"
#include <string_view>
#include <unordered_map>

void initializePrecomputedJamos();
std::wstring decompose(std::wstring_view hangul);

#endif /* __FH_JAMO_H__ */
