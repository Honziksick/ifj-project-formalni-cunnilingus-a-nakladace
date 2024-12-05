const ifj = @import("ifj24.zig");
pub fn main() void {
    var x: i32 = 10;
    x = 42;
    _ = x / x;
}
