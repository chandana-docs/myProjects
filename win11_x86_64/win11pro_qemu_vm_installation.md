# How to Generate a Windows 11 Pro QEMU VM Image
This guide covers preparing the environment, running the QEMU installation process, bypassing Windows 11 minimum requirements and network checks, and installing necessary VirtIO drivers for optimal performance.

## Prerequisites
Ensure you have QEMU (`qemu-system-x86`) installed and the following ISO images downloaded:
- `en-us_windows_11_consumer_editions_version_23h2_updated_oct_2024_x64_dvd_4728d672.iso`
- `virtio-win-0.1.266.iso`

## Step 1: Create the QEMU VM Image File
```bash
qemu-img create -f qcow2 win11pro_x86.qcow2 50G
```

## Step 2: Start the VM Installation
Use the following command (or wrap it in an `install.sh` script) to boot the QEMU VM, mounting both the Windows ISO and the VirtIO driver ISO.
```bash
#!/bin/bash

# Configuration variables
RAM_SIZE="4096"  
CPU_CORES="4"       
DISK_IMAGE="win11pro_x86.qcow2"
ISO_IMAGE="en-us_windows_11_consumer_editions_version_23h2_updated_oct_2024_x64_dvd_4728d672.iso"

echo "Running QEMU VM..."
qemu-system-x86_64 \
    -machine q35,kernel_irqchip=on \
    -enable-kvm \
    -cpu host,kvm=off,hv_vendor_id=null \
    -bios /usr/share/ovmf/OVMF.fd  \
    -smp "$CPU_CORES" \
    -m "$RAM_SIZE" \
    -drive format=qcow2,file="$DISK_IMAGE" \
    -cdrom "$ISO_IMAGE" \
    -drive file=virtio-win-0.1.266.iso,media=cdrom,format=raw,id=drivers,if=none \
    -device usb-storage,drive=drivers,serial=drivers \
    -name "win11-pro" \
    -net none \
    -usb \
    -device qemu-xhci \
    -vga virtio \
```
> Note: This command will open the QEMU window and boot from the Windows 11 ISO.

## Step 3: Follow the Windows Installer Prompts
1. Select country and language settings.
2. Enter a Product Key if available, or select "I don't have a product key".
3. Select the Windows 11 Pro edition to install.

## ⚠️ Bypass Minimum Requirements Check (TPM/Secure Boot/RAM)
If you encounter the error message: **"This PC can't run Windows 11"**:
1. On that error screen, press `Shift + F10` simultaneously to open a Command Prompt window.
2. Type `regedit` and press `Enter`. This opens the Registry Editor.
3. Navigate to `HKEY_LOCAL_MACHINE\SYSTEM\Setup`.
4. **Create the LabConfig Key:** Right-click on the `Setup` key, select **New** > **Key**, and name it **LabConfig**.
5. **Create DWORD Values:** Inside the `LabConfig` key, create the following four DWORD (32-bit) values and set their **Data value to `1`**:
    - `BypassTPMCheck`
    - `BypassSecureBootCheck`
    - `BypassRAMCheck`
    - `BypassCPUCheck`
6. Close the Registry Editor and the Command Prompt.
7. Click the **Back arrow** in the Windows installer error window, and then click **Next** again. The minimum requirements error will now be bypassed.

## Step 4: Continue Installation 
1. Accept the license terms and conditions.
2. Select **"Custom: Install Windows only (advanced)"**.
3. Select the drive (Drive 0 Unallocated Space) and click Next. Windows will begin installing.

> Note: The installer may not see the `virtio` drive yet. Click **"Load driver"**, navigate to the VirtIO CD drive (`virtio-win-0.1.266.iso`), browse the correct OS folder (`amd64\w11` or similar), and install the drivers. The main `win11pro_x86.qcow2` drive should now appear.

## Step 5 : Initial Windows Setup
1. Select country and keyboard language.

## ⚠️ Bypass Network Requirement (No Internet)
If you do not have network connectivity configured in QEMU yet and need to continue without an internet connection:

1. When you reach the network connection screen, press `Shift + F10` to open a **Command Prompt**.
2. Type `OOBE\BYPASSNRO` and press `Enter`.
3. The system will reboot and return to the OOBE screen. Select your region/keyboard again.
4. This time, the network screen will present an **"I don't have internet"** option. Select it.
5. Click on **"Continue with limited setup"**.
6. Set up a local account.
7. Deselect all options under the privacy settings window.

Windows 11 will now boot into the desktop.

## Step 6: Install VirtIO Drivers inside the VM
For better performance, network access, and correct display resolution adjustment, you must install the VirtIO drivers.
1. Open **File Explorer** inside the VM.
2. Click on the **CD Drive** labeled `virtio-win-0.1.266.iso` (or similar name/drive letter).
3. Double-click on the installer binary (usually `virtio-win-gt-x64.exe`).
4. Follow the installation prompts: **Next** > **Accept license agreement** > **Next** > **Install**.
5. Click **Yes** when prompted by UAC.
6. The system resolution should adjust automatically during or immediately after installation.
7. Click **Finish**.

Your Windows 11 Pro QEMU VM is now ready for use. You can shut down the VM and remove the `-cdrom` options from your QEMU start script for future boots.












