open Ws;
open Yargs;

exception NotFound(string);

type argv = {
  port: int,
  file: string,
};
type option = {port: int};

let {port, file} =
  yargs
  ->usage("Usage: $0 <path-to-file> [options]")
  ->alias("f", "file")
  ->nargs("f", 1)
  ->option(
      "port",
      {
        description: "Set the port of the WebSocket server - Default is 9999",
        alias: "p",
        type_: "number",
      },
    )
  ->demandOption([|"f"|])
  ->default({port: 9999})
  ->argv;

let fileToWatch =
  switch (file) {
  | "" => raise(NotFound("You must pass a path to the file to watch"))
  | path => Path.resolve(path)
  };

let fileWatcher = Fs.watchFile(fileToWatch, {interval: 300}, (_, _) => ());

let wss = make({port: port});

wss->on("connection", ws => {
  fileWatcher->Fs.on("change", () => ws->send("update"))
});
