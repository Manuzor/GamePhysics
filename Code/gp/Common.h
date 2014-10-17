#pragma once

#include "gp/Config.h"
#include "gp/Types.h"

#define GP_NotImplemented do {             \
    EZ_REPORT_FAILURE("Not implemented."); \
    throw gpNotImplementedException();     \
} while(false)
