const ifj = @import("ifj24.zig");
pub fn main() void {
    const x : ?[]u8 = "Bad semantic";
    x = x + x;
}