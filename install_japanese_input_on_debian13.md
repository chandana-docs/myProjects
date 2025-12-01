**Chandana**
**- 2025-12-01**

# How to Install Japanese (Mozc) Input on Debian 13 (Trixie)
This document consists of the steps to install Japanese (Mozc) input on Debian 13 "Trixie".

# Steps
1. Install `ibus-mozc`: Open a terminal and install the `ibus-mozc` package using the following command:
```bash
sudo apt install ibus-mozc
```

2. Open `ibus-setup` GUI: Run the IBus configuration tool from the terminal **without** `sudo`:
```bash
ibus-setup
```

* Enter `Yes` if prompted to enable the `ibus-daemon`.

3. **Add Japanese (Mozc) Input Method:**
* Navigate to `Input Method`
* Click `Add`
* Select **Japanese**, then choose **Mozc** from the list.
* Click `Add`
* Click `Close` to exit the preferences window.
  
7. **Re-login**: `Logout` and `Login` from your system to ensure the changes take effect.

> **Note**: This adds the **Japanese (Mozc)** input method. The default key combination to switch between input methods is often `Super + Space` (`Windows Key + Space`).
