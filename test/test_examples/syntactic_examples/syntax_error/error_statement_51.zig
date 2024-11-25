// Syntax error: dot somewhere else than between "ifj" and "and"

const ifj = @import("ifj24.zig"); 

pub fn main() void {
    var z : f64 = ifj.write(10.5,. 5, 89.98);
}