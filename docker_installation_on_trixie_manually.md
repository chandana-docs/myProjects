**Chandana -**
**2025-11-20**

# Docker Installation on Debian 13 (Trixie) and Usage Notes
This document outlines the process for manually installing Docker on Debian 13 (Trixie) using 
specific local packages and provides instructions for managing images, containers, 
and creating images from tarballs.

## 1. Manual Docker Installation (Debian 13 Trixie, AMD64)
### Download Required Files
Navigate to the directory below and download the necessary stable AMD64 packages:

[https://download.docker.com/linux/debian/dists/trixie/pool/stable/amd64/](https://download.docker.com/linux/debian/dists/trixie/pool/stable/amd64/)

The required files are:

*   `containerd.io_2.1.5-1~debian.13~trixie_amd64.deb`
*   `docker-ce-cli_29.0.2-1~debian.13~trixie_amd64.deb`
*   `docker-buildx-plugin_0.30.0-1~debian.13~trixie_amd64.deb`
*   `docker-compose-plugin_2.40.3-1~debian.13~trixie_amd64.deb`
*   `docker-ce_29.0.2-1~debian.13~trixie_amd64.deb`

### Installation Commands
1.  **Install the downloaded packages:**

    Navigate to the directory containing the `.deb` files and run:

    ```bash
    sudo dpkg -i containerd.io_2.1.5-1~debian.13~trixie_amd64.deb \
    docker-ce-cli_29.0.2-1~debian.13~trixie_amd64.deb \
    docker-buildx-plugin_0.30.0-1~debian.13~trixie_amd64.deb \
    docker-compose-plugin_2.40.3-1~debian.13~trixie_amd64.deb \
    docker-ce_29.0.2-1~debian.13~trixie_amd64.deb
    ```
    > **Note**: If dependency errors occur, running `sudo apt install -f` (fix-broken) afterwards will usually
    > resolve them by fetching necessary prerequisites from the Debian repositories.

2.  **Manage User Groups:**

    Add the `docker` group and add your current user to that group to run Docker commands without needing `sudo`.

    ```bash
    sudo groupadd docker
    sudo usermod -aG docker $USER
    ```

3.  **Apply Group Changes:**

    Reboot the system for the group changes to take effect:

    ```bash
    sudo reboot
    ```
    
## 2. Creating a Docker Image using an RFS Tarball
If you have a Root File System (RFS) tarball (e.g., `deb11_x86_64_forbuild.tar.xz`), 
you can use a `Dockerfile` to convert it into a Docker image.

### `Dockerfile`
Create a file named `Dockerfile` in the same directory as your tarball and copy the below contents:
```dockerfile
# Stage 1: Extract the tarball into an intermediate container
FROM scratch AS extract

# ADD the tarball contents. They will land in a subdirectory named 'deb11' 
# at the root of this stage's filesystem because that's how the tarball is structured.
ADD deb11_x86_64_forbuild.tar.xz /

# Stage 2: Build the final image from scratch
FROM scratch AS base

# Define the default command to run when the container starts
CMD ["/bin/bash"]
```

### Build the Image
Use the `docker build` command to create the image from the `Dockerfile`.
```bash
docker build -t debian11:v1 .
```
>**Note:** Replace `debian11` and `v1` to the appropriate docker IMAGE and TAG name 

## 3. General Docker Container and Image Management Commands
### Check the created image:
```bash
docker images
```

### Run the docker image (creates and starts a container):
```bash
docker run -it debian11:v1 --name base
```
>**Note:**
>- Replace `debian11` and `v1` to the appropriate docker IMAGE and TAG name
>- Replace `base` with the appropriate container name

### List containers:
```bash
docker container ls -a
```

### Attach to a stopped container:
```bash
docker container start -ai base
```
>**Note:** Replace `base` with appropriate container ID or name

### Create a new image from a container (committing changes):
```bash
docker container commit <container_id> <new_image_name>:<new_tag_name>
```

### Mount a host directory while running a Docker image:
Use the `--mount` flag to share a local directory with the container:
```bash
docker run -it --mount type=bind,src=<host_dir>,dst=<mnt_point_inside_docker> --name <name_for_container> <image_name>:<image_tag>
```
>**Note:**
>- Replace `<host_dir>` with the directory name of the hostname
>- Replace `<mnt_point_inside_docker>` with the directory name inside the docker where the host dir should be mounted
>- Replace `<name_for_container>` with the correct container name
>- Replace `<image_name>:<image_tag>` with the correct Docker IMAGE and TAG name


