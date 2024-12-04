const ifj = @import("ifj24.zig");

pub fn main() void {}

pub fn foo(a: i32, b: ?i32, c: f64, d: ?f64, e: []u8, f: ?[]u8) void {
    ifj.write(a);
    ifj.write(b);
    ifj.write(c);
    ifj.write(d);
    ifj.write(e);
    ifj.write(f);
}