const ifj = @import("ifj24.zig");
pub fn main() void {
    const x : ?i32 = 42;
    var y = 64.8;
    var z : f64 = 0 - 6;
const vysledek = (
  func1(
    (
      (
        x +
        (
          5 -
          (
            y *
            (
              z +
              anotherFunc1(
                x + (y * z)
              )
            )
          )
        )
      )
    )
  )
) +
(
  func2(
    (
      y / 2
    ),
    (
      z -
      anotherFunc2(
        x - y + (z * func4(x))
      )
    )
  ) *
  func3(
    5 * (2 / (y + z))
  )
)-
(
  ifj.func4(
    x - 10,
    10 + anotherFunc5(x * y)
  ) /
  func5(
    y * z,
    z / (2 + x),
    15 - x
  )
)+
(
  func6(
    20 * (x - y),
    y + z
  ) *
  func7(
    z * (5 / x)
  )
)+
(
  ifj.func8(
    25 - y,
    x + z,
    y * 3
  ) /
  ifj.func9()
)/ x +
(
  35 -
  (
    y * ifj.func10(
      40 + x,
      z * y
    )
  )
) / x +
(
  func11(
    y + 3,
    45 - z
  ) * 50
) -
(
  55 /
  ifj.func12(
    60 * z,
    y - 2,
    z + (5 - x)
  ) * func13(
    x * (2 + (y - z))
  )
) +
(
  70 - (x + y)
);
}