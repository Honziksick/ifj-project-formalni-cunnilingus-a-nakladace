const ifj = @import("ifj24.zig");
pub fn main() void {
    const x: ?f64 = 54 + 3.14;
    if (x == x) |y| {
        ifj.write(y);
    } else {
        var z: ?f64 = null;
        z = x;
        ifj.write(z);
    }
}
