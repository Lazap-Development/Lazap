# Tauri Lazap

Before starting, please make sure you have the [prerequisites](https://tauri.app/v1/guides/getting-started/prerequisites) or you won't be able to compile this all by yourself!

## Project setup
```
yarn install
```

Keep in mind that if you're using **Windows** and you're getting an execution error for yarn, you might need to do this if in a powershell window that is ran as admin!

`Set-ExecutionPolicy RemoteSigned`

In case you're getting an error related to incompatible node engine when installing packages, do the following:

`yarn config set ignore-engines true`

### Compiles and hot-reloads for development
```
yarn tauri:serve
```

### Compiles and minifies for production
```
yarn build
```

### Lints and fixes files
```
yarn lint
```
