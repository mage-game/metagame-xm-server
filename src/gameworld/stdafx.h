#ifndef STDAFX_H___
#define STDAFX_H___

#include "common/platform/file64.h"
#include "common/platform/socket/pisocket.h"

#include <iostream>
#include <algorithm>
#include <functional>
#include <array>
#include <vector>
#include <map>
#include <queue>
#include <list>
#include <deque>
#include <set>
#include <string>

// 项目中比较重的头文件，可能改动频率不会很大的，放在下面，提高编译速度
#include "world.h"
#include "gstr.h"
#include "protocal/msgother.h"
#include "gamecommon.h"
#include "config/gameconfigcommon/gameconfigcommon.h"
#include "servercommon/activitydef.hpp"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "gamelog.h"

#endif