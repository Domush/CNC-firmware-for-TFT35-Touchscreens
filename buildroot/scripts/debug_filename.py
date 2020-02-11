import os
Import("env")

# print("Creating debug-compatible firmware.elf link")

if os.path.exists('D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' + "firmware.elf"):
    os.unlink('D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' + "firmware.elf")
    print("Removing old firmware.elf file..")

if os.path.exists('D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' + env["PROGNAME"] + ".elf") and os.path.exists('D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' + env["PROGNAME"]):
    os.unlink('D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' + env["PROGNAME"] + ".elf")

if not os.path.exists('D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' + env["PROGNAME"] + ".elf") and os.path.exists('D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' + env["PROGNAME"]):
    print("*.elf file not found, creating " + env["PROGNAME"] + ".elf")
    os.link('D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' +
            env["PROGNAME"], 'D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' + env["PROGNAME"] + ".elf")
            
if os.path.exists('D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' + env["PROGNAME"] + ".elf"):
    os.link('D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' +
            env["PROGNAME"] + ".elf", 'D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' + "firmware.elf")
    print("firmware.elf file linked to " + env["PROGNAME"] + ".elf")

# print("Creating debug-compatible firmware.bin link")

if os.path.exists('D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' + "firmware.bin"):
    os.unlink('D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' + "firmware.bin")

if os.path.exists('D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' + env["PROGNAME"] + ".bin"):
    os.link('D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' +
            env["PROGNAME"] + ".bin", 'D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' + "firmware.bin")
    print("firmware.bin file linked to " + env["PROGNAME"] + ".bin")
else:
    print("No *.bin file found")
