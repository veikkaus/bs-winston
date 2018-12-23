type t;

let create: (
  ~level: string =?,
  ~format: Format.t =?,
  ~transports: list(Transport.t),
  ~silent: bool =?,
  ~errorKey: string =?,
  ~jsonKey: string =?,
  unit
) => t;

let error:   t => t;
let warn:    t => t;
let info:    t => t;
let verbose: t => t;
let debug:   t => t;
let silly:   t => t;

let withMessage: t => string => t;  /* or use 'a and enforce with Js.String.make() ? */
let withExn:     t => exn => t;
let withJson:    t => Js.Json.t => t;

let log: t => unit;


/* Common combinations abbreviated for usage convenience */
let logErrorMsg: t => string => unit;
let logErrorExn: t => string => exn => unit;
let logWarnMsg:  t => string => unit;
let logInfoMsg:  t => string => unit;
let logDebugMsg: t => string => unit;
