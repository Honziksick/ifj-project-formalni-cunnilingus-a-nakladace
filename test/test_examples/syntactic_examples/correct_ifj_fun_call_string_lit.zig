const ifj = @import("ifj24.zig");
pub fn main() void {
    ifj.write("Hello, World!");
    const greeting : ?[]u8 = null;
    ifj.write(greeting);
}
