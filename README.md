<p align="center">
<a href="#" target="_blank"><img src="https://user-images.githubusercontent.com/59381835/216808462-0edf903c-b3d3-451b-a3fb-089b0ee31f82.png" width="90px" height="auto"/></a>
</p>

<h1 align="center">
  Lazap
</h1>

<p align="center">
  A lightweight cross-platform software <br>
  Unifying clients into a singular, modern library
</p>

<p align="center">
  <a href="https://github.com/Lazap-Development/lazap/releases">
     <img src="https://img.shields.io/github/downloads/Lazap-Development/lazap/total.svg?style=for-the-badge&color=ffffff&logo=docusign&logoColor=white" />
  </a>
  <a href="https://github.com/Lazap-Development/lazap/releases">
      <img src="https://img.shields.io/github/v/release/Lazap-Development/Lazap?style=for-the-badge&logo=Github&color=Green">
  </a>
 </p>

> [!WARNING]  
> Lazap is currently undergoing a **major rewrite in C++** to improve performance, stability, and functionality.
> The rewrite is being developed in the [`cpp`](https://github.com/Lazap-Development/lazap/tree/cpp) branch. There's currently a PR open for more details: https://github.com/Lazap-Development/Lazap/pull/162

<div align="center">
  <img src="https://github.com/user-attachments/assets/ab65fee8-5d8e-4389-8427-84c2829813f3">
</div>

<br>

**The new face of Open Source Game Launchers**, Lazap, unites all your games together in one place! Whether it's on Windows, Linux or MacOS, we got you covered. Lazap connects with other proprietary game launchers such as Riot Games, Epic Games Launcher and Steam, and collects all of those games and deposits them inside of one place. Lazap is developed to be deeply customizable, meets up to the user's expectance, possess a glancing & modernist look, and isn't expensive at all on hardware consumption. 

## ✨ Key Features
- Remarkably lightweight and efficient on hardware resources
- Compatibility across Windows, Linux, and MacOS platforms
- Support for clients such as Steam, Epic, Ubisoft, R*, Lutris, etc.
- Flexibility to add and personalize user-set custom games
- Modern and Sleek user interface with theming customization
- Spotify Integration with streamlined music control
- `[Beta]` Cross-Platform Lazap Overlay
- `[WIP]` System monitor, overclock, and benchmarking tools
  
## ‍💻 Installation

Depending on your operating system, we try to provide as much ease in terms of installation:<br>

▸ **Windows**: 
- Download and install the latest `.msi` file in the [releases](https://github.com/Lazap-Development/lazap/releases) section.
- Or install via Winget: 
```
winget install LazapDevelopment.Lazap
```

▸ **Linux**: 
- Download and install one of `.deb`, `.tar.zst`, or `.rpm` file in the [releases](https://github.com/Lazap-Development/lazap/releases) section.
- Arch Linux users can also directly install from the maintained `lazap-bin` [AUR](https://aur.archlinux.org/packages/lazap-bin) (Thanks to [@begin-theadventure](https://www.github.com/begin-theadventure)).
  
▸ **MacOS**: 
> [!NOTE] 
> Most modern MacOS systems are of `aarch64` (also known as ARM) instead of `x64` architecture. <br>
> Execute the `arch` command in a new terminal to confirm your MacOS architecture.
- Download and install the latest `[x64/aarch64].dmg` file in the [releases](https://github.com/Lazap-Development/lazap/releases) section.

## 👾 Development

Lazap is fully open-source and can be compiled from source.

### Prerequisites
- [Rust](https://www.rust-lang.org/tools/install)
- [C++ Build Tools](https://visualstudio.microsoft.com/) (It should already come with Rust installation)
- [Latest Node.JS](https://nodejs.org/en)
- [Yet Another Resource Negotiator](https://yarnpkg.com/getting-started)

> [!NOTE] 
> Ensure you have **corepack** enabled in your system by running `corepack enable` as admin or root.

Once you have the above preqs, you may continue with the following:

```bash
# clone this repo 
git clone https://github.com/Lazap-Development/Lazap.git 
# change current dir, install deps
cd Lazap && yarn install
# run in development mode
yarn tauri dev
```

## 📚 Help & Support
<details>
  <summary>Have a question/problem?</summary>
  ▸ If you need assistance or technical support for your Lazap installation, feel free to file an issue so we can assist you.
</details>

<details>
  <summary>Want to help out instead of getting help?</summary>
  ▸ Open a Pull Request and fix some gears! We would love the community's help to improve Lazap and make it more user-friendly.
</details>

## ⁉️ FAQ

<details>
  <summary>What are the next steps of this project?</summary>

  **Current Priority:**
  - Support as many launchers as possible.
  - Optimize the software to its fullest potential.

  **Future Possibility:**
  - Consider transforming the launcher into its own store (similar to Steam).

</details>


## 🛂 License
Lazap is licensed under the terms of [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International](https://github.com/DashCruft-Nation/lazap/blob/main/LICENSE.md) ("CC-BY-NC-SA-4.0"). Commercial use is not allowed under this license. This includes any kind of revenue made with or based upon the software, even donations.

The CC-BY-NC-SA-4.0 allows you to:
- [x] **Share** -- copy and redistribute the material in any medium or format
- [x] **Adapt** -- remix, transform, and build upon the material

Under the following terms:
- **Attribution** — You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
- **NonCommercial** — You may not use the material for commercial purposes. 
- **ShareAlike** — If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.

More information can be found [here](https://creativecommons.org/licenses/by-nc-sa/4.0/).
