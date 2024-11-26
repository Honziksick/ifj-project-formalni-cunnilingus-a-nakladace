// Semantic error: Expression in return of void function

const ifj = @import("ifj24.zig");
pub fn main() void {
    _ = f();
}

pub fn f() void {
    const str = ifj.string("hello");
    return ifj.length(str);
}