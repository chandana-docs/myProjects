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
