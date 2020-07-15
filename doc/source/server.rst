Remote Procedure Call Server
============================

This part of documentation describes server part of the project.

Server can dynamically link functions and provide their calls to clients.

Functions should be compilled as shared library (.so) and be placed in **server/functions/.build** directory.

Function compilling in directory **server/functions** can be done by using **make functions**.
If a function require additional linking, compiling should be done manually.

Usage
------
To build server call ``make``

To see usage call ``./RSTserver --help``

Code description
----------------
.. c:autodoc:: server/inc/commondefs.h server/src/*.c

