let describe = BsMocha.Mocha.describe;
open BsMocha.Async;

module Assert = BsMocha.Assert;

describe("Demo", () => {
  it("should always pass", done_ => {
    done_();
  });
});
