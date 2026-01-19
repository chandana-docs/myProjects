# How to Rotate the GRUB Screen on an EFI System (Experimental/Custom Build)
This guide outlines an experimental method to patch, build, and install a custom version of GRUB that supports screen rotation via a configuration flag.

⚠️ **Warning**: This process involves compiling a custom bootloader and overwriting system files. Proceed with caution. Incorrect execution may render your system unbootable.

This guide uses specific commands tailored for **Debian/Ubuntu-based systems**. Adjust package names (`apt install ...`) for other distributions (Fedora, Arch, etc.).

## Prerequisites
* A Debian-based Linux distribution.
* An active internet connection to clone the repository and install dependencies.

## Instructions
Follow these steps:

### 1. Clone the Custom GRUB Repository
Clone the specific repository branch that contains the rotation patch:
```bash
git clone https://github.com/kbader94/grub.git
cd grub/
git checkout fix-grub-mkconfig
```

>Note: If SSL verification issue occures while cloning, follow the below step:
>```bash
>git -c http.sslVerify=false clone https://github.com/kbader94/grub.git
>```

### 2. Install Build Dependencies
You need several packages to compile GRUB from source.
```bash
sudo apt update
sudo apt install build-essential bison flex gettext autoconf automake texinfo grub-common grub-pc-bin grub-pc grub2-common autopoint gawk pkg-config m4 libtool
```

### 3. Build and Compile GRUB
Navigate to the repository you cloned and start the build process. This assumes you are building for a standard `x86_64` EFI system.
```bash
# Prepare the build environment
./bootstrap

# Create a build directory and enter it
mkdir build && cd build

# Configure the build for EFI
../configure \
    --prefix=/usr \
    --sysconfdir=/etc \
    --libdir=/usr/lib \
    --sbindir=/usr/sbin \
    --with-platform=efi \
    --target=x86_64 \
    --disable-werror \
    --disable-pc \
    --disable-nls

# Compile the source code (uses all available CPU cores for speed)
make -j$(nproc)
```

### 4. Install the New GRUB Binary
Install the newly compiled binaries onto your system, overwriting the default ones.
```bash
sudo make install
```

### 5. Reinstall GRUB to the EFI Boot Sector
This command installs the new GRUB system files to your EFI partition (`/boot/efi`). 

**Ensure you specify the correct mount point for your EFI partition if it differs.**
```bash
sudo grub-install --target=x86_64-efi --efi-directory=/boot/efi --bootloader-id=GRUB
```

### 6. Configure the Rotation Setting
Now that the custom GRUB is installed, you can use the new rotation feature via the standard configuration file.

Open the GRUB default configuration file:
```bash
sudo nano /etc/default/grub
```

Add the following line to the file, choosing your desired rotation angle:
```bash
GRUB_FB_ROTATION=90
```
**Available Options:**
* `0`: Normal (`Default`)
* `90`: Rotate 90 degrees (`left`)
* `180`: Rotate 180 degrees (`right`)
* `270`: Rotate 270 degrees clockwise (`inverted`)

Save the file and exit the editor.

### 7. Update the GRUB Configuration File
Run the command to generate the final configuration file (`/boot/grub/grub.cfg`). This process will now include your new `GRUB_FB_ROTATION` setting because you are using the patched `grub-mkconfig` utility.
```bash
sudo update-grub
```

You can quickly verify that the configuration was processed correctly:
```bash
head /boot/grub/grub.cfg
```
Look for comments or settings that indicate the new configuration was included as below.
```bash
...from /etc/grub.d and settings from /etc/default/grub
```

### 8. Reboot
Reboot your system to test the new screen orientation in the GRUB boot menu.
```bash
sudo reboot
```

### References
* [https://hackaday.io/project/203272/instructions](https://hackaday.io/project/203272/instructions)
* [https://mail.gnu.org/archive/html/grub-devel/2024-06/msg00164.html](https://mail.gnu.org/archive/html/grub-devel/2024-06/msg00164.html)

