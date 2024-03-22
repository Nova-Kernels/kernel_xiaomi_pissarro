#!/bin/bash
#
# Compile script for NoVA kernel
# Brought to you by rio004
# Adapted for NoVA - @Abdul7852
#

SECONDS=0
DEVICE="pissarro"
DATE=$(date '+%Y%m%d-%H%M')
ZIPNAME="NoVA-${DEVICE}-4.14.336-${DATE}.zip"
toolchain_DIR="$PWD/toolchain/"
DEFCONFIG="${DEVICE}_user_defconfig"
AK3_DIR="$PWD/Anykernel"

# Ensure the toolchain is available
export PATH="$toolchain_DIR/bin:$PATH"

# Process options
CLEAN_BUILD=false
INCLUDE_KSU=false
for arg in "$@"; do
    case $arg in
        -c)   CLEAN_BUILD=true ;;
        -ksu) INCLUDE_KSU=true
              ZIPNAME="NoVA-KSU-${DEVICE}-4.14.336-${DATE}.zip"
              ;;
    esac
done

[ "$CLEAN_BUILD" = true ] && rm -rf out
[ "$INCLUDE_KSU" = true ] && echo "Save your stuff!!" && cd KernelSU/ && git pull origin main && cd ../

# Compilation process
mkdir -p out
make O=out ARCH=arm64 "$DEFCONFIG"
exec 2> >(tee -a out/error.log >&2)

echo -e "\nStarting compilation...\n"
if make -j$(nproc --all) O=out ARCH=arm64 CC="ccache clang" LLVM=1 LLVM_IAS=1 CROSS_COMPILE=aarch64-linux-gnu- CROSS_COMPILE_ARM32=arm-linux-gnueabi- Image.gz; then
    echo -e "\nKernel compiled successfully! Zipping up...\n"



if [ -d "$AK3_DIR" ]; then
	echo -e "\nAnykernel found"
elif ! git clone -q https://github.com/Wahid7852/Anykernel -b pissarro; then
	echo -e "\nAnyKernel repo not found locally and couldn't clone from GitHub! Aborting..."
fi

    cp out/arch/arm64/boot/Image.gz Anykernel
    rm -rf *zip out/arch/arm64/boot
    (cd Anykernel && zip -r9 "../$ZIPNAME" * -x '*.git*' README.md *placeholder)
    echo -e "\nCompleted in $((SECONDS / 60)) minute(s) and $((SECONDS % 60)) second(s)!"
    echo "Zip: $ZIPNAME"
else
    echo -e "\nCompilation failed!"
fi
