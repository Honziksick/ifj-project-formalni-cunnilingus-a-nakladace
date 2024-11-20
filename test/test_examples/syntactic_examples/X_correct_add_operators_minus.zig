const ifj = @import("ifj24.zig");
pub fn main() void {
    const x : ?i32 = 42;
    x = x - x;
}