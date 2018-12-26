let describe = BsMocha.Mocha.describe;
open BsMocha.Async;
module Assert = BsMocha.Assert;
open BsYarp;
open Logger;


exception TestException(string);

let createTestLogger: unit => (Builder.t, (unit => Promise.t(string))) =
  () => {
    let (testTransport, createResultPromise) = TestTransport.create();
    let builder = Builder.create()
    -> Builder.setLevel(Builder.Debug)
    -> Builder.addTransport(testTransport);
    (builder, createResultPromise);
  };

let createTestLogger2 =
  () => {
    let (builder, createResultPromise) = createTestLogger();
    (builder
       -> Builder.addFormat(Format.createTimestamp(()))
       -> Builder.addFormat(Format.createSimple()),
     createResultPromise);
  };

let testResult: (unit => unit) => Promise.t(string) => string => unit =
  (done_, promise, expected) =>
    promise
    -> Promise.mapOk(res => {
      Assert.equal(res, expected);
      done_();
    })
    -> Promise.catch(e => Assert.fail(e));

let createJsExn: string => exn =
  msg =>
    try (Js.Exn.raiseError(msg)) {
      | e => e
    };

let messageToUpper: Js.Dict.t(Js.Json.t) => Js.Dict.t(Js.Json.t) =
  infoObj => {
    infoObj
    -> Js.Dict.get("message")
    -> Belt.Option.flatMap(Js.Json.decodeString)
    -> Belt.Option.map(Js.String.toUpperCase)
    -> Belt.Option.map(Js.Json.string)
    -> Belt.Option.map(Js.Dict.set(infoObj, "message", _))
    -> ignore;
    infoObj;
  };

describe("Basic Logging", () => {

  describe("without Formats", () => {

    it("should log strings", done_ => {
      let (builder, createResultPromise) = createTestLogger();
      let logger = builder -> Builder.build;

      let resultPromise = createResultPromise();
      logger -> withMessage("jobs") -> withMessage(3451) -> logDebugMsg("testing this");
      testResult(done_, resultPromise, {js|{"message":"jobs 3451 testing this","level":"debug"}|js});
    });

    it("should log correct level", done_ => {
      let (builder, createResultPromise) = createTestLogger();
      let logger = builder -> Builder.setLevel(Builder.Warn) -> Builder.build;

      let resultPromise = createResultPromise();
      logger -> logDebugMsg("testing this");
      logger -> logErrorMsg("Some Error");
      logger -> logInfoMsg("kuckoo");
      testResult(done_, resultPromise, {js|{"message":"Some Error","level":"error"}|js});
    });

    it("should log ocaml errors", done_ => {
      let (builder, createResultPromise) = createTestLogger();
      let logger = builder -> Builder.build;

      let error = TestException("testing");

      let resultPromise = createResultPromise();
      logger -> logErrorExn("My context", error);
      testResult(done_, resultPromise, {js|{"error":"BasicsTest-BsWinston.TestException,2,testing","message":"My context","level":"error"}|js});
    });

    it("should log Js.Exn.t errors", done_ => {
      let (builder, createResultPromise) = createTestLogger();
      let logger = builder -> Builder.build;

      let error = createJsExn("another brother");
      let resultPromise = createResultPromise();
      logger -> logErrorExn("My context", error);

      resultPromise
      -> Promise.mapOk(res => {
        Assert.equal(Js.String.startsWith({js|{"error":{"name":"Error","message":"another brother","stack":"Error: another brother|js}, res), true);
        Assert.equal(Js.String.endsWith({js|,"message":"My context","level":"error"}|js}, res), true);
        done_();
      })
      -> Promise.catch(e => Assert.fail(e));

    });

  });

  describe("format simple with timestamp", () => {
    it("should log strings", done_ => {
      let (builder, createResultPromise) = createTestLogger2();
      let logger = builder -> Builder.build;

      let resultPromise = createResultPromise();
      logger -> withMessage("jobs") -> withMessage(3451) -> logDebugMsg("testing this");

      resultPromise
      -> Promise.mapOk(res => {
        Assert.equal(Js.String.startsWith({js|debug: jobs 3451 testing this {"timestamp":"|js}, res), true);
        done_();
      })
      -> Promise.catch(e => Assert.fail(e));
    });
  });

  describe("custom formatter with simple output formatter", () => {
    it("should do the trick", done_ => {
      let (builder, createResultPromise) = createTestLogger();
      let logger =
        builder
        -> Builder.addFormat(Format.fromMapFn(messageToUpper))
        -> Builder.addFormat(Format.createSimple())
        -> Builder.build;

      let resultPromise = createResultPromise();
      logger -> logDebugMsg("testing this");
      testResult(done_, resultPromise, {js|debug: TESTING THIS|js});
    });
  });

});
