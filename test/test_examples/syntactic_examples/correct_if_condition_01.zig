const ifj = @import("ifj24.zig");

pub fn main() void {
    const float = 3.14;
    const x: i32 = 32;
    const y : i32 = float * 89;

    if((y < x))
    {
        ifj.write(1458242);
    }
    else
    {
        ifj.write(53878);
    }
}