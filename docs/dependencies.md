# OpenDDS Dependencies

## Required to Build the OpenDDS Library

**NOTE: Perl is required to run the configure script; MPC, ACE, and TAO will be
downloaded automatically by the configure script by default.**

### Perl

Perl is an interpreted language used in the configure script, the tests, and
any other scripting in OpenDDS codebase. Even if the configure script is not
used, it is also required to run MPC, so it is required to build OpenDDS.

Testing scripts are written in Perl and use the common PerlACE modules provided
by ACE. For scripts that will be part of automated testing, don't assume the
presence of any non-standard (CPAN) modules. Perl offers many facilities for
portability. Shell scripts are by definition non-portable and should not to be
committed to the OpenDDS repository.

### MPC

MPC is the build system used by OpenDDS, used to configure the build and
generate platform specific build files (Makefiles, VS solution files, etc.).

The official repository is hosted on Github at
[DOCGroup/MPC](https://github.com/DOCGroup/MPC).

### ACE

ACE is the platform abstraction layer used by OpenDDS. It is used both directly
and through TAO. Facilities not provided by the C++ 2003 standard library, for
example sockets, threads, and dynamic library loading, are provided by ACE.

Some other features OpenDDS relies on ACE for:

- ACE provides the `gnuace` type used by MPC
- ACE contains a script, `generate_export_file.pl`, which is used (along with
  MPC) to manage shared libraries' symbol visibility (also known as
  export/import)
  - See ACE documentation and usage guidelines for details
  - ACE logging is used (`ACE_Log_Msg` and related classes).
  - ACE logging uses a formatting string that works like `std::printf()` but not
    all of the formatting specifiers are the same as `printf()`. Please read the
    `ACE_Log_Msg` documentation before using.
- The most commonly misused one is `%s` for `char*` strings. ACE uses `%C` for
  `char*` strings.
- ACE has classes and macros for wide/narrow string conversion. See
  `docs/design/WCHAR` in the OpenDDS repository for details.
- ACE provides support for platforms that have a non-standard program
  entry point (`main`). All of our `main` functions are
  `int ACE_TMAIN(int argc, ACE_TCHAR* argv[])`.

The OCI page for ACE is https://objectcomputing.com/products/ace.

The upstream DOC Group repository is hosted on Github at
[DOCGroup/ACE\_TAO](https://github.com/DOCGroup/ACE_TAO), which it shares with
TAO.

### TAO

TAO is a C++ CORBA Implementation built on ACE.

- TAO provides the IDL compiler `tao_idl` and non-generated classes which
  implement the IDL-to-C++ mapping.
- TAO ORBs are only created for interaction with the DCPSInfoRepo, all other
  uses of TAO are basic types and local interfaces.
- A separate library, OpenDDS\_InfoRepoDiscovery, encapsulates the participant
  process's use of the ORB
  - This is the only library which depends on TAO\_PortableServer

The OCI page for TAO is https://objectcomputing.com/products/tao.

The upstream DOC Group repository is hosted on Github at
[DOCGroup/ACE\_TAO](https://github.com/DOCGroup/ACE_TAO), which it shares with
ACE.

## Optional Dependencies

### CMake

OpenDDS has a [CMake "FindPacakge" module included](../cmake). See
[cmake.md](cmake.md) for how to make OpenDDS applications with CMake and
without the need to use MPC in your application.

CMake is also used for the Google Test based DDS security unit tests. See
[../tests/gtest_setup.txt](../tests/gtest_setup.txt).

### Google Test

Google Test is used for DDS security unit tests.
See [../tests/gtest_setup.txt](../tests/gtest_setup.txt) for details.

### Java

OpenDDS has optional Java bindings. It requires the Java Development Kit
(JDK). See [../java/README](../java/README).

There is also support for Java Message Server (JMS) v1.1. In addition to the
JDK, it requires Ant and JBoss 4.2.x. See [../java/jms](../java/jms/README).

### Qt

Qt5 is used for the [monitor](../tools/monitor) utility program and the
[ishapes](../examples/DCPS/ishapes) RTPS demo.

See [qt.md](qt.md) for details on configuring OpenDDS to use Qt.

### Wireshark

A Wireshark dissector plugin for OpenDDS's non-RTPS transports is included with
OpenDDS. The dissector supports displaying and filtering by sample contents and
works with Wireshark 1.x and 2.x.

See [../tools/dissector/README.md](../tools/dissector/README.md) for details.

### RapidJSON

RapidJSON is a C++ JSON Library used in the Wireshark dissector for sample
dissection. Samples are described in what is called the Intermediate Type
Language (ITL) which is JSON that can be generated by `opendds_idl` from IDL
files.

See
[../tools/dissector/README.md#sample-dissection](../tools/dissector/README.md#sample-dissection)
for details.

### Boost

Boost, as of writing, is just an optional dependency of the
[ishapes](../examples/DCPS/ishapes) RTPS demo and only if C++11 is not
available.
