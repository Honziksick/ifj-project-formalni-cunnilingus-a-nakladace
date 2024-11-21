const ifj = @import("ifj24.zig");
pub fn main() void {
    const x = 59 + 3.14;
    if(x == x) | y | {
        y = x + x;
    } else {
        var z : ?[]u8 = null;
        z = x / x;
    }
}