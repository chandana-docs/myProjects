# Shell Commands

### To tell apt to ignore an HTTPS peer verification error
```bash
sudo apt -o "Acquire::https::Verify-Peer=false" update
sudo apt -o "Acquire::https::Verify-Peer=false" install <package_name>
```
