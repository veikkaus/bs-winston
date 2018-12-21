# BsWinston

This is adapter of [winston](https://github.com/winstonjs/winston) for ReasonML.

# Install 

Not yet available via npm.

# Examples

Minimal console logging
```
module Builder = BsWinston.Builder;
open BsWinston.Open;

let consoleTransport = BsWinston.Transport.Console.create(());
let logger =
  Builder.create()
  -> Builder.setLevel(Builder.Debug)
  -> Builder.addTransport(consoleTransport)
  -> Builder.build;


logger -> error -> withMessage("Some error happened") -> log;

logger -> logDebugMsg("Abbreviation for logging a message at debug level");

/*prefixed logger:*/
let logger2 = logger -> withMessage("Prefixed always:");

logger2 -> logInfoMsg("Hello!");
/*> Prefixed always: Hello! */

```

# API

TODO
