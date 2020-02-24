#!/usr/bin/python3

FFTW_ARCHIVE_NAME = "fftw-3.3.8.tar.gz"
FFTW3_URL = "http://www.fftw.org/" + FFTW_ARCHIVE_NAME
FFTW_SRC_DIR = 'fftw-3.3.8'

OBOE_SRC_DIR = 'oboe'
EIGEN_SRC_DIR = 'eigen'
LIBSPEECH_SRC_DIR = 'lib'

FFTW3_COMPILED_DIRECTORY = "fftw-3.3.8_compiled"
OBOE_COMPILED_DIRECTORY = "oboe_compiled"
LIBSPEECH_COMPILED_DIRECTORY = "libspeech_compiled"


FFTW_BUILD_SCRIPT = '''
NDK={0}
TOOLCHAIN=$NDK/toolchains/llvm/prebuilt/linux-x86_64

API={1}
ARCHITECTURE={2}

case $ARCHITECTURE in
	armv7a)
        export TARGET=$ARCHITECTURE-linux-androideabi
        export AR=$TOOLCHAIN/bin/$TARGET-ar
        export AS=$TOOLCHAIN/bin/$TARGET-as
        export CC=$TOOLCHAIN/bin/$TARGET$API-clang
        export CXX=$TOOLCHAIN/bin/$TARGET$API-clang++
        export LD=$TOOLCHAIN/bin/$TARGET-ld
        export RANLIB=$TOOLCHAIN/bin/$TARGET-ranlib
        export STRIP=$TOOLCHAIN/bin/$TARGET-strip
        export CMAKE_AR=$AR
        export CMAKE_RANLIB=$RANLIB
		;;
	*)
        export TARGET=$ARCHITECTURE-linux-android
        export AR=$TOOLCHAIN/bin/$TARGET-ar
        export AS=$TOOLCHAIN/bin/$TARGET-as
        export CC=$TOOLCHAIN/bin/$TARGET$API-clang
        export CXX=$TOOLCHAIN/bin/$TARGET$API-clang++
        export LD=$TOOLCHAIN/bin/$TARGET-ld
        export RANLIB=$TOOLCHAIN/bin/$TARGET-ranlib
        export STRIP=$TOOLCHAIN/bin/$TARGET-strip
        export CMAKE_AR=$AR
        export CMAKE_RANLIB=$RANLIB
		;;
  esac

cmake -DBUILD_SHARED_LIBS=OFF -DCMAKE_C_FLAGS=-fPIC -DCMAKE_CXX_FLAGS=-fPIC -DBUILD_TESTS=OFF ..
make -j4
'''

OBOE_BUILD_SCRIPT = FFTW_BUILD_SCRIPT
LIBSPEECH_BUILD_SCRIPT = '''
NDK={0}
TOOLCHAIN=$NDK/toolchains/llvm/prebuilt/linux-x86_64

API={1}
ARCHITECTURE={2}

Eigen3_DIR=../../eigen/build

case $ARCHITECTURE in
	armv7a)
        export FFTW_ARCH_FOLDER=arm

        export TARGET=$ARCHITECTURE-linux-androideabi
        export AR=$TOOLCHAIN/bin/$TARGET-ar
        export AS=$TOOLCHAIN/bin/$TARGET-as
        export CC=$TOOLCHAIN/bin/$TARGET$API-clang
        export CXX=$TOOLCHAIN/bin/$TARGET$API-clang++
        export LD=$TOOLCHAIN/bin/$TARGET-ld
        export RANLIB=$TOOLCHAIN/bin/$TARGET-ranlib
        export STRIP=$TOOLCHAIN/bin/$TARGET-strip
        export CMAKE_AR=$AR
        export CMAKE_RANLIB=$RANLIB
		;;
	*)
        export FFTW_ARCH_FOLDER=$ARCHITECTURE

        export TARGET=$ARCHITECTURE-linux-android
        export AR=$TOOLCHAIN/bin/$TARGET-ar
        export AS=$TOOLCHAIN/bin/$TARGET-as
        export CC=$TOOLCHAIN/bin/$TARGET$API-clang
        export CXX=$TOOLCHAIN/bin/$TARGET$API-clang++
        export LD=$TOOLCHAIN/bin/$TARGET-ld
        export RANLIB=$TOOLCHAIN/bin/$TARGET-ranlib
        export STRIP=$TOOLCHAIN/bin/$TARGET-strip
        export CMAKE_AR=$AR
        export CMAKE_RANLIB=$RANLIB
		;;
  esac

cmake -DBUILD_SHARED_LIBS=OFF -DCMAKE_C_FLAGS=-fPIC -DCMAKE_CXX_FLAGS=-fPIC -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE -DBUILD_TESTS=OFF -DFFTW3_INCLUDE_DIR=`realpath ../../fftw-3.3.8_compiled/$FFTW_ARCH_FOLDER/usr/local/include` ..
make -j4
'''

import argparse
import sys
import os
import subprocess
import logging

ROOT_CWD = os.getcwd()

logging.basicConfig(
    level=logging.DEBUG,
    format="%(asctime)s [%(levelname)s] %(message)s",
    handlers=[
        logging.StreamHandler()
    ]
)

ARCHITECTURES = {
    'AARCH64': 'aarch64',
    'ARM': 'arm',
    'X86_64': 'x86_64',
    'I686': 'i686'
}

ANDROID_CLANG_ARCH_PREFIX = {
    'AARCH64': 'aarch64',
    'ARM': 'armv7a',
    'X86_64': 'x86_64',
    'I686': 'i686'
}

LIBS = {
    'FFTW3': 'fftw3',
    'OBOE': 'oboe',
    'LIBSPEECH': 'libspeech'
}

ANDROID_PLATFORM_VERSION = 21

# def clean_fftw3():
#     pass

def _run_subproc(args, cwd=None):
    if cwd is not None:
        _process = subprocess.Popen(args, stdout=subprocess.PIPE, cwd=cwd)
    else:
        _process = subprocess.Popen(args, stdout=subprocess.PIPE)

    # def run_command(command):
    # process = subprocess.Popen(shlex.split(command), stdout=subprocess.PIPE)
    while True:
        output = _process.stdout.readline()
        if output == '' and _process.poll() is not None:
            break
        if output:
            print(output.strip().decode())
        rc = _process.poll()
        if rc is not None:
            print(output.strip().decode())
            return rc
    return None

def build_fftw3(args):
    logging.debug("building fftw3")
    _retcode = _run_subproc(['wget', '-c', FFTW3_URL])
    logging.debug("Downloaded {}, retcode {}".format(LIBS['FFTW3'], _retcode))

    # unzip and start building
    _retcode = _run_subproc(['tar', '--overwrite', "-zxvf", FFTW_ARCHIVE_NAME])
    logging.debug("Untarred {}".format(LIBS['FFTW3']))

    # recreate the output directory
    _run_subproc(['rm', '-rf', FFTW3_COMPILED_DIRECTORY])
    _retcode = _run_subproc(['mkdir', FFTW3_COMPILED_DIRECTORY])
    logging.debug("Recreated the {} output dir, retcode {}".format(LIBS['FFTW3'], _retcode))

    cmake_build_dir = os.path.join(FFTW_SRC_DIR, 'build')

    for key in ARCHITECTURES:
        output_dir = os.path.join(FFTW3_COMPILED_DIRECTORY, ARCHITECTURES[key])
        _retcode = _run_subproc(['mkdir', output_dir])

        # recreate the fftw3 build folder
        _run_subproc(['rm', '-rf', cmake_build_dir])
        _run_subproc(['mkdir', cmake_build_dir])

        # write the build script to the build folder
        _script = open(os.path.join(FFTW_SRC_DIR, 'build', 'build.sh'), "wt")
        _script.write(FFTW_BUILD_SCRIPT.format(args.sdk, ANDROID_PLATFORM_VERSION, ANDROID_CLANG_ARCH_PREFIX[key]))
        _script.close()

        _run_subproc(['make', 'clean'], cwd=cmake_build_dir)
        _run_subproc(['sh', 'build.sh'], cwd=cmake_build_dir)

        # move the binary into the appropriate folder
        # _run_subproc(['mv', os.path.join(FFTW_SRC_DIR, 'build', 'libfftw3.a'), output_dir])
        # _run_subproc(['DESTDIR=' + output_dir, os.path.join(FFTW_SRC_DIR, 'build', 'make')])
        _run_subproc(['make', 'install', 'DESTDIR=' + os.path.join(ROOT_CWD, output_dir)], cwd=cmake_build_dir)

    # print(FFTW_BUILD_SCRIPT.format(args.sdk, ANDROID_PLATFORM_VERSION, ARCHITECTURES['AARCH64']))

    # _process = subprocess.Popen(['wget', FFTW3_URL], stdout=subprocess.PIPE)

    # def run_command(command):
    # process = subprocess.Popen(shlex.split(command), stdout=subprocess.PIPE)
    # while True:
    #     output = _process.stdout.readline()
    #     if output == '' and _process.poll() is not None:
    #         break
    #     if output:
    #         print(output.strip())
    #     rc = _process.poll()
    #     if rc is not None:
    #         print("Downloaded fftw3", rc)
    #         print(output.strip())
    # return rc

    pass

def build_eigen(args):
    # generate the eigen headers first
    logging.debug("generating eigen headers")

    cmake_build_dir = os.path.join(EIGEN_SRC_DIR, 'build')

    # recreate the eigen build folder
    _run_subproc(['rm', '-rf', cmake_build_dir])
    _run_subproc(['mkdir', cmake_build_dir])
    _run_subproc(['cmake', '..'], cwd=cmake_build_dir)

    # Eigen3Config.cmake and eigen3 prefix merely point to the location at which
    # eigen is expected to be, hence no need to worry about actually cross compiling
    # eigen since it's also a header only library

def build_oboe(args):
    logging.debug("building oboe")
    # assume that the user already did do git submodule update --init --recursive

    # recreate the output directory
    _run_subproc(['rm', '-rf', OBOE_COMPILED_DIRECTORY])
    _retcode = _run_subproc(['mkdir', OBOE_COMPILED_DIRECTORY])
    logging.debug("Recreated the {} output dir, retcode {}".format(LIBS['OBOE'], _retcode))

    cmake_build_dir = os.path.join(OBOE_SRC_DIR, 'build')

    for key in ARCHITECTURES:
        output_dir = os.path.join(OBOE_COMPILED_DIRECTORY, ARCHITECTURES[key])
        _retcode = _run_subproc(['mkdir', output_dir])

        # recreate the oboe build folder
        _run_subproc(['rm', '-rf', cmake_build_dir])
        _run_subproc(['mkdir', cmake_build_dir])

        # write the build script to the build folder
        _script = open(os.path.join(OBOE_SRC_DIR, 'build', 'build.sh'), "wt")
        _script.write(OBOE_BUILD_SCRIPT.format(args.sdk, ANDROID_PLATFORM_VERSION, ANDROID_CLANG_ARCH_PREFIX[key]))
        _script.close()

        _run_subproc(['make', 'clean'], cwd=cmake_build_dir)
        _run_subproc(['sh', 'build.sh'], cwd=cmake_build_dir)

        _run_subproc(['make', 'install', 'DESTDIR=' + os.path.join(ROOT_CWD, output_dir)], cwd=cmake_build_dir)


def build_libspeech(args):
    build_eigen(args)

    # recreate the output directory
    _run_subproc(['rm', '-rf', LIBSPEECH_COMPILED_DIRECTORY])
    _retcode = _run_subproc(['mkdir', LIBSPEECH_COMPILED_DIRECTORY])
    logging.debug("Recreated the {} output dir, retcode {}".format(LIBS['LIBSPEECH'], _retcode))

    cmake_build_dir = os.path.join(LIBSPEECH_SRC_DIR, 'build')

    for key in ARCHITECTURES:
        output_dir = os.path.join(LIBSPEECH_COMPILED_DIRECTORY, ARCHITECTURES[key])
        _retcode = _run_subproc(['mkdir', output_dir])

        # recreate the libspeech build folder
        _run_subproc(['rm', '-rf', cmake_build_dir])
        _run_subproc(['mkdir', cmake_build_dir])

        # write the build script to the build folder
        _script = open(os.path.join(LIBSPEECH_SRC_DIR, 'build', 'build.sh'), "wt")
        _script.write(LIBSPEECH_BUILD_SCRIPT.format(args.sdk, ANDROID_PLATFORM_VERSION, ANDROID_CLANG_ARCH_PREFIX[key]))
        _script.close()

        _run_subproc(['make', 'clean'], cwd=cmake_build_dir)
        _run_subproc(['sh', 'build.sh'], cwd=cmake_build_dir)

        # _run_subproc(['make', 'install', 'DESTDIR=' + os.path.join(ROOT_CWD, output_dir)], cwd=cmake_build_dir)
        _run_subproc(['mkdir', '-p', os.path.join(output_dir, 'lib')])
        _run_subproc(['cp', os.path.join(cmake_build_dir, 'libspeech.a'), os.path.join(output_dir, 'lib')])

    pass

# def build_all():
#     pass

if __name__ == "__main__":
    parser = argparse.ArgumentParser()

    parser.add_argument("--sdk", help="path to android ndk r20b or above")
    parser.add_argument('--libs', nargs='+', help="space separated list of {fftw3, eigen, libspeech, and oboe} to rebuild", required=True)
    parser.add_argument("--clean", help="clean instead of building the libraries specified with --libs", action="store_true")

    args = parser.parse_args()

    if not args.clean: #assume build
        if not args.sdk:
            logging.error("must provide path to sdk if building")
            parser.print_help()
            sys.exit()
        if LIBS['FFTW3'] in args.libs:
            build_fftw3(args)
        if LIBS['OBOE'] in args.libs:
            build_oboe(args)
        if LIBS['LIBSPEECH'] in args.libs:
            build_libspeech(args)

# TODO: add clean functionality =)