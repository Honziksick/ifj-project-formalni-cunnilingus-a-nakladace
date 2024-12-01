const ifj = @import("ifj24.zig");
pub fn main() void {
    _ = ifj.concat(ifj.string("3.14..."), ifj.string("Please work!"));
}