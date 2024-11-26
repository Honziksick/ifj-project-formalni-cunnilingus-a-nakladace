// Semantic error: Expression in return of void function

const ifj = @import("ifj24.zig");
pub fn main() void {
    _ = f();
}

pub fn f() void {
    return 42.428 + 5;
}