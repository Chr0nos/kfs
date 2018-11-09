CORES=7
BINUTIL=binutils-2.31.tar.gz
GCC=gcc-8.2.0.tar.gz
INSTALL_DIR=/opt/cross/i686

if [ ! -f $BINUTIL ]; then
    wget ftp://ftp.gnu.org/gnu/binutils/$BINUTIL
fi

if [ ! -f $GCC ]; then
    wget ftp://ftp.gnu.org/gnu/gcc/gcc-8.2.0/$GCC
fi

if [ ! -d $INSTALL_DIR ]; then
    sudo mkdir -pv $INSTALL_DIR
fi

rm -rf build
tar -xvzf $BINUTIL
tar -xvzf $GCC

export TARGET=i686-elf
export PATH="$INSTALL_DIR/bin:$PATH"

mkdir -v build && cd build &&

mkdir -v build-binutils && cd build-binutils &&
    ../../binutils-2.31/configure --target=$TARGET --prefix="$INSTALL_DIR" --with-sysroot --disable-nls --disable-werror &&
    make -j $CORES &&
    sudo make install

cd .. && mkdir build-gcc && cd build-gcc &&
    ../../gcc-8.2.0/configure --target=$TARGET --prefix="$INSTALL_DIR" --disable-nls --enable-languages=c,c++ --without-headers &&
    make -j $CORES all-gcc &&
    make -j $CORES all-target-libgcc &&
    sudo make install-gcc install-target-libgcc
