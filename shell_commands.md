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
