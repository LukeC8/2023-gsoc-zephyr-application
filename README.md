# Simple socket-based program to serve a Web page
This project is my application to the Zephyr Project slot of Linux Foundation in the GSoC 23. This repo will track all development process of a simple program written in C using the Linux Sockets to serve a web page. This program is my application to participate of the Zephyr HTTP Server Implementation in GSoC23, to know more about check [Zephyr Page](https://wiki.linuxfoundation.org/gsoc/2023-gsoc-zephyr) in the Linux Foundation Wiki.

# Installation
The program is a [Zephyr freestanding application](https://docs.zephyrproject.org/latest/develop/application/index.html#zephyr-freestanding-application). It means that you will need a Zephyr workspace to build and run this program.
After [setting up the west workspace](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#get-zephyr-and-install-python-dependencies), clone this repo and build it with your workspace.

    git clone git@github.com:LukeC8/2023-gsoc-zephyr-application.git lukec8_gsoc_zephyr
    west build -b qemu_x86 lukec8_gsoc_zephyr

# Usage
To test the program on Linux Host check the Zephyr documentation [Networking with QEMU](https://docs.zephyrproject.org/latest/connectivity/networking/qemu_setup.html#networking-with-qemu).
After run the scripts `loop-socat.sh` and `loop-slip-tap.sh`, start the program in QEMU with:

    west build -t run

The server will listen for new connections to the port `8080` of the TAP interface created by the `loop-slip-tap.sh` script.
The default IP addres of that TAP interface is `192.0.2.1`.
