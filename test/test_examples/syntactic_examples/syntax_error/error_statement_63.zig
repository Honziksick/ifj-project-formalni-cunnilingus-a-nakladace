// Syntax error: separator "," somewhere else than between function arguments

const ifj = @import("ifj24.zig"); 

pub fn main() void {
    var z : f64 = ,ifj.write(10.5, 5, 89.98);
}