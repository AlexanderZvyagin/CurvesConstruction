#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <gsl/gsl_errno.h>

#include "gsl_init.hpp"

// #include "DynamicInitializer.hpp"

// namespace {
// struct Init {
//     Init (void) {
//         printf("INIT!!!!!!\n");
//         gsl_set_error_handler_off();
//     }
// };
// DynamicInitializer<Init> const init;

// inline bool fmyinit (void) {
//     printf("%s\n",__PRETTY_FUNCTION__);
//     return true;
// }
//
// bool v = fmyinit();
//
// }
//
// bool v = fmyinit();


TEST_CASE("a"){

}
