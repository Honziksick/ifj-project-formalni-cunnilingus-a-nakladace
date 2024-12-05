const ifj = @import("ifj24.zig");
pub fn main() void {
    _ = foo(3.14);
    const pi = foo(3.14);
    ifj.write(pi);
}

pub fn foo(x: ?f64) ?f64 {
    return x;
}
