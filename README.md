# react-cli

### Requirements
- cmake standard >= 3.25

### Install
```bash
clone repo
cd react-cli
cmake -S ./ -B build/
cd build
make
sudo make install
```

### Commands
- `react-cli generate component path/to/MyComponent`
- `react-cli add template path/to/MyExistingFile`
- `react-cli remove template MyTemplate`
- `react-cli list template`

note: removing or creating template files requires superuser permissions due to the modification of the installation directory

### Custom template directory:
```bash
/usr/local/share/react-cli/assets/templates/custom/
```
