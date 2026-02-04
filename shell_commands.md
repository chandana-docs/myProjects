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
