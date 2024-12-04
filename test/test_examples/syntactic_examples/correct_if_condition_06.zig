const ifj = @import("ifj24.zig");

pub fn main() void {
    const x: i32 = 32;
    const y : i32 = 89;

    if(ifj.write("hello"))
    {
        ifj.write(1458242);
    }
    else
    {
        ifj.write(53878);
    }
}