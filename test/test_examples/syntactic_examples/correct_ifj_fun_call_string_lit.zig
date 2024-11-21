const ifj = @import("ifj24.zig");
pub fn main() void {
    ifj.foo("Hello, World!");
    const greeting : ?[]u8 = ifj.foo("Hello, World!");
}
