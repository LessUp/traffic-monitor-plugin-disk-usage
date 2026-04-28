# Disk Usage Plugin for TrafficMonitor

A [TrafficMonitor](https://github.com/zhongyang219/TrafficMonitor) plugin that displays the disk usage percentage of Windows drives. You can choose to show the **total** usage across all fixed drives, or monitor a **specific drive** (e.g. C:, D:).

The usage value is refreshed every **60 seconds**.

---

## Features

- Shows disk usage percentage as a label + value in the TrafficMonitor taskbar / main window.
- Draws a resource-usage graph bar in the taskbar (same style as CPU/RAM).
- Options dialog lets you pick **Total** (aggregate of all fixed drives) or any individual drive letter.
- Configuration is saved to `DiskUsage.ini` inside TrafficMonitor's config directory.
- Tooltip shows per-drive breakdown when "Total" mode is selected.

---

## Repository Layout

```
traffic-monitor-plugin-disk-usage/
├── include/
│   └── PluginInterface.h       ← TrafficMonitor plugin API header
├── DiskUsage/
│   ├── framework.h             ← Windows / MFC includes
│   ├── pch.h / pch.cpp         ← Precompiled header
│   ├── resource.h              ← Resource ID definitions
│   ├── DiskUsage.rc            ← String table + dialog resources (EN + ZH)
│   ├── DataManager.h/.cpp      ← Settings, config file I/O, DPI helpers
│   ├── DiskUsageItem.h/.cpp    ← IPluginItem implementation
│   ├── DiskUsage.h/.cpp        ← ITMPlugin implementation + DllExport
│   ├── OptionsDlg.h/.cpp       ← Options dialog (MFC CDialog)
│   ├── DiskUsage.vcxproj       ← MSVC project (Win32 + x64, Debug + Release)
│   └── DiskUsage.vcxproj.filters
└── DiskUsage.sln               ← Visual Studio solution
```

---

## Build Requirements

| Requirement | Version |
|---|---|
| Visual Studio | 2022 (v143 toolset) or 2019 (v142) |
| Windows SDK | 10.0 |
| MFC | Dynamic MFC (included in VS "Desktop development with C++" workload) |

> **Note:** The project uses **Dynamic MFC** (`/MD`). Make sure the corresponding Visual C++ Redistributable is installed on the target machine, or switch to Static MFC in the project properties.

### Steps

1. Open `DiskUsage.sln` in Visual Studio.
2. Select the desired configuration (e.g. **Release | x64** for a 64-bit TrafficMonitor).
3. Build → the output DLL will be placed in `bin\x64\Release\DiskUsage.dll`.

---

## Installation

1. Locate your TrafficMonitor installation directory (where `TrafficMonitor.exe` lives).
2. Create (or open) the `plugins` subdirectory.
3. Copy `DiskUsage.dll` into `plugins\`.
4. (Re)start TrafficMonitor.
5. Go to **Options → General → Plugin Management** – "Disk Usage" should appear in the list.
6. Right-click the taskbar window → **Display Settings** → tick **Disk Usage** to show it.
7. To change which disk is monitored, click **Options** next to the plugin in Plugin Management.

---

## Configuration

Settings are stored in `<TrafficMonitor config dir>\DiskUsage.ini`:

```ini
[Settings]
SelectedDisk=0   ; 0 = Total, 1 = first fixed drive, 2 = second fixed drive, …
```

---

## License

MIT – see [LICENSE](LICENSE) (if present).

---

## Acknowledgements

- Plugin interface based on [TrafficMonitorPlugins](https://github.com/zhongyang219/TrafficMonitorPlugins) template.
- Main application: [TrafficMonitor](https://github.com/zhongyang219/TrafficMonitor) by zhongyang219.