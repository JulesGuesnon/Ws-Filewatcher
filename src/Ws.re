type t;

type payload = {port: int};

[@bs.new] [@bs.module "ws"] external make: payload => t = "Server";

[@bs.send] external on: (t, string, 'a => unit) => unit = "on";

[@bs.send] external send: (t, 'a) => unit = "send";
