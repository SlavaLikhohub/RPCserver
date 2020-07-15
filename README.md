#RPC Server

Welcome
-------
This project is realization of simple ease-to-use Remote Procedure Call server with the appropriate client library.

Remote Procedure Call Server
----------------------------

This part of documentation describes server part of the project.

Server can dynamically link functions and provide their calls to clients.

Functions should be compilled as shared library (.so) and be placed in **server/functions/.build** directory.

Function compilling in directory **server/functions** can be done by using **make functions**.
If a function require additional linking, compiling should be done manually.

Usage
------
To build server call ``make``

To see usage call ``./RSTserver --help``

Remote Procedure Call Client
============================

This part of the documentation describes client part of the project.

Client part distributes as library and can be easely integrated into the program.

It is possible to include library as a source files or to build shared library by calling **make**
