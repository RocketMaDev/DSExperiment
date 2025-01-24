# DS Experiment HDU

The source code in this repo is about `Data Structure Experiment` in
Hangzhou Dianzi University, Cyber Security college.

## Features

1. Comments about functions are included in all header files.
2. No errors or warnings when compiling test cases, no memory
   leaks when running.
3. Reusablity and robustness. I considered many edge cases and how to
   reuse on top modules. **Though DO NOT use it in production!**
4. Standardized project structure. Source code and headers are stored
   in `src/`, object files are stored in `build/` and executables are
   are stored in `./`.
5. Compile executables with GNU Make conveniently.

To meet some strange requirements in Chinese textbook, some code
may looks strange.

> I don't upload experiment reports, that's deliberate. And I will never
> do that. Please learn how to compile this project first.

## Compilation

All code can be compiled under Arch Linux + gcc and pass all tests,
but I never try to compile on any other platforms. Please note that
there are gcc special macros, so the source may not be compiled in
Visual Studio successfully.

With GNU Make tool chain, run `make TARGET=$EXE` to compile target,
targets can be found in `Makefile`. `-O2` optimization is enabled by
default, to debug the program, please run `make TARGET=$EXE DEBUG=1`
to compile with debuginfo and default optimization level.

To compile a whole set of an experiment, please run `make experiment$i`,
where `i` can be 1 to 5. Read `Makefile` to know what executables different
experiment generates.

To clear dependencies (.o) and keep executables, please run `make clean_deps`.
To clear everything generated, run `make clean`.

## "I have questions"

If you find compilation not working, spot some bugs or would like to
adapt this project to other platforms, please consult [CONTRIBUTING](CONTRIBUTING.md).

## Package source code

Due to reusablity of modules, you probably need to package everything to
meet dependencies. If you don't want to package the same code for every
experiment, you can checkout corresponding tags to see what I package at
every experiment.

For instance, run `git checkout experiment1` to trace back to the time
when I finished writing experiment 1. After packaging code with tools
like `zip`, run `git checkout` to rewind to the HEAD of main.

## License

This project is distributed under GPLv3, see [LICENSE](LICENSE)
for more details.
