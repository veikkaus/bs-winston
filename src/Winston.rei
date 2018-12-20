type t;

/* npm levels - TODO support for custom levels.. */
type level =
  | Error
  | Warn
  | Info
  | Verbose
  | Debug
  | Silly;

module Config {
  type t;

  module Transport {
    type t;

    module Console {
      let createDefault: unit => t;
      let create: (
        ~level: string=?,
        ~silent: bool=?,
        ~eol: string=?,
        ~stderrLevels: array(string)=?,
        ~consoleWarnLevels: array(string)=?, unit) => t;
    }

    /* Extension libraries like bs-winston-aws-cloudwatch may provide additional ('a => t) functions to create new transports */
  };

  module Format {
    type t;
  };

  module Builder {
    let create: Transport.t => t;

    let setLevel: t => level => t;
    let setSilent: t => bool => t;
    let addFormat: t => Format.t => t;
    let addTransport: t => Transport.t => t
  };
};

/**
 * Example:
 * module ConfigBuilder = Winston.Config.Builder;
 * let consoleTransport = Winston.Config.Transport.Console.createDefault();
 * let winstonConfig = ConfigBuilder.create(consoleTransport) -> ConfigBuilder.setLevel(Debug);
 * let logger = Winston.createLogger(winstonConfig);
 */
let createLogger: Config.t => t;

let log: t => Js.Json.t => unit;

let logLevel: (t, level, ~message: string =?, ~json: Js.Json.t =?, ~error: exn =?, unit) => unit;

let error: (t, ~message: string =?, ~json: Js.Json.t =?, ~error: exn =?, unit) => unit;
let errorMsg: t => string => unit;
let errorExn: t => string => exn => unit;

let warn: (t, ~message: string =?, ~json: Js.Json.t =?, ~error: exn =?, unit) => unit;
let warnMsg: t => string => unit;
let warnExn: t => string => exn => unit;

let info: (t, ~message: string =?, ~json: Js.Json.t =?, unit) => unit;
let infoMsg: t => string => unit;

let debug: (t, ~message: string =?, ~json: Js.Json.t =?, unit) => unit;
let debugMsg: t => string => unit;
