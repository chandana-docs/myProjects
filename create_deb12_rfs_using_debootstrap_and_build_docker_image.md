# Creating a Debian 12 RootFS and Docker Image via debootstrap
This guide outlines the steps required to generate a minimal Debian 12 (Bookworm) root filesystem using debootstrap, package it into a compressed tarball, and use that tarball to create a new Docker image.

## Prerequisites
Ensure you have `debootstrap`, `tar`, and Docker installed on your host machine.

## Step 1: Generate the Minimal Debian 12 Root Filesystem
We use `debootstrap` to download and extract the base Debian packages into a dedicated directory (`/srv/debian12_rootfs`).
```bash
# Create the target directory
sudo mkdir -p /srv/debian12_rootfs

# Run debootstrap to populate the directory with Debian 12 (Bookworm) minimal files
sudo debootstrap --arch=amd64 bookworm /srv/debian12_rootfs
```

> Note: We can also specify the default Debian mirror URL as below
> ```bash
> sudo debootstrap --arch=amd64 bookworm /srv/debian12_rootfs
> ```

## Step 2: Configure the Base System (Optional)
You can use `chroot` to enter the new environment and perform basic configurations, such as setting a root password or hostname.
```bash
# Mount necessary pseudo-filesystems for chroot to function correctly
sudo mount -t proc /proc /srv/debian12_rootfs/proc
sudo mount -t sysfs /sys /srv/debian12_rootfs/sys
sudo mount -o bind /dev /srv/debian12_rootfs/dev

# Chroot into the new environment
sudo chroot /srv/debian12_rootfs /bin/bash

# --- Inside the chroot environment (your prompt changes here) ---
# Set the root password (optional, but recommended for non-container use)
passwd root

# Set the hostname
echo "deb12" > /etc/hostname

# Exit the chroot
exit

# --- Back on the host machine ---
# Unmount the filesystems
sudo umount /srv/debian12_rootfs/dev
sudo umount /srv/debian12_rootfs/sys
sudo umount /srv/debian12_rootfs/proc
```

## Step 3: Package the RootFS into a Compressed Tarball
Navigate into the rootfs directory and use `tar` to archive and compress the contents. This file will serve as the base layer for our Docker image.
```bash
# Change directory into the rootfs
cd /srv/debian12_rootfs

# Create a compressed tarball (.tar.gz) in the /tmp directory
sudo tar --numeric-owner -czvf /tmp/debian12-rootfs.tar.gz .
```

## Step 4: Create the Dockerfile
We will use the generated tarball as the base layer for a new Docker image using the minimal `FROM scratch` approach.

First, navigate to your desired build directory and copy the tarball and Dockerfile there:
```bash
cd ~
cp /tmp/debian12-rootfs.tar.gz .
# Make sure you have a Dockerfile in your current directory
```

Create a `Dockerfile` with the following content:
```bash
# Dockerfile content
# Use the minimal base image 'scratch' as the starting point
FROM scratch

# Define an argument for the tarball filename
ARG IMGTARBALL=./debian12-rootfs.tar.gz

# Add the contents of the tarball to the root of the container filesystem
ADD "$IMGTARBALL" /

# Set the working directory inside the container
WORKDIR /root
```

## Step 5: Build and Run the Docker Image
Finally, build the Docker image and test it by running an interactive shell session.
```bash
# Build the Docker image, tagging it as 'deb12-debootstrap'
sudo docker build -t deb12-debootstrap .

# List local Docker images to verify the new image size
docker images

# Run the new container in interactive mode and start a bash shell
docker run -it deb12-debootstrap /bin/bash
```
You are now inside your minimal Debian 12 Docker container!

