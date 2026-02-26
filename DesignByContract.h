//============================================================================
// Name        : DesignByContract.h
// Author      : Serge Demeyer, modified by Kasper Engelen
// Version     :
// Copyright   : Project Software Engineering - BA1 Informatica - Serge Demeyer - University of Antwerp
// Description : Declarations for design by contract in C++
//============================================================================

// #include <assert.h>

// #define REQUIRE(assertion, what)
// 	if (!(assertion)) __assert (what, __FILE__, __LINE__)

// #define ENSURE(assertion, what)
// 	if (!(assertion)) __assert (what, __FILE__, __LINE__)


#include <stdio.h>
#include <stdlib.h>

static inline void contract_fail(
    const char *expr,
    const char *file,
    int line,
    const char *msg)
{
    fprintf(stderr,
            "Contract violation\n"
            "  Expression: %s\n"
            "  Message:    %s\n"
            "  Location:   %s:%d\n",
            expr,
            msg ? msg : "(none)",
            file,
            line);

    abort();
}

#define REQUIRE(assertion, what) \
    do { \
        if (!(assertion)) \
            contract_fail(#assertion, __FILE__, __LINE__, what); \
    } while (0)

#define ENSURE(assertion, what) \
    do { \
        if (!(assertion)) \
            contract_fail(#assertion, __FILE__, __LINE__, what); \
    } while (0)
