#pragma once

#include "command_macros.h"
#include "configuration.h"
#include "documentation.h"
#include "run.h"

#ifdef CPPLI_IMPLEMENTATION
    #define IRO_IMPL // necessary because if an include that isn't preceded by #define *_IMPL is expanded before the include that *is* preceded by it,
    #define ORI_IMPL // the automatic header guard will kick in and prevent the file from being expanded again, preventing the implementation from being pasted in, which will cause undefined reference errors
    #include "configuration.cpp"
    #include "arg_parsing.cpp"
    #include "documentation.cpp"
    #include "subcommand.cpp"
    #include "user_error.cpp"
    #include "command_registration.cpp"
    #include "command_macros.cpp"
    #include "run.cpp"
#endif

