const ifj = @import("ifj24.zig");

pub fn main() void {
    const y: i32 = 42;
    foo(y);
}

pub fn foo(x: i32) void {
    ifj.write(x);
}
