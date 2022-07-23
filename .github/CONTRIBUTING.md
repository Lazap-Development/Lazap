<p align="center">
<a href="#" target="_blank"><img src="https://media.discordapp.net/attachments/910422768045133869/914878042508251156/icon.png" width="160px" height="auto"/></a>
</p>

<h1 align="center">
  Lazap Launcher
</h1>

<p align="center">
  Lazap, a cross-platform Games Client/Launcher <br>
  All your games at one library with a modernish look and experience.
</p>

<p align="center">
  <a href="https://github.com/Lazap-Development/lazap/releases">
     <img src="https://img.shields.io/github/downloads/Lazap-Development/lazap/total.svg?style=for-the-badge&color=ffffff&logo=windows" />
  </a>⠀
  <a href="https://dashcruft.com/discord">
      <img src="https://img.shields.io/discord/836790685784211486?logo=discord&label=Discord&style=for-the-badge&color=228B22">
  </a>
 </p>

<br>

# Welcome to Lazap Development Guide! 🎉

Here, you will be able to see and understand how you should contribute to Lazap and help us create an amazing gane launcher.

Make sure to follow our [Code of Conduct](https://github.com/Lazap-Development/lazap/blob/main/.github/CODE_OF_CONDUCT.md) to keep the community approachable and respectable.

This guide will give you a brief overview on how you can contribute to Lazap, from building and testing, creating an issue, opening a PR, and merging the PR. 🎊


## Quick Contribution Overview

You will need to fulfil some requirements in order to fully build, test and contribute changes. All of them are listed below:

* [NodeJS](https://nodejs.org) (>=17.X.X)
* [NPM](https://npmjs.com) or [yarn](https://yarnpkg.com) or any other NodeJS package manager.
* [Git](https://git-scm.com) or [Github Desktop](https://desktop.github.com) or any other GitHub interactive app/command-based-system.

Once you have them, it is time to get started on the programming side of things. Here is a list of all the things mentioned:
* [Getting Started](#getting-started)
* Building Lazap
	* [Windows](#building-windows)
	* [Linux](#building-linux)

After developing, bug fixing and a few cups of coffee later when you are ready to create a Pull Request make sure to read the [Guidelines](#pr-guidelines) before making a Pull Request!
<!-- Add images here later -->

- - - -

### Getting Started<a name="getting-started">

First of all, fork the repository so that you can push your changes to some place. Here is how to do it:

![image](https://user-images.githubusercontent.com/77503777/179051993-d397984c-0734-4563-ba51-1f936585ca84.png)

![image](https://user-images.githubusercontent.com/77503777/179053134-451dd11c-f774-44d4-b2fd-4f122eb8966f.png)

Click on "Create Fork", without changing anything, to make a fork which can be found at `https://github.com/YOURUSERNAME/lazap`

- - - -

To initialize a working local git repository, clone the repository with this:
```bash
git clone https://github.com/YOURUSERNAME/lazap.git lazap
```
To get started with a working environment install dependencies with this:
```bash
npm install
```
To get the app started run this:
```bash
electron .
```
To build the app for testing run this:
```bash
electron-builder --dir
```
- - - -

### Building(Windows)<a name="building-windows">

To build Lazap for Windows, make sure you install all dependencies and you need to have to be on Windows. After that run this command:
```bash
electron-builder build
```

### Building(Linux)<a name="building-linux">

To build Lazap for Linux, make sure you install all dependencies and you need to have to be on Linux. After that run this command:
```bash
electron-builder build
```

<br>

# Pull Request Guidelines<a name="pr-guidelines">

### Code guidelines

Please go through these guidelines before submitting the PR.
1. The changes must be reasonable and should not contain only syntax changes or similar. For example: The Pull Request #41
2. The changes must be in some what working condition before it is made fully functional and merged. Preferrably it should work perfectly with no issues.
3. The changes must be practical and cannot not have a temporary fix for only specific conditions. For example: The Pull Request #32.
4. The changes must not be working with only deprecated version(s) of NodeJS, dependencies, or any other external dependencies.
5. The version of Lazap must follow [SemVer](https://semver.org) versioning.

### Merge guidelines
1. The PR must have a good and understandable title.
	1. The title must start with capital letter. It should also have capital letters at appropriate places.
	2. There should be no grammatical mistakes in the title.
	3. If there are any mistakes edit it out immediately.
2. The PR must follow the template provided below.
	1. If not followed, the Pull Request will be closed.
3. The PR must have a description with all the information labelled with required.
	1. Any missing information can create delay in merge or may get straight up closed.
4. The PR must not have the base set as "main" branch.
	1. Only under special circumstances will this be allowed.
	2. The PR will instantly be closed, otherwise.

*Pull Request is abbreviated as "PR"
