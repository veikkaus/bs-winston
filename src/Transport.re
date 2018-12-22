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
  let newConsoleTransport: (. config) => t = [%bs.raw {|
    function (config) {
      return new (require('winston').transports).Console(config);
    }
  |}];

  let create =
    (~level=?, ~silent=?, ~eol=?, ~stderrLevels=?, ~consoleWarnLevels=?, ()) =>
      newConsoleTransport(. config(
        ~level =? level,
        ~silent =? silent,
        ~eol =? eol,
        ~stderrLevels =? stderrLevels -> Belt.Option.map(Belt.List.toArray),
        ~consoleWarnLevels =? consoleWarnLevels -> Belt.Option.map(Belt.List.toArray),
        ()
      ));
};

module File {
  [@bs.deriving abstract]
  type config = {
    [@bs.optional] level: string,
    [@bs.optional] silent: bool,
    [@bs.optional] eol: string,
    [@bs.optional] filename: string,
    [@bs.optional] maxsize: int,
    [@bs.optional] maxFiles: int,
    [@bs.optional] tailable: bool,
    [@bs.optional] maxRetries: int,
    [@bs.optional] zippedArchive: bool,
    [@bs.optional] options: Js.t({.})
  };
  let newFileTransport: (. config) => t = [%bs.raw {|
    function (config) {
      return new (require('winston').transports).File(config);
    }
  |}];

  let create =
    (~level=?, ~silent=?, ~eol=?, ~filename=?, ~maxsize=?, ~maxFiles=?, ~tailable=?, ~maxRetries=?, ~zippedArchive=?, ~options=?, ()) =>
      newFileTransport(. config(
        ~level =? level,
        ~silent =? silent,
        ~eol =? eol,
        ~filename =? filename,
        ~maxsize =? maxsize,
        ~maxFiles =? maxFiles,
        ~tailable =? tailable,
        ~maxRetries =? maxRetries,
        ~zippedArchive =? zippedArchive,
        ~options =? options,
        ()
      ));
};

module Http {
  [@bs.deriving abstract]
  type config = {
    [@bs.optional] host: string,
    [@bs.optional] port: int,
    [@bs.optional] path: string,
    [@bs.optional] auth: Js.t({.}),
    [@bs.optional] ssl: bool
  };
  let newHttpTransport: (. config) => t = [%bs.raw {|
    function (config) {
      return new (require('winston').transports).Http(config);
    }
  |}];

  let create =
    (~host=?, ~port=?, ~path=?, ~auth=?, ~ssl=?, ()) =>
      newHttpTransport(. config(
        ~host =? host,
        ~port =? port,
        ~path =? path,
        ~auth =? auth,
        ~ssl =? ssl,
        ()
      ));
};

module Stream {
  [@bs.deriving abstract]
  type config = {
    [@bs.optional] level: string,
    [@bs.optional] silent: bool,
    [@bs.optional] eol: string,
    [@bs.optional] stream: Js.t({.})
  };
  let newStreamTransport: (. config) => t = [%bs.raw {|
    function (config) {
      return new (require('winston').transports).Stream(config);
    }
  |}];

  let create =
    (~level=?, ~silent=?, ~eol=?, ~stream=?, ()) =>
      newStreamTransport(. config(
        ~level =? level,
        ~silent =? silent,
        ~eol =? eol,
        ~stream =? stream,
        ()
      ));
};
