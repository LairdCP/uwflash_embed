=============
December 2016
=============

This folder contains a sample console mode application for Windows which uses
Borland Builder v6 and AsyncPro Serial Port VCL component for serial comms access.

It demonstrates how to invoke the upgrader in a blocking function to expedite
a firmware download into a BL652 module.

The source will make calls to helper functions that need to be implemented for
any target platform.

It is as simple as calling as a single blocking function to expedite the
firmware download which is called UwFlashUpgradeFirmare()
