type t;

[@bs.module "winston.format"] [@bs.variadic] external combineExternal: (. array(t)) => t = "combine";

let combine =
  formats => combineExternal(. Belt.List.toArray(formats));
