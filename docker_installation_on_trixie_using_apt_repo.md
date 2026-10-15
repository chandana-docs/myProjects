**Chandana -**
**2026-08-27**

# Docker Installation on Debian 13 (Trixie) using `apt` repository
This document outlines the process for installing Docker on Debian 13 (Trixie) using 
`apt` repository.

Run the following command to uninstall all conflicting packages:
```bash
 sudo apt remove $(dpkg --get-selections docker.io docker-compose docker-doc docker-buildx podman-docker containerd runc | cut -f1)
```

## 1. Set up Docker's `apt` repository.
```bash
# Add Docker's official GPG key:
sudo apt update
sudo apt install ca-certificates curl
sudo install -m 0755 -d /etc/apt/keyrings
sudo curl -fsSL https://download.docker.com/linux/debian/gpg -o /etc/apt/keyrings/docker.asc
sudo chmod a+r /etc/apt/keyrings/docker.asc

# Add the repository to Apt sources:
sudo tee /etc/apt/sources.list.d/docker.sources <<EOF
Types: deb
URIs: https://download.docker.com/linux/debian
Suites: $(. /etc/os-release && echo "$VERSION_CODENAME")
Components: stable
Architectures: $(dpkg --print-architecture)
Signed-By: /etc/apt/keyrings/docker.asc
EOF

sudo apt update
```

## 2. Install the Docker packages.
```bash
sudo apt install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
```

## 3. (Optional) Allow non-root user to manage Docker:
```bash
sudo usermod -aG docker $USER
```
Log out and Log in back.

## 4. After installation, verify that Docker is running:
```bash
sudo systemctl status docker
```

> [!NOTE]
>  When the status of the docker systemctl service is  giving error as below:
> ```bash
> root@minimal:/home/skelios/elera_pos_flutter# systemctl status docker
> × docker.service - Docker Application Container Engine
>      Loaded: loaded (/usr/lib/systemd/system/docker.service; enabled; preset: enabled)
>      Active: failed (Result: exit-code) since Thu 2026-08-27 09:54:20 JST; 4min 17s ago
>  Invocation: 67f2b9e98a4a4a03841a0c6932b4d601
> TriggeredBy: × docker.socket
>        Docs: https://docs.docker.com
>     Process: 3634 ExecStart=/usr/bin/dockerd -H fd:// --containerd=/run/containerd/containerd.sock (code=exited, status=1/FAILURE)
> Main PID: 3634 (code=exited, status=1/FAILURE)
>
> Aug 27 09:54:18 minimal systemd[1]: Failed to start docker.service - Docker Application Container Engine.
> Aug 27 09:54:20 minimal systemd[1]: docker.service: Scheduled restart job, restart counter is at 3.
> Aug 27 09:54:20 minimal systemd[1]: docker.service: Start request repeated too quickly.
> Aug 27 09:54:20 minimal systemd[1]: docker.service: Failed with result 'exit-code'.
> Aug 27 09:54:20 minimal systemd[1]: Failed to start docker.service - Docker Application Container Engine.
> ```
>
> To see the exact reason why the Docker daemon itself crashed, run:
> ```bash
> journalctl -u docker.service -n 50 --no-pager
> ```
> The partial output of the above command:
> ```bash
> Aug 27 09:54:17 minimal systemd[1]: docker.service: Scheduled restart job, restart counter is at 2.
> Aug 27 09:54:17 minimal systemd[1]: Starting docker.service - Docker Application Container Engine...
> Aug 27 09:54:17 minimal dockerd[3634]: time="2026-08-27T09:54:17.820984750+09:00" level=info msg="Starting up"
> Aug 27 09:54:17 minimal dockerd[3634]: time="2026-08-27T09:54:17.828954250+09:00" level=info msg="OTEL tracing is not configured, using no-op tracer provider"
> Aug 27 09:54:17 minimal dockerd[3634]: time="2026-08-27T09:54:17.829531300+09:00" level=info msg="CDI directory does not exist, skipping" dir=/var/run/cdi
> Aug 27 09:54:17 minimal dockerd[3634]: time="2026-08-27T09:54:17.829630600+09:00" level=info msg="CDI directory does not exist, skipping" dir=/etc/cdi
> Aug 27 09:54:17 minimal dockerd[3634]: time="2026-08-27T09:54:17.876702300+09:00" level=info msg="Creating a containerd client" address=/run/containerd/containerd.sock timeout=1m0s
> Aug 27 09:54:17 minimal dockerd[3634]: time="2026-08-27T09:54:17.880131475+09:00" level=warning msg="failed check for fsverity support" error="enable fsverity failed: operation not supported" path=/var/lib/docker/plugins/storage
> Aug 27 09:54:17 minimal dockerd[3634]: time="2026-08-27T09:54:17.897381425+09:00" level=info msg="Loading containers: start."
> Aug 27 09:54:17 minimal dockerd[3634]: time="2026-08-27T09:54:17.897622100+09:00" level=info msg="NRI is disabled"
> Aug 27 09:54:18 minimal dockerd[3634]: time="2026-08-27T09:54:18.156823475+09:00" level=info msg="[graphdriver] using prior storage driver: overlay2"
> Aug 27 09:54:18 minimal dockerd[3634]: time="2026-08-27T09:54:18.157612975+09:00" level=info msg="Restoring containers: start."
> Aug 27 09:54:18 minimal dockerd[3634]: time="2026-08-27T09:54:18.198783325+09:00" level=info msg="Deleting nftables IPv4 rules" error="running nft: /dev/stdin:1:17-30: Error: Could not process rule: No such file or directory\ndelete table ip docker-bridges\n                ^^^^^^^^^^^^^^\n exit status 1"
> Aug 27 09:54:18 minimal dockerd[3634]: time="2026-08-27T09:54:18.226721375+09:00" level=info msg="Deleting nftables IPv6 rules" error="running nft: /dev/stdin:1:18-31: Error: Could not process rule: No such file or directory\ndelete table ip6 docker-bridges\n                 ^^^^^^^^^^^^^^\n exit status 1"
> Aug 27 09:54:18 minimal dockerd[3634]: time="2026-08-27T09:54:18.468993575+09:00" level=info msg="stopping event stream following graceful shutdown" error="<nil>" module=libcontainerd namespace=moby
> Aug 27 09:54:18 minimal dockerd[3634]: time="2026-08-27T09:54:18.471365075+09:00" level=info msg="Daemon shutdown complete" error="failed to start daemon: Error initializing network controller: error obtaining controller instance: failed to register \"bridge\" driver: failed to create NAT chain DOCKER: iptables failed: iptables --wait -t nat -N DOCKER: iptables v1.8.11 (nf_tables):  TABLE_ADD failed (Operation not supported): table nat\n (exit status 4)"
> Aug 27 09:54:18 minimal dockerd[3634]: failed to start daemon: Error initializing network controller: error obtaining controller instance: failed to register "bridge" driver: failed to create NAT chain DOCKER: iptables failed: iptables --wait -t nat -N DOCKER: iptables v1.8.11 (nf_tables):  TABLE_ADD failed (Operation not supported): table nat
> Aug 27 09:54:18 minimal dockerd[3634]:  (exit status 4)
> Aug 27 09:54:18 minimal systemd[1]: docker.service: Main process exited, code=exited, status=1/FAILURE
> Aug 27 09:54:18 minimal systemd[1]: docker.service: Failed with result 'exit-code'.
> Aug 27 09:54:18 minimal systemd[1]: Failed to start docker.service - Docker Application Container Engine.
> Aug 27 09:54:20 minimal systemd[1]: docker.service: Scheduled restart job, restart counter is at 3.
> Aug 27 09:54:20 minimal systemd[1]: docker.service: Start request repeated too quickly.
> Aug 27 09:54:20 minimal systemd[1]: docker.service: Failed with result 'exit-code'.
> Aug 27 09:54:20 minimal systemd[1]: Failed to start docker.service - Docker Application Container Engine.
> ```
>
> Check for the Common iptables / NFTables Issue:
> On minimal Debian installations, Docker often crashes because it expects legacy firewall components that aren't present or configured. You can test if forcing Docker to bypass `iptables` allows it to start:
> 1. Create or edit the configuration file:
> ```bash
> nano /etc/docker/daemon.json
> ```
> 2. Paste the following JSON block:
> ```bash
> {
>   "iptables": false
> }
> ```
> 3. Reset and Restart the Daemon
> ```bash
> # Reset the failed state tracking
> systemctl reset-failed docker.service
>
> # Restart containerd first, then docker
> systemctl restart containerd
> systemctl restart docker
> ```
> 4. Check the status now:
> ```bash
> root@minimal:/home/skelios/elera_pos_flutter# systemctl status docker
> ● docker.service - Docker Application Container Engine
>      Loaded: loaded (/usr/lib/systemd/system/docker.service; enabled; preset: enabled)
>      Active: active (running) since Thu 2026-08-27 10:00:15 JST; 2s ago
>  Invocation: 8a7f99a2be12406eb97a435be8bf257d
> TriggeredBy: ● docker.socket
>        Docs: https://docs.docker.com
>    Main PID: 4933 (dockerd)
>       Tasks: 9
>      Memory: 26.8M (peak: 29.4M)
>         CPU: 2.179s
>      CGroup: /system.slice/docker.service
>              └─4933 /usr/bin/dockerd -H fd:// --containerd=/run/containerd/containerd.sock
>
> Aug 27 10:00:13 minimal dockerd[4933]: time="2026-08-27T10:00:13.080868875+09:00" level=warning msg="Could not load necessary modules for IPSEC rules: protocol not su>
> Aug 27 10:00:13 minimal dockerd[4933]: time="2026-08-27T10:00:13.110019250+09:00" level=warning msg="configuring DOCKER-USER" error="failed to create DOCKER-USER ipv6>
> Aug 27 10:00:13 minimal dockerd[4933]: time="2026-08-27T10:00:13.242008375+09:00" level=info msg="Loading containers: done."
> Aug 27 10:00:13 minimal dockerd[4933]: time="2026-08-27T10:00:13.515128450+09:00" level=info msg="Docker daemon" commit=6a43e3d containerd-snapshotter=false storage-d>
> Aug 27 10:00:13 minimal dockerd[4933]: time="2026-08-27T10:00:13.515635900+09:00" level=info msg="Initializing buildkit"
> Aug 27 10:00:13 minimal dockerd[4933]: time="2026-08-27T10:00:13.516766200+09:00" level=warning msg="failed check for fsverity support" error="enable fsverity failed:>
> Aug 27 10:00:15 minimal dockerd[4933]: time="2026-08-27T10:00:15.250405350+09:00" level=info msg="Completed buildkit initialization"
> Aug 27 10:00:15 minimal dockerd[4933]: time="2026-08-27T10:00:15.300272350+09:00" level=info msg="Daemon has completed initialization"
> Aug 27 10:00:15 minimal dockerd[4933]: time="2026-08-27T10:00:15.300568750+09:00" level=info msg="API listen on /run/docker.sock"
> Aug 27 10:00:15 minimal systemd[1]: Started docker.service - Docker Application Container Engine.
> ```

