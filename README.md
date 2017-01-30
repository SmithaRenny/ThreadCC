## What?

This is a general purpose compiler suite that includes GCC, GNU make and GDB. If you just need to "download a C/++ compiler and be done with it", this is the way to go. This doesn't require any installation or path config etc. It comes preconfigured with static builds of the [Intel&reg; Threading Building Blocks](https://www.threadingbuildingblocks.org/intel-tbb-tutorial) library, which is a sane extension of C++ for dead-simple threading and concurrency. Click the link to learn more.

This hosts GCC 5.

## Why?

I need the same GCC environment on all my Windows PCs without going crazy. I hate installers. That's basically it.

## How?

- Download as a zip
- Run one of the batch files to get a compiler prompt. 

Now you can use gcc, g++, gdb and make as ordinary commands. It launches in powershell, so you get at least some unix aliases (`ls` etc).

