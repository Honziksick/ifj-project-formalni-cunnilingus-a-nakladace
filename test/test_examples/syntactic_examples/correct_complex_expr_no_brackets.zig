const ifj = @import("ifj24.zig");
pub fn main() void {
    const x : ?i32 = 42;
    var y = 64.8;
    var z : f64 = 0 - 6;
    const vysledek = x + y * z - 5 / x + 10 * y - 15 / z * x + 20 - y * 25 / x + z * 30 - 35 / y * z + 40;
}