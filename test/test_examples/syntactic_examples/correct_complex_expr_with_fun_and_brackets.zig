const ifj = @import("ifj24.zig");

pub fn main() void {
    const x: i32 = 42;
    const y: i32 = 64;
    const z: i32 = 0 - 6;
    const float: f64 = 3.14;
    const vysledek = ((((((func1(x))) +
        ((((func2(y, z))) *
        ((func3(5)))))) -
        ((((ifj.f2i(float))) /
        ((func5(y, z, 15)))))) +
        (((func6(20, y)) *
        (func7(z)))) -
        (((ifj.f2i(float)) /
        (ifj.f2i(float))) * x)) +
        (((35) -
        ((y) *
        (ifj.f2i(float)))) / x) +
        ((func11(y, 45)) * 50) -
        (((55) /
        (ifj.f2i(float))) *
        (func13())) +
        70);

    ifj.write(vysledek);
}

pub fn func1(x: i32) i32 {
    return x;
}
pub fn func2(x: i32, y: i32) i32 {
    return x * y;
}
pub fn func3(x: i32) i32 {
    return x - 8;
}
pub fn func4(x: i32, y: i32) i32 {
    return x / y;
}
pub fn func5(x: i32, y: i32, z: i32) i32 {
    return 8 / 2 * z - 4 * x + y;
}
pub fn func6(x: i32, y: i32) i32 {
    return 4 * x + y;
}
pub fn func7(x: i32) i32 {
    return 42 - x;
}
pub fn func11(x: i32, y: i32) i32 {
    return 2 * y - x;
}
pub fn func13() i32 {
    return 1;
}
