type t;

type option = {interval: int};

[@bs.module "fs"] external exists: string => bool = "existsSync";

[@bs.module "fs"]
external watchFile: (string, option, ('a, 'a) => unit) => t = "watchFile";

[@bs.send] external on: (t, string, unit => unit) => unit = "on";

[@bs.send] external off: (t, string, unit => unit) => unit = "off";
