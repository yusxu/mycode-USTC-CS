# PL/0 language compiler

This is the developing project for the compiler of language PL/0, which is a light-weight programming language.

## BUILD

This project is managed by `cmake`, and the mininum version required is CMake 2.8. Follow the ordinary steps to build this project.

### 1 Generate Makefile
It is strongly recommended to run `do_cmake.sh`, which does everything automatically.
```
./do_cmake.sh
```

If manual operations are needed, try the following steps:
```
mkdir build
cd build
cmake ..
```

### 2 Compilation
If you use `do_cmake.sh` in the last step, change working directory to `build/` with:
```
cd build/
```

Now there should be `Makefile` in the current directory. Build the project with `make`:
```
make
```

Done!

## SYNCHRONIZING FORKS
Forking of a repository cannot be automatically synced. When synchronization is required, follow:

1. First make sure you have a local repository. If not, clone your fork.
2. Add one more remote address with:
```
git remote add upstream ssh://git@202.38.79.111:33126/staff/pl0compiler.git
```
Here, `upstream` is another remote repository address.
3. Now merge changes with:
```
git pull upstream master
```
4. If an immediate sync is needed for the forked remote repository, just use push:
```
git push origin master
```

Now it's OK!
