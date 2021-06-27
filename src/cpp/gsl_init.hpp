#pragma once
#include <gsl/gsl_errno.h>
#include "DynamicInitializer.hpp"

namespace {

void gsl_handler (const char * reason, const char * file, int line, int gsl_errno) {
    throw std::runtime_error(reason);
}

struct Init {
    Init (void) {
        // printf("************** gsl_set_error_handler_off *************\n");
        // gsl_set_error_handler_off();
        gsl_set_error_handler(gsl_handler);
    }
};
DynamicInitializer<Init> const init;
}
