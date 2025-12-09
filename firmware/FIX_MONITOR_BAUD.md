# Permanent Fix for 9600 Baud Issue

If PlatformIO Serial Monitor keeps defaulting to 9600 instead of 115200, follow these steps:

## Step 1: Close VS Code Completely
1. Close all VS Code windows
2. Make sure PlatformIO processes are closed (check Task Manager)

## Step 2: Clear PlatformIO Cache (Windows)
Open PowerShell in the `firmware/` directory and run:
```powershell
Remove-Item -Recurse -Force "$env:USERPROFILE\.platformio\packages\tool-pio\*" -ErrorAction SilentlyContinue
```

Or manually delete: `C:\Users\YourUsername\.platformio\packages\tool-pio\`

## Step 3: Verify Configuration Files

### Check `firmware/platformio.ini`:
- Must have `monitor_speed = 115200` in `[env:esp32dev]` section
- Must have `default_envs = esp32dev` in `[platformio]` section

### Check `.vscode/settings.json` (in repo root):
- Must have `"platformio-ide.monitorBaudRate": 115200`

## Step 4: Reopen VS Code
1. Open VS Code
2. Open the `manoeuvre/` folder (repo root)
3. Wait for PlatformIO to initialize

## Step 5: Use Command Line Monitor (Most Reliable)
Instead of using the UI button, always use:
```bash
cd firmware
pio device monitor -b 115200 -e esp32dev
```

Or use the helper script:
```bash
cd firmware
.\monitor.bat
```

## Why This Happens
VS Code PlatformIO extension sometimes:
- Caches old monitor settings
- Doesn't reload platformio.ini immediately
- Uses default 9600 if config isn't read properly

## Permanent Solution
Always use the command line method (`pio device monitor -b 115200`) or the `monitor.bat` script. This bypasses the VS Code extension's caching issues.

