## Toolchains and Testing

As with any other compiled languages, we need to compiler and link the source files. Manually writing the commands on the terminal gets impractical as soon as the project grows

**CMake** generates the build instrctions for the files present in CMakeLists (build system generation). These are used by tools like Make and Ninja to actually build and run the program

### Testing

A [unit test](https://en.wikipedia.org/wiki/Unit_testing) checks whether a small unit of code behaves as expected. The expected behavior comes from the class's contract, not from the implementation itself.

For example, if inserting "X" at position 1 in "abc" is supposed to produce "aXbc", the test should perform that operation and compare the resulting text with "aXbc".

A useful structure for writing such tests is Arrange-Act-Assert:

- Arrange the initial state.
- Act by performing the operation being tested.
- Assert that the result matches the expected behavior.

"A test that merely calls a method does not establish that the method works. It needs an assertion that would detect an incorrect result."

I will be using CTest + GoogleTest for this project
