# Lazap
A native-like attempt to unify your clients with one fast, modern, and cross-platform solution. 

> [!Note]  
> Lazap has undergone a significant rewrite in C++ (even the UI) to better align with the project’s core objectives.
> As part of this refactoring, some features have been removed to streamline development and focus on the project’s primary goals.
> While these features may or may not return, it'll ensure a more stable and purpose-driven evolution of Lazap.

![16:9 Concept Art](https://github.com/user-attachments/assets/0dc7ea20-49d5-45d5-b313-86c0130e7453)  

> [!Note]  
> This is a concept art. We have attempted to reproduce it in the software (so far very successful), and work is still ongoing.

## Key Features
- Lightweight and high-performance with instant startup and minimal runtime overhead
- Cross-platform compatibility: Windows & Linux (Wayland and Xorg)
- Native-like Support for Steam, Epic, Ubisoft, and Rockstar
- Custom game support: add, organize, and manage your own custom titles
- Modern, GPU-accelerated, and intuitive user interface built for speed and simplicity
- Fully offline-capable (i.e. access and launch games without internet)

## Installation

Depending on your operating system, we try to provide as much ease in terms of installation:

▸ **Windows**: 
- Download and install the latest `.msi` file in the [releases](https://github.com/Lazap-Development/lazap/releases) section.
- Or install via WinGet (updates may lag behind GitHub releases):
```
winget install LazapDevelopment.Lazap
```

▸ **Linux**:
- Download and install one of the following from the
  [releases](https://github.com/Lazap-Development/lazap/releases) section:
  - `.deb` *(Debian/Ubuntu and derivatives like Linux Mint, Pop!_OS)*
  - `.rpm` *(Fedora, RHEL, openSUSE)*
  - `.tgz` *(generic Linux, manual install on most distributions)*
  - `AppImage` *(universal Linux, no installation required)*
- Arch Linux users can also directly install from the maintained `lazap-bin`
  [AUR](https://aur.archlinux.org/packages/lazap-bin)
  *(Arch-based distros like Manjaro, EndeavourOS)* (Thanks to [@begin-theadventure](https://github.com/begin-theadventure))
