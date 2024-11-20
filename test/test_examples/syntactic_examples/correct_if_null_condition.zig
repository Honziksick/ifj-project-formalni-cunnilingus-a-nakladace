const ifj = @import("ifj24.zig");
pub fn main() void {
    const x = 59 + 3.14;
    var y : ?[]u8 = null;
    if(x == x) | y | {
        y = x + x;
    } else {
        x = y + y;
    }
}