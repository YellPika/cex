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
    printf(EXCEPTION.what);
} FINALLY {
    printf("Cleanup... cleanup...");
} END_TRY
```

Disclaimer: This code is a year old, and I can't quite remember how it works. All I know is that it worked. Additionally, the preceeding code sample may or may not be exactly how it was meant to be used. Use at your own risk.

As I re-figure out what exactly it was that I was doing, I will update this page.