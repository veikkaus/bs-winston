type t;

let combineExternal: (. array(t)) => t = [%bs.raw {|
    function (formats) {
      return (require('winston').format).combine(...formats);
    }
  |}];

let combine =
  formats => combineExternal(. Belt.List.toArray(formats));
