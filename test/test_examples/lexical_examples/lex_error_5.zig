const ifj = @import("ifj24.zig");
pub fn main() void {
    ifj.write("Ahoj ");
    const x = ifj.string(\n);
    ifj.write("svete!\n");
    ifj.write(x);
}