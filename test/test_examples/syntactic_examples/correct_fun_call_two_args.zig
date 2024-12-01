const ifj = @import("ifj24.zig");
pub fn main() void {
    foo(3.14, ifj.string("Please work"));
}

pub fn foo ( x : f64, y : []u8) void {
    ifj.write(x);
    ifj.write(y);
}