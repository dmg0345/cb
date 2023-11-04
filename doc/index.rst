Introduction
========================================================================================================================

|ProjectFriendlyName| is a C/C++ library that implements a circular buffer interface for generic
types. Some of its features are described below:

- Generic types such as `uint8_t`, `uint16_t` or `uint32_t` all supported with the same interface.
- Support for custom read and write functions or use of the built-in read and write operations.
- Lock-free single producer and single consumer scenarios on platforms that support `stdatomic`.
- Other scenarios and platforms can be handled by implementing custom locking mechanism via events.
- All functionality is accessible through a single include file ``cb/cb.h``.
- Fully tested, see `Test Results HTML Report <_static/_test_results/test_report.html>`_.
- Around 100% code coverage, see `Code Coverage HTML report <_static/_test_coverage/index.html>`_.
- `MISRA:C 2012 <https://misra.org.uk/misra-c/>`_ compliance, detailed below.
- *MIT* license.

The source code for |ProjectFriendlyName| is hosted at `Github <https://github.com/dmg0345/cb>`_ and related
*Docker* images for development containers are located at `DockerHub <https://hub.docker.com/r/dmg00345/cb>`_.

.. note::

    This is the documentation for |ProjectFriendlyName|, version |ProjectVersion|, refer to :doc:`api/version` for
    more details regarding the release.

MISRA C:2012 Compliance
------------------------------------------------------------------------------------------------------------------------

The `CppCheck <https://cppcheck.sourceforge.io/>`_ tool along with it's MISRA addon is used to check for compliance.
This tool is run, along with others, on workflows in `Github Actions` as part of pull requests and prior to release.

For details about suppressions and deviations, look in the global *suppressions.xml* file or for inline suppressions
in the source code on top of the ``cppcheck-suppress misra`` inline supression identifier.

License
-----------------------------------------------------------------------------------------------------------------------

.. literalinclude:: ../LICENSE

.. toctree::
    :caption: Main
    :titlesonly:
    :hidden:

    self
    Use Cases <usecases/cb>

.. toctree::
    :caption: API
    :titlesonly:
    :hidden:

    Circular Buffer <api/cb>
    Versioning <api/version>
