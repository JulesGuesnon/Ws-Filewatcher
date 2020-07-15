open Ws;
open Yargs;

[@bs.module "child_process"] external exec: string => unit = "exec";

exception NotFound;

type argv = {
  port: int,
  file: string,
  verbose: option(bool),
  command: option(string),
};

type option = {port: int};

let {port, file, verbose, command} =
  yargs
  ->usage("Usage: $0 <path-to-file> [options]")
  ->alias("f", "file")
  ->nargs("f", 1)
  ->option(
      "port",
      {
        description: "Set the port of the WebSocket server",
        alias: "p",
        type_: "number",
      },
    )
  ->option(
      "command",
      {
        description: "Run the command on every file update",
        alias: "c",
        type_: "string",
      },
    )
  ->option(
      "verbose",
      {description: "Allow logging", alias: "v", type_: "boolean"},
    )
  ->demandOption([|"f"|])
  ->default({port: 9999})
  ->argv;

let fileToWatch =
  switch (file) {
  | "" =>
    Chalk.red("You must pass a path to the file to watch")->print_endline;
    raise(NotFound);
  | path => Path.resolve(path)
  };

let verbose =
  switch (verbose) {
  | Some(_) => true
  | None => false
  };

let fileWatcher = Fs.watchFile(fileToWatch, {interval: 300}, (_, _) => ());

let wss = make({port: port});

if (verbose) {
  Printf.sprintf(
    "> Server is watching '%s' and is starting on port '%i' \n",
    fileToWatch,
    port,
  )
  ->Chalk.green
  ->print_endline;
};

wss->on("connection", ws => {
  let fsCb = () => {
    if (verbose) {
      Chalk.green("> The file was updated -> Sending an update to socket")
      ->print_endline;
    };

    switch (command) {
    | Some(c) => exec(c)
    | None => ()
    };

    ws->send("update");
  };

  if (verbose) {
    Chalk.green("> A socket is connected")->print_endline;
  };

  fileWatcher->Fs.on("change", fsCb);

  ws->on("close", () => {
    if (verbose) {
      Chalk.green("> A socket disconnected")->print_endline;
    };

    fileWatcher->Fs.off("change", fsCb);
  });
});
