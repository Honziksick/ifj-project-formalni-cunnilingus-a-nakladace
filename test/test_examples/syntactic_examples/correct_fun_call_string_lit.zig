const ifj = @import("ifj24.zig");
pub fn main() void {
    foo("Hello, World!");
    const greeting : ?[]u8 = foo("Hello, World!");
}
