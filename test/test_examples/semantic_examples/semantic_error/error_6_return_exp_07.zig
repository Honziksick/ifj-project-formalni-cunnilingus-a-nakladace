// Semantic error: Expression in return of void function

const ifj = @import("ifj24.zig");

pub fn g() i32 { return 5-2; }

pub fn main() void {
    _ = f();
}

pub fn f() void {
    return g();
}