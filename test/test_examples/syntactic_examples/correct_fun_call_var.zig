const ifj = @import("ifj24.zig");
pub fn main() void {
    var x : i32 = 10;
    x = x;
    _ = foo(x);
}

pub fn foo ( z : i32 ) i32 {
    return z;
}