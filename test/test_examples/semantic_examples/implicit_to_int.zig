// Korektni program
const ifj = @import("ifj24.zig");
pub fn main() void {
    const x : i32 = 5 + 10.0;
    const y : f64 = ifj.i2f(x);
    ifj.write(y);
}