# TST

This folder is reserved for the TST implementation. For whoever will be working on this.

When you start, you can just mirror the shape of what was done in the trie implementation so the two structures are
consistent in comparing:

- `tst.hpp` and `tst.cpp` exposing the same implementation like `Trie` does.
- A `test` folder with Catch2 cases.
- A `CMakeLists.txt` that builds the library and links `levenshtein`.

I made levenshtein reusable as a shared library for this reason, so you can link against it and reuse the same metric
for the TST.

The `SearchState` helper already does the row by row edit distance work, so the TST search walks left, middle, and
right links the same way the Trie walks its children.

Linking against the shhared `levenshtein` keeps both structures ranking the
same candidates in the same order, which is the point of the comparison.

> You can remove this README file once you're finished, it's really just for context.