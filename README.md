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

### Syntax 
`react-cli [activity] [type] [path] [--flags]`

### Functionalities
`[]` indicate optional parameter
- `react-cli generate component path/to/MyComponent [--css] [--template=CustomTemplate]`
- `react-cli add template path/to/MyExistingFile`
- `react-cli remove template MyTemplate`
- `react-cli list template`

Flags:
- `--css`: Dedicated css file for component will be generated
- `--template=CustomTemplate`: Use custom template for component generation

### Important:
- removing or creating template files requires superuser priviliges due to modification of the installation directory
- when adding a template file, the file name (without file extension) has to match component name exactly

### Custom template directory:
```bash
/usr/local/share/react-cli/assets/templates/custom/
```
