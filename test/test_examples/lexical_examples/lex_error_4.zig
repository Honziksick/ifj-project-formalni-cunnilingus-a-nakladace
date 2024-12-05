const ifj = @import("ifj24.zig");
pub fn main() void {
    const x = ifj.string(123.čus);
    ifj.write(x);
}