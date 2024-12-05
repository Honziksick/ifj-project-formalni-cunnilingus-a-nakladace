const ifj = @import("ifj24.zig");
pub fn main() void {
    _ = foo(ifj.string("Hello, World!"));
    const greeting: ?[]u8 = foo(ifj.string("Hello, World!"));
    ifj.write(greeting);
}

pub fn foo(x: []u8) []u8 {
    return x;
}
