#pragma once

/// Rendering
//////////////////////////////////////////////////////////////////////////

#define GP_AsyncRendering EZ_OFF

#define GP_UseDirect3D EZ_OFF
#define GP_UseOpenGL EZ_ON

// Check that only 1 renderer library is turned on
#if EZ_IS_NOT_EXCLUSIVE(GP_UseDirect3D, GP_UseOpenGL)
#error "You can only enable 1 renderer library at a time."
#endif

// Only until d3d is supported
#if EZ_ENABLED(GP_UseDirect3D)
#error "Direct3D is not supported at the moment."
#endif
