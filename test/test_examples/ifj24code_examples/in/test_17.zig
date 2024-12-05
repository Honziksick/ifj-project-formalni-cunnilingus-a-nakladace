const ifj = @import("ifj24.zig");

pub fn main() void{
    ifj.write(1e200);
    ifj.write("\n");
    ifj.write(1e-200);
    ifj.write("\n");
    ifj.write(1.1e200);
    ifj.write("\n");
    ifj.write(1.1e-200);
    ifj.write("\n");
    ifj.write(0-1.1e200);
    ifj.write("\n");
    ifj.write(0-1.1e-200);
    ifj.write("\n");
}