type t;

module Console = {
  [@deriving abstract]
  type config = {
    [@mel.optional]
    level: option(string),
    [@mel.optional]
    silent: option(bool),
    [@mel.optional]
    eol: option(string),
    [@mel.optional]
    stderrLevels: option(array(string)),
    [@mel.optional]
    consoleWarnLevels: option(array(string)),
  };
  [@mel.new] [@mel.module "winston"] [@mel.scope "transports"]
  external newConsoleTransport: config => t = "Console";

  let create =
      (~level=?, ~silent=?, ~eol=?, ~stderrLevels=?, ~consoleWarnLevels=?, ()) =>
    newConsoleTransport(
      config(
        ~level?,
        ~silent?,
        ~eol?,
        ~stderrLevels=?stderrLevels->Belt.Option.map(Belt.List.toArray),
        ~consoleWarnLevels=?
          consoleWarnLevels->Belt.Option.map(Belt.List.toArray),
        (),
      ),
    );
};
let createConsole = Console.create;

module File = {
  [@deriving abstract]
  type config('a) = {
    [@mel.optional]
    level: option(string),
    [@mel.optional]
    silent: option(bool),
    [@mel.optional]
    eol: option(string),
    [@mel.optional]
    filename: option(string),
    [@mel.optional]
    maxsize: option(int),
    [@mel.optional]
    maxFiles: option(int),
    [@mel.optional]
    tailable: option(bool),
    [@mel.optional]
    maxRetries: option(int),
    [@mel.optional]
    zippedArchive: option(bool),
    [@mel.optional]
    options: option(Js.t('a)),
  };
  [@mel.new] [@mel.module "winston"] [@mel.scope "transports"]
  external newFileTransport: config('a) => t = "File";

  let create =
      (
        ~level=?,
        ~silent=?,
        ~eol=?,
        ~filename=?,
        ~maxsize=?,
        ~maxFiles=?,
        ~tailable=?,
        ~maxRetries=?,
        ~zippedArchive=?,
        ~options=?,
        (),
      ) =>
    newFileTransport(
      config(
        ~level?,
        ~silent?,
        ~eol?,
        ~filename?,
        ~maxsize?,
        ~maxFiles?,
        ~tailable?,
        ~maxRetries?,
        ~zippedArchive?,
        ~options?,
        (),
      ),
    );
};
let createFile = File.create;

module Http = {
  [@deriving abstract]
  type config('a) = {
    [@mel.optional]
    host: option(string),
    [@mel.optional]
    port: option(int),
    [@mel.optional]
    path: option(string),
    [@mel.optional]
    auth: option(Js.t('a)),
    [@mel.optional]
    ssl: option(bool),
  };
  [@mel.new] [@mel.module "winston"] [@mel.scope "transports"]
  external newHttpTransport: config('a) => t = "Http";

  let create = (~host=?, ~port=?, ~path=?, ~auth=?, ~ssl=?, ()) =>
    newHttpTransport(config(~host?, ~port?, ~path?, ~auth?, ~ssl?, ()));
};
let createHttp = Http.create;

module Stream = {
  [@deriving abstract]
  type config('a) = {
    [@mel.optional]
    level: option(string),
    [@mel.optional]
    silent: option(bool),
    [@mel.optional]
    eol: option(string),
    [@mel.optional]
    stream: option('a),
  };
  [@mel.new] [@mel.module "winston"] [@mel.scope "transports"]
  external newStreamTransport: config('a) => t = "Stream";

  let create = (~level=?, ~silent=?, ~eol=?, ~stream=?, ()) =>
    newStreamTransport(config(~level?, ~silent?, ~eol?, ~stream?, ()));
};
let createStream = Stream.create;
