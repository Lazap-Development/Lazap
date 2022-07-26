# Tauri Lazap

#### Before starting, please make sure you have the [prerequisites](https://tauri.app/v1/guides/getting-started/prerequisites) 

### Project setup
```
yarn install
```

- Keep in mind that if you're using **Windows** and you're getting an execution error for yarn, you might need to do this in a powershell window that is ran as admin!

`Set-ExecutionPolicy RemoteSigned`

- In case you're getting an error related to incompatible node engine when installing packages, do the following:

`yarn config set ignore-engines true`

### Compiles and hot-reloads for development
```
yarn tauri:serve
```
⚠️ **WARNING**: This also installs and checks cargo packages every time ran, Compilation may take long depending on your processor **ONLY** for the first time! After the first time, compilation should not take long anymore. 


### Compiles and minifies for production
```
yarn tauri:build
```

### Lints and fixes files
```
yarn lint
```
