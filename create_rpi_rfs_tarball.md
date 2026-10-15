## Create and boot into custom RFS tarball

## Create Rpi RFS tarball

Download the official image (Raspberry Pi OS Lite, Ubuntu Server, etc.) and loop back mount to the RFS inside tar.

1. [Raspberry Pi OS official website](https://www.raspberrypi.com/software/operating-systems/) From the latest from 64-bit Lite Image (.img.xz) downloading **Raspberry Pi OS Lite** in this example. This will download `2026-10-06-raspios-trixie-arm64-lite.img` file.

2. Unpack it.
    ```bash
    unxz 2026-10-06-raspios-trixie-arm64-lite.img.xz
    ```

3. Check the partition structure of the image.
    ```bash
    sudo fdisk -l 2026-10-06-raspios-trixie-arm64-lite.img
    ```

4. Get the offset of the root partition (second partition).
    ```bash
    partx -g 2026-10-06-raspios-trixie-arm64-lite.img | awk 'NR==2 {print $2 * 512}'
    ```

    This will print the number similar to `545259520`. Copy this.

5. Mount the partition.
    ```bash
    sudo mkdir /mnt/rpi-rootfs
    sudo mount -o loop,offset=545259520 2026-10-06-raspios-trixie-arm64-lite.img /mnt/rpi-rootfs
    ```

    > Note: Replace `545259520` with correct number resulted from the previous step.

6. Create an archive (tarball) and unmount the mounted partition(sector).
    ```bash
    cd /mnt/rpi-rootfs
    sudo tar -cpzf ~/rpi5-rootfs.tar.gz .
    cd ~/
    sudo umount /mnt/rpi-rootfs
    ```

## Boot into RFS
1. Shrink the Existing Partition & Create a New One.
2. Extract the RFS Tarball
    ```bash
    sudo mount /dev/sdc3 /mnt/
    cd /mnt
    sudo tar xf rpi5-rootfs.tar.gz
    ```
    > Note: Replace `/dev/sdc3` with the correct partition which is created in the previous step.

3. Update `/etc/fstab` inside the New RFS
    - Update the `PARTUUID=` of the partitions.

4. Switch the Boot Target
    ```bash
    sudo mkdir -p /media/bootfs
    sudo mount /dev/sdc1 /media/bootfs
    cd /media/bootfs
    sudo nano cmdline.txt
    ```
    > Note: Replace `/dev/sdc1` with the correct boot partition.

5. Look for the `root=` parameter. It will likely say `root=PARTUUID=...` or `root=/dev/mmcblk0p2`.       
6. Change it to point to your new partition:
    - `root=/dev/mmcblk0p3` or correct `PARTUUID`
  
7. Save and exit. 
8. Reboot the Rpi.
