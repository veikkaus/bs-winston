/*
 * This module exists to enable convenience syntax of:
 * open BsWinston.Open;
 */

let error:   Logger.t => Logger.t = Logger.error;
let warn:    Logger.t => Logger.t = Logger.warn;
let info:    Logger.t => Logger.t = Logger.info;
let verbose: Logger.t => Logger.t = Logger.verbose;
let debug:   Logger.t => Logger.t = Logger.debug;
let silly:   Logger.t => Logger.t = Logger.silly;

let withMessage: Logger.t => string => Logger.t    = Logger.withMessage;
let withExn:     Logger.t => exn => Logger.t       = Logger.withExn;
let withJson:    Logger.t => Js.Json.t => Logger.t = Logger.withJson;

let log: Logger.t => unit = Logger.log;

/*
 * Abbreviations for the most common combinations:
 */
let logErrorMsg = (logger, msg) => logger -> error -> withMessage(msg) -> log;
let logErrorExn = (logger, msg, e) => logger -> error -> withMessage(msg) -> withExn(e) -> log;

let logWarnMsg = (logger, msg) => logger -> warn -> withMessage(msg) -> log;
let logInfoMsg = (logger, msg) => logger -> info -> withMessage(msg) -> log;
let logDebugMsg = (logger, msg) => logger -> debug -> withMessage(msg) -> log;
