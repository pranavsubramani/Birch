# LibBirch C++ Library

LibBirch is a C++ template library intended as a compile target for probabilistic programming languages (PPLs). It provides basic types, [Eigen](https://eigen.tuxfamily.org/) integration, and dynamic memory management (pooled allocators, reference counting, cycle collection, lazy deep copy). This is particularly important
for memory efficiency in PPLs using Sequential Monte Carlo (SMC) and related methods for inference.

LibBirch was developed primarily to support the [Birch](https://birch.sh) probabilistic programming language, but may be useful for other purposes too.


## License

LibBirch is open source software.

It is licensed under the Apache License, Version 2.0 (the "License"); you may not use it except in compliance with the License. You may obtain a copy of the License at <http://www.apache.org/licenses/LICENSE-2.0>.


## Installing

To build and install, use:

./bootstrap
./configure
make
make install
