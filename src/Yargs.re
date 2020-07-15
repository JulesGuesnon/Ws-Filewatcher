type t;

type positional('a) = {
  describe: string,
  default: 'a,
};

type yargsOption = {
  description: string,
  alias: string,
  type_: string,
};

[@bs.module] external yargs: t = "yargs";

[@bs.send] external command: (t, string, string) => t = "command";

[@bs.send] external option: (t, string, yargsOption) => t = "option";

[@bs.send] external usage: (t, string) => t = "usage";

[@bs.send] external alias: (t, string, string) => t = "alias";

[@bs.send] external nargs: (t, string, int) => t = "nargs";

[@bs.send] external default: (t, 'a) => t = "default";

[@bs.send] external demandOption: (t, array(string)) => t = "demandOption";

[@bs.get] external argv: t => 'a = "argv";
