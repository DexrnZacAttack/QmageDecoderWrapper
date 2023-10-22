<img src="QMGDecoderIcon.png" width="256" height="256"></img>
# QmageDecoder
### A project to hopefully decode and convert QMG files.
<br>

# Setup
## Setting up the Docker Container.
### You need a docker container to run ARMv7 on x86 (or if you are using a native ARM machine you probably don't need a docker container)
### The recommended distro for this would be Alpine
```bash
docker pull --platform linux/arm/v7 alpine:latest
docker run --privileged --cap-add=SYS_PTRACE --security-opt seccomp=unconfined --security-opt apparmor=unconfined -v .:/arm -it alpine:latest
apk add make gcc g++
cd arm
```
### You can also use Debian, but you'd need to patch the libQmageDecoder.so
```bash
docker pull --platform linux/arm/v7 debian:latest
docker run --privileged --cap-add=SYS_PTRACE --security-opt seccomp=unconfined --security-opt apparmor=unconfined -v .:/arm -it debian:latest
apt update && apt install make gcc g++
cd arm
patchelf --replace-needed libc.so libc.so.6 ./lib/libQmageDecoder.so
patchelf --replace-needed libstdc++.so libstdc++.so.6 ./lib/libQmageDecoder.so
```
## Building and running
### To build
```bash
make build
```
### To run
```bash
cd bin
LD_LIBRARY_PATH="../lib/:" ./main.o {args} {filename}
# note: args are NOT required to run it, only the filename.
# you can access the help page by running
LD_LIBRARY_PATH="../lib/:" ./main.o -h
```
<br>

# Contributors
- [DexrnZacAttack](https://github.com/DexrnZacAttack)
- [Zero_DSRS_VX](https://github.com/PhoenixVX)
- [NerdTheNed](https://github.com/NeRdTheNed)
- [vlOd2](https://github.com/vlOd2)
- [Boreal](https://github.com/bor-real)

