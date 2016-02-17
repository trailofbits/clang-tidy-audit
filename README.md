# clang-tidy-audit

A prototype tool that rewrites c/cpp/objc files to annotate security points of interest.

 * replaceUsesOfAuto - rewrite uses of auto to their deduced types
 * elaborateOnImplicitCasts - make implicit casts more explicit by injecting comments
 * annotateLambaPointerCaptures - inject comments anytime a pointer is captured by lambda 

## Build

   ./build.sh

## Test

  ./test.sh
