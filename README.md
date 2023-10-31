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
docker run --privileged --cap-add=SYS_PTRACE --security-opt seccomp=unconfined --security-opt apparmor=unconfined -v .:/qmg -it alpine:latest
apk add make gcc g++ lld
cd qmg
```
### You can also use Debian, but you'd need to patch the libQmageDecoder.so
```bash
docker pull --platform linux/arm/v7 debian:latest
docker run --privileged --cap-add=SYS_PTRACE --security-opt seccomp=unconfined --security-opt apparmor=unconfined -v .:/qmg -it debian:latest
apt update && apt install make gcc g++ patchelf lld
cd qmg
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
LD_LIBRARY_PATH="../lib/:" ./qmdecoder {args} {filename}
# note: args are NOT required to run it, only the filename.
# you can access the help page by running
LD_LIBRARY_PATH="../lib/:" ./qmdecoder -h
```
<br>

# Contributors
- [DexrnZacAttack](https://github.com/DexrnZacAttack)
- [NeRdTheNed](https://github.com/NeRdTheNed)
- [Zero_DSRS_VX](https://github.com/PhoenixVX)
- [CarPBon](https://github.com/CarPBon) (For providing ARM hardware to build and test on)
- [vlOd2](https://github.com/vlOd2)
- [Boreal](https://github.com/bor-real)
