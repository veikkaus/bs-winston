type t;

module Console {

  [@bs.deriving abstract]
  type config = {
    [@bs.optional] level: string,
    [@bs.optional] silent: bool,
    [@bs.optional] eol: string,
    [@bs.optional] stderrLevels: array(string),
    [@bs.optional] consoleWarnLevels: array(string)
  };

  [@bs.new] [@bs.module "winston.transports"] external consoleExternal: config => t = "Console";

  let create =
    (~level=?, ~silent=?, ~eol=?, ~stderrLevels=?, ~consoleWarnLevels=?, ()) =>
      consoleExternal(config(
        ~level =? level,
        ~silent =? silent,
        ~eol =? eol,
        ~stderrLevels =? stderrLevels -> Belt.Option.map(Belt.List.toArray),
        ~consoleWarnLevels =? consoleWarnLevels -> Belt.Option.map(Belt.List.toArray),
        ()
      ));

};
