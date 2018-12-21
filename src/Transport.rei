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
