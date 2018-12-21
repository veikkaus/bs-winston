type u;

[@bs.deriving abstract]
type config = {
  [@bs.optional] level: string,
  [@bs.optional] format: Format.t,
  transports: array(Transport.t),
  [@bs.optional] silent: bool
};

[@bs.module "winston"] external createLoggerExternal: config => u = "createLogger";
[@bs.send] external logExternal: u => Js.Json.t => unit = "log";

type content = {
  level: string,
  message: string,
  error: option(exn),
  errorKey: string,
  json: option(Js.Json.t),
  jsonKey: string
};

type t = {
  logger: u,
  content: content
};

let create =
  (~level=?, ~format=?, ~transports, ~silent=?, ()) => ({
    logger: createLoggerExternal(config(~level=?level, ~format=?format, ~transports=Belt.List.toArray(transports), ~silent=?silent, ())),
    content: {
      level: "silly",
      message: "",
      error: None,
      errorKey: "error",
      json: None,
      jsonKey: "json"
    }
  });

let level =
  (obj, level) => ({
    logger: obj.logger,
    content: {...obj.content, level: level}
  });
let error = obj => level(obj, "error");
let warn = obj => level(obj, "warn");
let info = obj => level(obj, "info");
let verbose = obj => level(obj, "verbose");
let debug = obj => level(obj, "debug");
let silly = obj => level(obj, "silly");

let withMessage =
  (obj, msg) => ({
    logger: obj.logger,
    content: {...obj.content, message: Js.String.trim(obj.content.message ++ " " ++ msg)}
  });

let withExn =
  (obj, error) => ({
    logger: obj.logger,
    content: {...obj.content, error: Some(error)}
  });

let withJson =
  (obj, json) => ({
    logger: obj.logger,
    content: {...obj.content, json: Some(json)}
  });

let jsExnToJson: Js.Exn.t => Js.Json.t =
  (jsError) => {
    let e: Js.Dict.t(Js.Json.t) = Js.Dict.empty();
    Js.Exn.name(jsError) -> Belt.Option.map(name => Js.Dict.set(e, "name", Js.Json.string(name))) -> ignore;
    Js.Exn.message(jsError) -> Belt.Option.map(message => Js.Dict.set(e, "message", Js.Json.string(message))) -> ignore;
    Js.Exn.stack(jsError) -> Belt.Option.map(stack => Js.Dict.set(e, "stack", Js.Json.string(stack))) -> ignore;
    Js.Exn.fileName(jsError) -> Belt.Option.map(fileName => Js.Dict.set(e, "fileName", Js.Json.string(fileName))) -> ignore;
    Js.Json.object_(e);
  };

let exnToJson: exn => Js.Json.t =
  error => switch (Js.Exn.asJsExn(error)) {
    | Some(error) => jsExnToJson(error)
    | None => Js.Json.string(Js.String.make(error))
  };

let log =
  obj => {
    let m: Js.Dict.t(Js.Json.t) = Js.Dict.empty();
    Belt.Option.map(obj.content.json, json => Js.Dict.set(m, obj.content.jsonKey, json)) |> ignore;
    Belt.Option.map(obj.content.error, error => Js.Dict.set(m, obj.content.errorKey, exnToJson(error))) |> ignore;
    Js.Dict.set(m, "message", Js.Json.string(obj.content.message));
    Js.Dict.set(m, "level", Js.Json.string(obj.content.level));
    logExternal(obj.logger, Js.Json.object_(m));
  };
