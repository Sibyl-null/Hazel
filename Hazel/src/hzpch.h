#pragma once

// 预编译头文件（Precompiled Header，PCH）是一种优化技术，用于加速 C++ 源代码的编译过程
//	1. 缩短编译时间：将常用和稳定的头文件的编译结果缓存起来，下次编译时可以直接使用，而不需要重新编译这些头文件。
//  2. 减少重复的工作：C++ 头文件通常包含许多其他头文件，而这些头文件可能在多个源文件中都被引用。
//	   如果没有预编译头文件，每个源文件都要重新包含这些公共头文件，导致重复的编译工作。
//  3. 提高开发效率：由于减少了编译时间，可以更快地看到修改后的结果，提高开发效率和迭代速度。
// 
// 如果代码中的头文件频繁变动或者项目规模较小，使用预编译头文件可能会带来额外的开销，因为需要维护和更新预编译结果。
// .cpp 文件需要 include pch 文件（因为 cpp 文件需要参与编译）。
// .h 文件不需要 include pch 文件（pch 在整个项目中可见，没有显式包含也能访问）

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Hazel/Log.h"

#ifdef HZ_PLATFORM_WINDOWS
	#include <Windows.h>
#endif // HZ_PLATFORM_WINDOWS
