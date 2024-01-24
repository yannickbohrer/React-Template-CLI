# rtc

### Requirements
- cmake standard >= 3.25

### Installation
```bash
git clone git@github.com:yannickbohrer/react-template-cli.git
cd react-template-cli
sudo cmake -S ./ -B .build/
cd .build
sudo make
sudo make install
```

### Syntax 
`rtc [activity] [type] [path] [--flags]`

### Usage
`[]` indicate optional parameter
- `rtc generate component path/to/MyComponent [--css] [--template=CustomTemplate]`
- `rtc add template path/to/MyExistingFile`
- `rtc remove template MyTemplate`
- `rtc list template`

Flags:
- `--css`: Dedicated css file for component will be generated
- `--template=CustomTemplate`: Use custom template for component generation. Not set: Standard template will be used

### Important:
- removing or creating template files requires superuser priviliges due to modification of the installation directory
- when adding a template file, the file name (without file extension) has to match component name exactly

### Custom template directory:
```bash
/usr/local/share/rtc/assets/templates/custom/
```
