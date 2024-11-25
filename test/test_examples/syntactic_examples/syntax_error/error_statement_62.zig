// Syntax error: "ifj" somewhere else than in built-in function call

const ifj = @import("ifj24.zig"); 

pub fn main() void {
    var z : f64 = 8 / ifj.9();
}