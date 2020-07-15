# Ws-Filewatcher

Ws-Filewatcher is a cli that will watch for file change and open a websocket server where it will send a message everytime the file is updated

## :zap: Requirements

-   [Npm](https://www.npmjs.com/) or [Yarn](https://yarnpkg.com/), the package managers for NodeJs

## :wrench: Installation

Here are the instruction to install the cli globally, but you can do it locally on your projects.

```bash
npm i -g ws-filewatcher
OR
yarn global add ws-filewatcher
```

## :fire: Usage

Here is the minimum you need to write to launch the service:

```bash
ws-filewatcher -f path/to/my/file
```

**:warning: 'path/to/my/file' isn't necessarily an existing file. You may want to listen for a file creation. So if you make a typo in the path, it won't be considered as an error**

### Options

|               | Verbose         |
| ------------- | --------------- |
| parameter     | `--verbose`     |
| shortcut      | `-v`            |
| default value | _(bool)_`false` |
| description   | Allow loggings  |

|               | Port                                 |
| ------------- | ------------------------------------ |
| parameter     | `--port`                             |
| shortcut      | `-p`                                 |
| default value | _(number)_`9999`                     |
| description   | Set the port of the WebSocket server |

|               | Command                                      |
| ------------- | -------------------------------------------- |
| parameter     | `--command`                                  |
| shortcut      | `-c`                                         |
| default value | _(string)_`""`                               |
| description   | The command will be run on every file update |

### Full example

```bash
ws-filewatcher -f path/to/my/file -p 8080 -v -c 'cp folder/file other/folder/file'
```

### Client example

_The example assume you are running the code inside a browser_

```javascript
const ws = new WebSocket('ws://localhost:9999')

ws.onmessage = () => {
    console.log('My file was updated!')
}
```
