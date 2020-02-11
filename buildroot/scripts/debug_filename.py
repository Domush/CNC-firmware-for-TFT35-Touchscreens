import os
Import("env")

print("Creating debug-compatible firmware.elf link")
if os.path.exists('D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' + "firmware.elf"):
    os.unlink('D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' + "firmware.elf")
if os.path.exists('D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' + env["PROGNAME"] + ".elf"):
    os.link('D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' + env["PROGNAME"] + ".elf",'D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' + "firmware.elf")
print("Creating debug-compatible firmware.bin link")
if os.path.exists('D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' + "firmware.bin"):
    os.unlink('D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' + "firmware.bin")
if os.path.exists('D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' + env["PROGNAME"] + ".bin"):
    os.link('D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' + env["PROGNAME"] + ".bin",'D:/Google Drive/Development/GitHub/BIGTREETECH-TouchScreenFirmware/.pio/build/BIGTREE_TFT35_V2_0/' + "firmware.bin")
