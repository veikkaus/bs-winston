type t;

module Console {
  let create: (
    ~level: string =?,
    ~silent: bool =?,
    ~eol: string =?,
    ~stderrLevels: list(string) =?,
    ~consoleWarnLevels: list(string) =?,
    unit
  ) => t;
};

module File {
  let create: (
    ~level: string =?,
    ~silent: bool =?,
    ~eol: string =?,
    ~filename: string =?,
    ~maxsize: int =?,
    ~maxFiles: int =?,
    ~tailable: bool =?,
    ~maxRetries: int =?,
    ~zippedArchive: bool =?,
    ~options: Js.t({.}) =?,
    unit
  ) => t;
};

module Http {
  let create: (
    ~host: string =?,
    ~port: int =?,
    ~path: string =?,
    ~auth: Js.t({.}) =?,
    ~ssl: bool =?,
    unit
  ) => t;
};

module Stream {
  let create: (
    ~level: string =?,
    ~silent: bool =?,
    ~eol: string =?,
    ~stream: Js.t({.}) =?,
    unit
  ) => t;
};