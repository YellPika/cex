A small exception library written in and for C. It has full support for try, catch and finally, and is fully capable of handling nested try blocks.

Usage:
```
#include "cex.h"

...

cex_exception_type INVALID_OPERATION = 1;

...

TRY {
    THROW(INVALID_OPERATION, "Some random error has occurred.");
    printf("Should not happen...");
} CATCH(INVALID_OPERATION) {
    printf("%s", EXCEPTION.what);
} FINALLY {
    printf("Cleanup... cleanup...");
} END_TRY
```
