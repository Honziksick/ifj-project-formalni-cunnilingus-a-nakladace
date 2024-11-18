// Korektni program
const ifj = @import("ifj24.zig");
pub fn main() void {
    const x : f64 = 5 + 10.0;
    const y : i32 = ifj.f2i(x);
    ifj.write(x);
}