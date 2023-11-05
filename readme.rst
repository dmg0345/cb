Circular Buffer
========================================================================================================================

**Circular Buffer**, or **cb** for short, is a C/C++ library that implements a circular buffer interface for generic
types. It uses `stdatomic` for single consumer single producer scenarios, avoiding the need for a lock, and provides
a callback to implement a locking and unlocking mechanism for other scenarios or platforms.

**cb** is fully tested and documented and it is `MISRA C:2012 <https://misra.org.uk/misra-c/>`_ compliant.

Refer to the documentation `here <https://dmg0345-cb.netlify.app>`_ for details.

Build and Development
------------------------------------------------------------------------------------------------------------------------

Clone the repository as:

.. code-block:: powershell

    git clone https://github.com/dmg0345/cb

Example commands to build the library and run the tests are provided below:

.. code-block:: powershell

    cmake -C "./other/cmake/preload/release.cmake" -S . -B "./.cmake_build"
    cmake --build "./.cmake_build" -j --target all
    cmake --build "./.cmake_build" -j --target install
    ctest --test-dir "./.cmake_build"

Find the base Docker image for the development container at `DockerHub <https://hub.docker.com/r/dmg00345/cb>`_. To
develop using `devcontainers` and `Visual Studio Code`:

.. code-block:: powershell

    docker pull dmg00345/cb:latest
    ./manage.ps1 run

Create a release
------------------------------------------------------------------------------------------------------------------------

To generate a release follow the steps below:

1. Create a ``release`` branch from ``develop`` branch, e.g. ``release/X.Y.Z``.
2. Update version in *conf.py* file and in *version.cmake* file.
3. Run *cmake* with ``-DCFG_TAG:BOOL=ON`` to tag all the relevant files.
4. Create pull request from ``release`` branch to ``master`` with the changes with title *Release X.Y.Z*.
5. When merged in ``master`` create release and tag from *Github*, review production workflow passes for deployment.
6. Delete the ``release/X.Y.Z`` branch.
