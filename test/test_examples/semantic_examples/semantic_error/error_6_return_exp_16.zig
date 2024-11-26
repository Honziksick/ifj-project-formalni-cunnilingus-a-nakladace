// Semantic error: Missing expression in return of non-void function

const ifj = @import("ifj24.zig");
pub fn main() void {
    _ = f();
}

pub fn f() ?f64 {
}