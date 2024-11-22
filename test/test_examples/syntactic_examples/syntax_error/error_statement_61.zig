// Syntax error: "ifj" somewhere else than in built-in function call

const ifj = @import("ifj24.zig"); 

pub fn main() void {
    var z : f64 = ifj.(10.5, 5, 89.98) / ifj.;
}