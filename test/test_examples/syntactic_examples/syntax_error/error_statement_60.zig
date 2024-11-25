// Syntax error: "ifj" somewhere else than in built-in function call

const ifj = @import("ifj24.zig"); 

pub fn main() void {
    var z : f64 = ifj.write(10.5 ifj, 5, 89.98);
}