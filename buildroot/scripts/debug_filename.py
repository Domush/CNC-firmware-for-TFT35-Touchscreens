import os
import time
import pprint
Import("env")


def debug_enable(target, source, env):
    build_flags = env.ParseFlags(env['BUILD_FLAGS'])
    flags = {k: v for (k, v) in build_flags.get("CPPDEFINES")}
    filename = flags.get("HARDWARE") + "." + flags.get("SOFTWARE_VERSION") + "." + \
        time.strftime("%m.%d.%H.%M", time.localtime())

    if (flags.get("TFT35_V3_0") == ''):
        bin_dir = "D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V3_0/"
    else:
        bin_dir = "D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/"

    elf_firmware = bin_dir + "firmware.elf"
    bin_firmware = bin_dir + "firmware.bin"
    bin_filename = filename + ".bin"
    elf_filename = filename + ".elf"

    # print("Searching for firmware.elf")
    if os.path.exists(elf_firmware):
        os.unlink(elf_firmware)
        # print("Removed old firmware.elf")

    # print("Searching for " + elf_filename)
    if os.path.exists(bin_dir + elf_filename):
        print("Found " + elf_filename + " - Linking..")
        os.link(bin_dir + elf_filename, elf_firmware)
        print("firmware.elf successfully linked")
    else:
        print(elf_filename + " not found.")
        print("**Debugging may not be possible**")

    # print("Searching for firmware.bin")
    if os.path.exists(bin_firmware):
        # print("Found firmware.bin Deleting..")
        os.unlink(bin_firmware)

    print("Searching for " + bin_filename)
    if os.path.exists(bin_dir + bin_filename):
        print("Found " + filename + " - Linking..")
        os.link(bin_dir + bin_filename, bin_firmware)
        print("firmware.bin successfully linked")
    else:
        print(bin_filename + " not found.")
        print("**Debugging may not be possible**")


env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", debug_enable)
