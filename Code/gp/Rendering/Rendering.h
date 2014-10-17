#pragma once

#if EZ_ENABLED(GP_UseOpenGL)
#include "gp/Rendering/OpenGL/OpenGL.h"
#elif EZ_ENABLED(GP_UseDirect3D)
#include "gp/Rendering/Direct3D/Direct3D.h"
#endif
