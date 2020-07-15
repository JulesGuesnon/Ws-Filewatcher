open Ws;
open Yargs;

exception NotFound;

type argv = {
  port: int,
  file: string,
  verbose: option(bool),
};

type option = {port: int};

let {port, file, verbose} =
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
  if (verbose) {
    Chalk.green("> A socket is connected")->print_endline;
  };

  fileWatcher->Fs.on("change", () => {
    if (verbose) {
      Chalk.green("> The file was updated -> Sending an update to socket")
      ->print_endline;
    };

    ws->send("update");
  });

  ws->on("close", () =>
    if (verbose) {
      Chalk.green("> A socket disconnected")->print_endline;
    }
  );
});
