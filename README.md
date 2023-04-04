# Simple socket-based program to serve a Web page
This project is my application to the Zephyr Project slot of Linux Foundation in the GSoC 23. This repo will track all development process of a simple program written in C using the Linux Sockets to serve a web page. This program is my application to participate of the Zephyr HTTP Server Implementation in GSoC23, to know more about check [Zephyr Page](https://wiki.linuxfoundation.org/gsoc/2023-gsoc-zephyr) in the Linux Foundation Wiki.

The application is based on the [Zephyr Samples - Socket Dumb Http Server](https://docs.zephyrproject.org/latest/samples/net/sockets/dumb_http_server/README.html), the difference is the integration of the [LLHTTP Parser](https://github.com/nodejs/llhttp/blob/main/README.md) to show how we can use the `llhttp` project to implement the http parser of the Zephyr HTTP server.

In the [doc](doc/) folder you can find a pcap file with the network traffic between the client and host. Note that all responses from the host contain the `SERVER` header with value equal to `zephyr_os`.

# Installation
The program is a [Zephyr freestanding application](https://docs.zephyrproject.org/latest/develop/application/index.html#zephyr-freestanding-application). It means that you will need a Zephyr workspace to build and run this program.
After [setting up the west workspace](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#get-zephyr-and-install-python-dependencies), clone this repo and build it with your workspace.

    git clone git@github.com:LukeC8/2023-gsoc-zephyr-application.git lukec8_gsoc_zephyr
    west build -b qemu_x86 lukec8_gsoc_zephyr

# Usage
To test the program on Linux Host check the Zephyr documentation [Networking with QEMU](https://docs.zephyrproject.org/latest/connectivity/networking/qemu_setup.html#networking-with-qemu).
Makes sure that `loop-socat.sh` and `loop-slip-tap.sh` are running and start the program in QEMU with:

    west build -t run

The server will listen for new connections to the port `8080` of the TAP interface created by the `loop-slip-tap.sh` script.
The default IP addres of that TAP interface is `192.0.2.1`.

Use the browser or command line tools to send http requests to the server. The URL that will be accepted by the server are described in the table below:

|  URL  | Description |
| :---: |   :---:     |
| `/`   | The Index page. Show a html page with a brief description about the Zephyr Project |
| `/small` | Show a simple plain text message |
| `/big` | same content of `/` |
| any other url | The server will send a 404 Error and show a 404 html error page |
