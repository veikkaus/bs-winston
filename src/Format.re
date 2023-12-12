type t;

/*
 * Bucklescript do not support dynamic variadic arguments for external, the following:
 * [@mel.module "winston"] [@mel.scope "format"] [@mel.variadic] external combineExt: array(t) => t = "combine";
 * fails with Belt.List.toArray(formats):
 *     "Error: function call with [@@mel.val "combineExt"]  is a primitive with [@mel.splice], it expects its
 *      `bs.splice` argument to be a syntactic array in the call site and  all arguments to be supplied"
 *
 * Therefore resorting to raw:
 */
let combineExt: (. array(t)) => t = [%mel.raw
  {|
  function (formats) {
    return require('winston').format.combine(...formats);
  }
|}
];

let combine = formats => combineExt(. Belt.List.toArray(formats));

[@mel.module "winston"] [@mel.scope "format"]
external alignExt: unit => t = "align";
let createAlign = alignExt;

module Cli = {
  [@deriving abstract]
  type config = {
    [@mel.optional]
    level: option(bool),
    [@mel.optional]
    all: option(bool),
    [@mel.optional]
    message: option(bool),
    [@mel.optional]
    colors: option(Js.Dict.t(string)),
    [@mel.optional]
    levels: option(Js.Dict.t(int)),
  };
  [@mel.module "winston"] [@mel.scope "format"]
  external cliExt: config => t = "cli";
  let create = (~level=?, ~all=?, ~message=?, ~colors=?, ~levels=?, ()) =>
    cliExt(config(~level?, ~all?, ~message?, ~colors?, ~levels?, ()));
};
let createCli = Cli.create;

module Colorize = {
  [@deriving abstract]
  type config = {
    [@mel.optional]
    level: option(bool),
    [@mel.optional]
    all: option(bool),
    [@mel.optional]
    message: option(bool),
    [@mel.optional]
    colors: option(Js.Dict.t(string)),
  };
  [@mel.module "winston"] [@mel.scope "format"]
  external colorizeExt: config => t = "colorize";
  let create = (~level=?, ~all=?, ~message=?, ~colors=?, ()) =>
    colorizeExt(config(~level?, ~all?, ~message?, ~colors?, ()));
};
let createColorize = Colorize.create;

module Json = {
  [@deriving abstract]
  type config = {
    [@mel.optional]
    replacer: option((. string, Js.Json.t) => Js.Json.t),
    [@mel.optional]
    space: option(int),
  };
  [@mel.module "winston"] [@mel.scope "format"]
  external jsonExt: config => t = "json";
  let create = (~replacer=?, ~space=?, ()) =>
    jsonExt(config(~replacer?, ~space?, ()));
};
let createJson = Json.create;

module Label = {
  [@deriving abstract]
  type config = {
    [@mel.optional]
    label: option(string),
    [@mel.optional]
    message: option(bool),
  };
  [@mel.module "winston"] [@mel.scope "format"]
  external labelExt: config => t = "label";
  let create = (~label=?, ~message=?, ()) =>
    labelExt(config(~label?, ~message?, ()));
};
let createLabel = Label.create;

[@mel.module "winston"] [@mel.scope "format"]
external logstashExt: unit => t = "logstash";
let createLogstash = logstashExt;

module Metadata = {
  [@deriving abstract]
  type config = {
    [@mel.optional]
    key: option(string),
    [@mel.optional]
    fillExcept: option(array(string)),
    [@mel.optional]
    fillWith: option(array(string)),
  };
  [@mel.module "winston"] [@mel.scope "format"]
  external metadataExt: config => t = "metadata";
  let create = (~key=?, ~fillExcept=?, ~fillWith=?, ()) =>
    metadataExt(
      config(
        ~key?,
        ~fillExcept=?Belt.Option.map(fillExcept, Belt.List.toArray),
        ~fillWith=?Belt.Option.map(fillWith, Belt.List.toArray),
        (),
      ),
    );
};
let createMetadata = Metadata.create;

[@mel.module "winston"] [@mel.scope "format"] external msExt: unit => t = "ms";
let createMs = msExt;

module PadLevels = {
  [@deriving abstract]
  type config = {
    [@mel.optional]
    levels: option(Js.Dict.t(int)),
  };
  [@mel.module "winston"] [@mel.scope "format"]
  external padLevelsExt: config => t = "padLevels";
  let create = (~levels=?, ()) => padLevelsExt(config(~levels?, ()));
};
let createPadLevels = PadLevels.create;

module PrettyPrint = {
  [@deriving abstract]
  type config = {
    [@mel.optional]
    depth: option(int),
    [@mel.optional]
    colorize: option(bool),
  };
  [@mel.module "winston"] [@mel.scope "format"]
  external prettyPrintExt: config => t = "prettyPrint";
  let create = (~depth=?, ~colorize=?, ()) =>
    prettyPrintExt(config(~depth?, ~colorize?, ()));
};
let createPrettyPrint = PrettyPrint.create;

[@mel.module "winston"] [@mel.scope "format"]
external printfExt: (Js.Json.t => string) => t = "printf";
let createPrintf = printfExt;

[@mel.module "winston"] [@mel.scope "format"]
external simpleExt: unit => t = "simple";
let createSimple = simpleExt;

[@mel.module "winston"] [@mel.scope "format"]
external splatExt: unit => t = "splat";
let createSplat = splatExt;

module Timestamp = {
  [@deriving abstract]
  type config = {
    [@mel.optional]
    format: option(string),
    [@mel.optional]
    alias: option(string),
  };
  [@mel.module "winston"] [@mel.scope "format"]
  external timestampExt: config => t = "timestamp";
  let create = (~format=?, ~alias=?, ()) =>
    timestampExt(config(~format?, ~alias?, ()));
};
let createTimestamp = Timestamp.create;

module Uncolorize = {
  [@deriving abstract]
  type config = {
    [@mel.optional]
    level: option(bool),
    [@mel.optional]
    message: option(bool),
    [@mel.optional]
    raw: option(bool),
  };
  [@mel.module "winston"] [@mel.scope "format"]
  external uncolorizeExt: config => t = "uncolorize";
  let create = (~level=?, ~message=?, ~raw=?, ()) =>
    uncolorizeExt(config(~level?, ~message?, ~raw?, ()));
};
let createUncolorize = Uncolorize.create;

type fnX = unit => t;
[@mel.module "winston"]
external formatExt: (Js.Dict.t(Js.Json.t) => Js.Dict.t(Js.Json.t)) => fnX =
  "format";

let fromMapFn: (Js.Dict.t(Js.Json.t) => Js.Dict.t(Js.Json.t)) => t =
  fn => {
    /* We have to mutate the original obj in order to preserve magical members (Symbols etc.): */
    let wrapper = obj => {
      let res = fn(obj);
      res
      ->Js.Dict.entries
      ->Belt.Array.forEach(((key, value)) => {
          Js.Dict.set(obj, key, value)
        });
      obj;
    };
    formatExt(wrapper, ());
  };
