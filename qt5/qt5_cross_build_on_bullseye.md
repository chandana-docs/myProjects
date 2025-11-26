**Chandana -**
**2025-11-21**

# QT5 Cross Build on Debian 11 (Bullseye) and Usage Notes
This document outlines the process for Cross Building and Manually installing QT5 on Debian 11 (Bullseye). Run QT5 examples without using X11 server.

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Build Preparation](#build-preparation)
3. [Install Required Packages](#install-required-packages)
4. [Setup and Build qt5](#setup-and-build-qt5)
5. [Run Qt5 on Target System](#run-qt5-on-target-system)

## Prerequisites
### Download QT5 Source
Navigate to the directory below and download the necessary tarball:
[https://mirror.aarnet.edu.au/pub/qtproject/archive/qt/5.15/5.15.2/single/](https://mirror.aarnet.edu.au/pub/qtproject/archive/qt/5.15/5.15.2/single/)

The required file is:
* `qt-everywhere-src-5.15.2.tar.xz`

### Download required files and folder from this Folder
* `am62x-aarch64-gnu-g++.tar.xz`
* `setup-env.sh`
* `kms.config`

## Build Preparation
* Prepare Debian 11 ARM64 RootFS under `debian11_arm64_rfs` directory for development
* Prepare Debian 11 x86 RootFS under `debian11_x86_rfs` directory for build
* Prepare Debian 11 ARM64 RootFS for deployment (to run examples)

## Install Required Packages
`chroot` into each of the rootfs to install the required packages

### On Development RootFS
* `chroot` into the RFS
```bash
cd debian11_arm64_rfs/
sudo chroot . /bin/bash
```

* Install packages
```bash
apt install pkg-config libdrm-dev libgbm-dev libegl1-mesa-dev libgles2-mesa-dev
```


### On Build RootFS
* `chroot` into the RFS
```bash
cd debian11_x86_rfs/
sudo mount --bind /dev/ dev/
sudo mount --bind /dev/pts dev/pts
sudo mount --bind /proc proc/
sudo mount --bind /sys sys/
sudo chroot . /bin/bash
```

* Install packages
```bash
apt install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu binutils-aarch64-linux-gnu build-essential pkg-config libdrm-dev libgbm-dev
```

### On Deployment RootFS
* Boot into Target system (Deployment RootFS) and install below packages
```bash
sudo apt install libgles2-mesa libegl1-mesa libgl1-mesa-dri
```

## Setup and Build qt5
**1. Preparation**
* Navigate to the home directory
```bash
cd ~/debian11_x86_rfs/
cd home/
```

* Copy the required files
```bash
cp ~/am62x-aarch64-gnu-g++.tar.xz .
cp ~/qt-everywhere-src-5.15.2.tar.xz .
cp ~/setup-env.sh .
```

</br>

* Prepar SYSROOT directory
```bash
mkdir targetfs
sudo mount --bind ~/debian11_arm64_rfs/ targetfs/
```

</br>

**2. `chroot` into the Build RootFS if not using Native Debian 11 system**
```bash
cd debian11_x86_rfs/
sudo mount --bind /dev/ dev/
sudo mount --bind /dev/pts dev/pts
sudo mount --bind /proc proc/
sudo mount --bind /sys sys/
sudo chroot . /bin/bash
```

</br>

**3. Untar the Source**
```bash
cd /home/
tar xf /home/qt-everywhere-src-5.15.2.tar.xz
```

</br>

**4. Add the custom device spec into the qt source**
```bash
cd qt-everywhere-src-5.15.2/qtbase/mkspecs/devices/
tar xf /home/am62x-aarch64-gnu-g++.tar.xz
```

</br>

**5. Move the `setup-env.sh` script into the qt source**
```bash
mv /home/setup-env.sh /home/qt-everywhere-src-5.15.2/
chmod +x /home/setup-env.sh
```

</br>

**6. Configure the QT5**
```bash
cd qt-everywhere-src-5.15.2/
./setup-env.sh
```

> ***Note**: While configuring make sure that GBM is installed as the script result shown as **Checking for GBM... yes***

</br>

**7. Build Qt5**
```bash
gmake -j$(nproc)
```

> ***Note**:*
> *If the **"undefined reference to __pointer_chk_guard_local"** error occurs during build, do modifications as shown below on **SYSROOT** and continue building the qt using `gmake -j$(nproc)`*
> ```bash
> mv /home/targetfs/usr/lib/aarch64-linux-gnu/libdl.a /home/targetfs/usr/lib/aarch64-linux-gnu/libdl.a.bak
> mv /home/targetfs/usr/lib/aarch64-linux-gnu/libpthread.a /home/targetfs/usr/lib/aarch64-linux-gnu/libpthread.a.bak
> ```

</br>

**8. Install Qt5 binaries**
```bash
gmake install -j$(nproc)
```

> ***Note**:*
> * *This is generate all the required files under `/home/qt-everywhere-src-5.15.2/INSTALL` directory.*
> * *The required files to run **qt5 examples** on target system can be found under `/home/qt-everywhere-src-5.15.2/INSTALL/target/opt/qt5u15`*
> * *Verify the three library files (`libqeglfs-emu-integration.so`, `libqeglfs-kms-egldevice-integration.so` and `libqeglfs-kms-integration.so`) are installed under `egldeviceintegrations` plugins.
> Otherwise, **“Could not initialize display”** error will occur when running the qt5 examples.*
> ```bash
> ls -l INSTALL/target/opt/qt5u15/plugins/egldeviceintegrations/
> ```

</br>

**9. Install Built Qt5 on Target system**

Follow one of the below two options to install,
* Copy the `INSTALL/target/opt/qt5u15` folder into the target systems’ `/opt/` directory (deployment system)
* Create a tarball of qt build binaries and install as shown below
On Build System,
```bash
cd /home/qt-everywhere-src-5.15.2
cd INSTALL/target
tar cfJ built-qt5.tar.xz opt/
```

> ***Note:***
> *Above command will generate `built-qt5.tar.xz` under `/home/qt-everywhere-src-5.15.2/INSTALL/target`*
> *Copy the `built-qt5.tar.xz` file to Target RootFS Home (`/home/linux/`) directory*
> *Install the qt5 binaries on Target system as below*
> ```bash
> cd /
> tar xf /home/linux/built-qt5.tar.xz
> ```
>
> *Verify the `/opt` folder on Target system, it should includes `qt5u15` folder*

</br>

## Run Qt5 on Target System
1. Boot into the Target System (Debian 11 ARM64)
2. Copy the downloaded `kms.config` file under `/etc/`

> ***Note:***
> *Contents of the `kms.config` file is as shown below*
> ```
> {
>        "device": "/dev/dri/card1",
>        "outputs": [
>        {
>                "name": "DVI-I-1",
>                "mode": "1920x1080"
>        }
> ]
> }
> ```
>
> *Replace `card1`, name (`DVI-I-1`) and mode (`1920x1080`) accordingly*
  
3. Untar the `built-qt5.tar.xz` under `/` directory or copy the `qt5u15` folder under `/opt`
```bash
cd /
tar xf /home/linux/built-qt5.tar.xz
```

4. Go the specific Qt5 example directory
```bash
cd /opt/qt5u15/examples/qpa/qrasterwindow/
```

5. Run the example
```bash
export QT_QPA_EGLFS_KMS_CONFIG=/etc/kms.config
./qrasterwindow
```

> ***Note:***
> * *After running the running the example, **“evdi: [I] (card1) Notifying display power state: on”** message should occur*
> * *`Ctrl + C` to stop the example*
