# Shell Commands

### To tell `apt` to ignore an HTTPS peer verification error
```bash
sudo apt -o "Acquire::https::Verify-Peer=false" update
sudo apt -o "Acquire::https::Verify-Peer=false" install <package_name>
```

### To Skip SSL verification in Git
```bash
export GIT_SSL_NO_VERIFY=true
```

To skip SSL certificate verification when cloning a repository,
```bash
git -c http.sslVerify=false clone <github_repo>
```

### How to Assign a Static IP Address Temporarily on Linux
```bash
sudo ip addr add 192.168.0.235/24 dev eth0
```

> **Note**:
> * You must run this command with administrative privileges (`sudo`).
> * Replace `192.168.0.235` with your desired IP address and `eth0` with the correct network interface name.
> * Use the command `ip a`(or `ip addr show`) to list your active network interfaces and identify the correct name (e.g., `eth0`, `enp3s0`, or `wlan0`).

### How to Assign a Static IP Address on a Linux System using the `/etc/network/interfaces` file
1. Identify Your Network Interface Name
```bash
ip a
```
2. Configure the Static IP Address
```bash
# The primary network interface
allow-hotplug enp2s0
iface enp2s0 inet static
        address 192.168.1.100
        netmask 255.255.255.0
```
3. Save and Apply Changes
```bash
sudo systemctl restart networking.service
```

### How to assign a `192.168.x.x` network to Docker containers via a specific host interface
1. Find your host network interface name and subnet:
```bash
ip a
```
> **Note**:
> Look for your active interface (e.g., `eth0`, `enp3s0`, or `ens192`) and note your 192 subnet range (e.g., `192.168.1.0/24`).

2. Create the Macvlan network in Docker:
```bash
sudo docker network create -d macvlan --subnet=192.168.0.0/24 --gateway=192.168.0.1 -o parent=enp3s0 my_network
```

3. Run a container with a static 192.x IP address:
```bash
docker run -d \
  --name my_container \
  --network my_network \
  --ip 192.168.0.6 \
  nginx
```
> **Note**:
> Replace `nginx` with the `/sbin/init` if needed.

4. Check the IP address assignment using:
```bash
ip a
```
