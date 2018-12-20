type t;

type level =
  | Error
  | Warn
  | Info
  | Verbose
  | Debug
  | Silly;

let levelStr: level => string =
  lvl => switch (lvl) {
    | Error => "error"
    | Warn => "warn"
    | Info => "info"
    | Verbose => "verbose"
    | Debug => "debug"
    | Silly => "silly"
  };

module Config {

  module Transports {
    type t;

    module Console {
      [@bs.deriving abstract]
      type configJs = {
        [@bs.optional] level: string,
        [@bs.optional] silent: bool,
        [@bs.optional] eol: string,
        [@bs.optional] stderrLevels: array(string),
        [@bs.optional] consoleWarnLevels: array(string)
      };

      [@bs.new] [@bs.module "winston.transports"] external consoleExternal: configJs => t = "Console";

      let createDefault = () => consoleExternal(configJs(()));
      
      let create =
        (~level=?, ~silent=?, ~eol=?, ~stderrLevels=?, ~consoleWarnLevels=?, ()) =>
          configJs(~level=?level, ~silent=?silent, ~eol=?eol, ~stderrLevels=?stderrLevels, ~consoleWarnLevels=?consoleWarnLevels, ())
          -> consoleExternal;
    };
  };

  module Format {
    type t;

    [@bs.val] [@bs.module "winston.format"] external combineExternal: t => t => t = "combine";

  };

  [@bs.deriving abstract]
  type t = {
    [@bs.optional] level: string,
    [@bs.optional] format: Format.t,
    transports: array(Transports.t),
    [@bs.optional] silent: bool
  };

  module Builder {
    let create: Transports.t => t =
      transport =>
        t(~transports=[|transport|], ());
    
    let setLevel: t => level => t =
      (config, lvl) =>
        t(~level=levelStr(lvl), ~format=?formatGet(config), ~transports=transportsGet(config), ~silent=?silentGet(config), ());

    let setSilent: t => bool => t =
      (config, silent) =>
        t(~level=?levelGet(config), ~format=?formatGet(config), ~transports=transportsGet(config), ~silent=silent, ());

    let addFormat: t => Format.t => t =
      (config, fmt) => {
        let fmt = switch (formatGet(config)) {
          | Some(existing) => Format.combineExternal(existing, fmt)
          | None => fmt
          };
        t(~level=?levelGet(config), ~format=fmt, ~transports=transportsGet(config), ~silent=?silentGet(config), ());
      };

    let addTransport: t => Transports.t => t =
      (config, transport) =>
        t(~level=?levelGet(config), ~format=?formatGet(config), ~transports=Belt.Array.concat(transportsGet(config), [|transport|]), ~silent=?silentGet(config), ());
  };

};

[@bs.val] [@bs.module "winston"] external createLoggerExternal: Config.t => t = "createLogger";

[@bs.send] external logExternal: t => Js.Json.t => unit = "log";

let createLogger: Config.t => t =
  config =>
    createLoggerExternal(config);

let log: t => Js.Json.t => unit =
  (logger, content) => switch (Js.Json.classify(content)) {
    | Js.Json.JSONObject(obj) => {
      let keys = obj -> Js.Dict.keys;
      let copy = obj -> Js.Dict.entries -> Js.Dict.fromArray;
      if (!Js.Array.includes("level", keys)) Js.Dict.set(copy, "level", Info -> levelStr -> Js.Json.string);
      if (Js.Array.includes("message", keys)) Js.Dict.set(copy, "message", Js.Json.string(""));
      logExternal(logger, Js.Json.object_(copy));
    }
    | _ => invalid_arg("content")
  };


let logJson: t => level => Js.Json.t => unit =
  (logger, lvl, msg) => {
    let obj = Js.Dict.empty();
    Js.Dict.set(obj, "message", msg);
    Js.Dict.set(obj, "level", lvl -> levelStr -> Js.Json.string);
    log(logger, Js.Json.object_(obj));
  };

let logError: t => level => string => exn => unit =
  (logger, lvl, msg, error) => switch (error) {
    | Js.Exn.Error(error) => {
      let obj = Js.Dict.empty();
      Js.Exn.name(error) -> Belt.Option.map(name => Js.Dict.set(obj, "name", Js.Json.string(name))) -> ignore;
      Js.Exn.stack(error) -> Belt.Option.map(stack => Js.Dict.set(obj, "stack", Js.Json.string(stack))) -> ignore;
      Js.Exn.fileName(error) -> Belt.Option.map(fileName => Js.Dict.set(obj, "fileName", Js.Json.string(fileName))) -> ignore;
      Js.Dict.set(obj, "message", Js.Json.string(msg ++ Belt.Option.getWithDefault(Js.Exn.message(error), "")));
      Js.Dict.set(obj, "level", lvl -> levelStr -> Js.Json.string);
      log(logger, Js.Json.object_(obj));
    }
    | _ => logJson(logger, lvl, Js.Json.string(msg ++ Js.String.make(error)))
  };

let logLevel =
  (logger, lvl, ~message=?, ~json=?, ~error=?, ()) => switch (error) {
    | Some(err) => logError(logger, lvl, Belt.Option.getWithDefault(message, ""), err)
    | None => switch (json) {
      | Some(json) => logJson(logger, lvl, json)
      | None => logJson(logger, lvl, Belt.Option.getWithDefault(message, "") -> Js.Json.string)
    }
  };

let error = (logger, ~message=?, ~json=?, ~error=?, ()) => logLevel(logger, Error, ~message=?message, ~json=?json, ~error=?error, ());
let errorMsg = (logger, message) => error(logger, ~message=message, ());
let errorExn = (logger, message, e) => error(logger, ~message=message, ~error=e, ());

let warn = (logger, ~message=?, ~json=?, ~error=?, ()) => logLevel(logger, Warn, ~message=?message, ~json=?json, ~error=?error, ());
let warnMsg = (logger, message) => warn(logger, ~message=message, ());
let warnExn = (logger, message, e) => warn(logger, ~message=message, ~error=e, ());

let info = (logger, ~message=?, ~json=?, ()) => logLevel(logger, Info, ~message=?message, ~json=?json, ());
let infoMsg = (logger, message) => info(logger, ~message=message, ());

let debug = (logger, ~message=?, ~json=?, ()) => logLevel(logger, Debug, ~message=?message, ~json=?json, ());
let debugMsg = (logger, message) => debug(logger, ~message=message, ());
