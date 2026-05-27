# Prepare Installation Media for Windows 11
**Author:** Chandana

## 1. Format the USB Flash Drive
Using a GUI tool like GParted is the safest way to handle partitions.

1. **Open GParted:**
```bash 
sudo gparted /dev/sdX  # Replace 'sdX' with your actual USB device identifier
```

2. Create a Partition Table:
- Navigate to **Device** -> **Create Partition Table...**
- Select partition type as **msdos**
- Click **Apply**

3. **Create a New Partition:**
- Right-click the unallocated space -> **New**.
- Ensure the file system is set to **fat32**.
- Click **Add**.

4. **Apply Changes:**
Click the Checkmark icon in the toolbar to apply operations.

5. **Enable Flags:**
- Right-click the **new partition** -> **Manage Flags**.
- Select **boot**.
- Save

6. **Close GParted.**

## 2. Mount the Devices
```bash
# Mount the USB partition
sudo mount /dev/sdX1 /mnt/

# Mount the Windows ISO
sudo mkdir -p /media/ISO/
sudo mount -o loop /path/to/windows.iso /media/ISO/
```

## 3. Copy Boot Files to USB
Windows 11 ISOs often contain an `install.wim` larger than **4GB**, which **FAT32 cannot handle**. Follow these steps to split it:
1. Copy all files except the main image:
```bash
sudo rsync -av --exclude='sources/install.wim' /media/ISO/ /mnt/
```

2. Install wimtools:
```bash
sudo apt update && sudo apt install wimtools
```
Split and copy the `install.wim` file:
This command splits the file into chunks smaller than **4000MB** and places them directly on the USB.
```bash
sudo wimlib-imagex split /media/ISO/sources/install.wim /mnt/sources/install.swm 4000
```

3. Verify the split images:
```bash
ls -l /mnt/sources/install*.swm
```

## 4. Cleanup
Always unmount before removing the USB to prevent data corruption.
```bash
cd ~/
sudo umount /mnt/
sudo umount /media/ISO/
```

## References
- [Microsoft: Install Windows from a Flash Drive](https://learn.microsoft.com/en-us/windows-hardware/manufacture/desktop/install-windows-from-a-usb-flash-drive?view=windows-11)
