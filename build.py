import os
import sys
import subprocess
import time
import glob
from termcolor import colored
from pathlib import Path    # to check if a directory exists

common_path = "/media/master/0E5513DF0E5513DF/Work/esp8266/Toolchain/xtensa-lx106-elf-gcc8_4_0-esp-2020r3-linux-amd64/xtensa-lx106-elf/bin/"

esptool2 = "/media/master/0E5513DF0E5513DF/Work/esp8266/Projects/esptool2/esptool2"

output_file_name = "main"

port = "/dev/ttyUSB0"

asm_path = f"{common_path}xtensa-lx106-elf-as"
compiler_path = f"{common_path}xtensa-lx106-elf-gcc"
linker_path = f"{common_path}xtensa-lx106-elf-ld"
objdump_path = f"{common_path}xtensa-lx106-elf-objdump"
objcopy_path = f"{common_path}xtensa-lx106-elf-objcopy"

linker_script = "ld/linkerscript.ld -T ld/addresses.ld"

compiler_flags = ["-flto",
                  "-Ofast",
                  "-Iinclude",
                  "-DMAIN_MHZ=160",
                  "-mno-serialize-volatile",
                  "-mlongcalls",
                  "-g"]

linker_flags = [f"-T {linker_script}",
                "-Wl,--print-memory-usage",
                ]


def compile(input_file:str) -> int:
    ''' returns 0 if error, 1 if ok '''
    print(f"file: {input_file}", flush=True)
    time.sleep(0.1)
    output_file = f"temp_files/{input_file.split('/')[-1].split('.')[0]}.o"
    c_string = f'{compiler_path} {" ".join(compiler_flags)} -c {input_file} -o {output_file}'
    return subprocess.call(c_string,shell=True)


def compile_all(files:list):
    print(f"files to compile: {' '.join(files)}")
    for item in files:
        if compile(item) == 1:
            print(colored(f"{item} compilation error", 'red'))
            sys.exit(0)
    print(colored("compilation completed successfully", 'green'))
    return 0


def link_all(files:list):
    l_string = f'{compiler_path} {" ".join(compiler_flags)} {" ".join(linker_flags)} {" ".join(files)} -o temp_files/{output_file_name}.elf'
    retval = subprocess.call(l_string, shell=True)
    if retval == 1:
        print(colored("linking error", 'red'))
        sys.exit(0)
    return retval


def get_files(path:str, filetype:str)->list:
    result = []

    for x in os.walk(path):
        for y in glob.glob(os.path.join(x[0], f'*{filetype}')):
            temp = y[len(path)+1:].replace("\\", "/")
            result.append(temp)
    return result


def convert_elf():
    e_string = f'esptool elf2image temp_files/{output_file_name}.elf -o {output_file_name}.bin'
    print(e_string)
    subprocess.call(e_string, shell=True)
    os.rename(f'{output_file_name}.bin0x00000.bin', f'{output_file_name}.bin')
    print(colored(f"file {output_file_name}.bin ready to be uploaded at address 0x00000000", 'green'))


def build():
    Path("temp_files").mkdir(parents=True, exist_ok=True) # create temp directory if there is no one
    workdir = os.path.abspath(os.getcwd())
    if 0 == compile_all(get_files(workdir, ".c")+get_files(workdir, ".S")) and 0 == link_all(get_files(workdir, ".o")):
        convert_elf()
        subprocess.call('rm -r temp_files/*', shell=True)
        return 1
    return 0


def flash_chip():
    os.system(f"sudo esptool --port {port} write_flash --flash_size=detect --flash_mode dio 0x00000 {output_file_name}.bin")


if __name__ == '__main__':
    if len(sys.argv) > 1:
        for i in range(1, len(sys.argv)):
            if sys.argv[i] == 'build' and build():
                sys.exit(0)
            elif sys.argv[i] == 'flash':
                flash_chip()
    else:
        build()
