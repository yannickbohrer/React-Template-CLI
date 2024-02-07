# react-template-cli (rtc)

rtc is a template driven cli tool for react development.\
Generate react components using generic out of the box templates, or generate reusable templates from your existing component files!

### Requirements
- cmake standard >= 3.25

### Installation Linux / MacOS
```bash
git clone git@github.com:yannickbohrer/react-template-cli.git
cd react-template-cli
sudo cmake -S ./ -B .build/
cd .build
make
sudo make install
```

### Syntax 
`rtc [activity] [type] [path] [--flags]`

### Usage
`[]` indicate optional parameter
- `rtc generate component path/to/MyComponent [--test] [--ts] [--css] [--template=CustomTemplate]`
- `rtc add template path/to/MyExistingFile`
- `rtc remove template MyTemplate`
- `rtc list template`
- `rtc history`

Flags:
- `--css`: Dedicated css file for component will be generated at component location
- `--template=CustomTemplate`: Use custom template for component generation. Not set: Standard template will be used
- `--ts`: create .tsx component instead of (standard) .jsx. Test file (if generated) will adjust as well
- `--test`: Generate test file along with component. Will use or create (if not existent) test dir at component location 

### Important:
- when adding a template file, the file name (without file extension) has to match component name exactly

#### Assets Directory:
```bash
~/.rtc/
```
